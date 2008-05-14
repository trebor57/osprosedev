/*
    Rose Online Server Emulator
    Copyright (C) 2006,2007 OSRose Team http://www.osrose.net
    
    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

    depeloped with Main erose/hrose source server + some change from the original eich source        
*/
#include "worldserver.h"

// Map Process
PVOID MapProcess( PVOID TS )
{
    while(GServer->ServerOnline)
    {
        pthread_mutex_lock( &GServer->PlayerMutex );
        pthread_mutex_lock( &GServer->MapMutex );
        for(UINT i=0;i<GServer->MapList.Map.size();i++)
        {
            CMap* map = GServer->MapList.Map.at(i);
            if( map->PlayerList.size()<1 )
                continue;
            // Player update //------------------------
            for(UINT j=0;j<map->PlayerList.size();j++)
            {
                CPlayer* player = map->PlayerList.at(j);
                if(!player->Session->inGame) continue;
                if(player->IsDead( )) continue;
                if(player->UpdateValues( ))
                    player->UpdatePosition( );
                if(player->IsOnBattle( ))
                    player->DoAttack( );
                player->RefreshBuff( );
                player->PlayerHeal( );
                player->Regeneration( );
                player->CheckPlayerLevelUP( );
            }
            // Monster update //------------------------
            pthread_mutex_lock( &map->MonsterMutex );
            for(UINT j=0;j<map->MonsterList.size();j++)
            {
                CMonster* monster = map->MonsterList.at(j);
                if((map->IsNight( ) || map->ghost==2))
                {
                    if (monster->IsGhostSeed( ))
                    {
						UINT etime = (UINT)GServer->round((clock( ) - monster->SpawnTime));
                        if(etime<20000) {if(!monster->PlayerInRange( )) continue; if(!monster->UpdateValues( )) continue; monster->UpdatePosition( );}
                        else if(etime>20000 && etime<120000) // convert seed to ghost btw 20 and 120sec after the seed was spawned
                        {
                            if(!monster->PlayerInRange( )) continue;
                            if(!monster->UpdateValues( )) continue;
                                monster->UpdatePosition( );
                            CPlayer* player = monster->GetNearPlayer( );
                            if(player==NULL) continue;
                            UINT montype = GServer->GetLevelGhost( player->Position->Map, player->Stats->Level );
                            map->ConverToMonster( monster, montype , true );
                            monster->StartAction( (CCharacter*)player, NORMAL_ATTACK, 0 );
                        }
                        else if (etime>120000) // delete seed after 120 seconde after the seed was spawned
                        {
                            BEGINPACKET( pak, 0x799 );
                            ADDWORD    ( pak, monster->clientid );
                            ADDWORD    ( pak, monster->clientid );
                            ADDWORD   ( pak, 0x8005 );
                            GServer->SendToVisible( &pak, monster );
                            map->DeleteMonster( monster, true, j ); continue;
                        }
                    }
                    if (monster->IsGhost( ))
                    {
                        UINT etime = (UINT)GServer->round((clock( ) - monster->SpawnTime));
                        if (etime>180000 && !monster->IsOnBattle( ) && !monster->IsMoving( )) // delete ghost after 180 sec after the ghost was spawned if not in battle
                        {
                            BEGINPACKET( pak, 0x799 );
                            ADDWORD    ( pak, monster->clientid );
                            ADDWORD    ( pak, monster->clientid );
                            ADDWORD   ( pak, 0x8005 );
                            GServer->SendToVisible( &pak, monster );
                            map->DeleteMonster( monster, true, j ); continue;
                        }
                    }
                }
                else
                if((!map->IsNight( ) && map->ghost!=2) && (monster->IsGhostSeed( ) || monster->IsGhost( ))) // if day, delete seed/ghost
                {
                           BEGINPACKET( pak, 0x799 );
                            ADDWORD    ( pak, monster->clientid );
                            ADDWORD    ( pak, monster->clientid );
                            ADDDWORD   ( pak, 0x8005 );
                            GServer->SendToVisible( &pak, monster );
                    map->DeleteMonster( monster, true, j );         
                    continue;
                }  
                if(!monster->PlayerInRange( )) continue;
                if(!monster->UpdateValues( )) continue;
                    monster->UpdatePosition( );
                if(monster->IsOnBattle( ))
                {
                    monster->DoAttack( );
                    if(monster->montype==208)//guardiantree 208
                    {
                        monster->Guardiantree(monster,map);
                    }
                }
                monster->RefreshBuff( );
                if (monster->IsSummon())
                {
                    monster->SummonUpdate(monster,map, j);
                    continue;
                }
                                        
                if(monster->IsDead( ))
                {
                    if(monster->montype==210)//Scarab210
                    {
                        monster->Scarab(monster,map);
                    }
                    else
                    monster->OnDie( );
                }
            }
            pthread_mutex_unlock( &map->MonsterMutex );
        }
        pthread_mutex_unlock( &GServer->MapMutex );
        pthread_mutex_unlock( &GServer->PlayerMutex );
        #ifdef _WIN32
        Sleep(GServer->Config.MapDelay);
        #else
        usleep(GServer->Config.MapDelay);
        #endif
    }
    pthread_exit( NULL );
}

// Visibility Process
PVOID VisibilityProcess(PVOID TS)
{
    while(GServer->ServerOnline)
    {
        pthread_mutex_lock( &GServer->PlayerMutex ); 
        pthread_mutex_lock( &GServer->MapMutex );
        for(UINT i=0;i<GServer->MapList.Map.size();i++)
        {
            CMap* map = GServer->MapList.Map.at(i);   
            if( map->PlayerList.size()<1 ) 
                continue;                              
            for(UINT j=0;j<map->PlayerList.size();j++)
            {
                CPlayer* player = map->PlayerList.at(j);
                if(!player->Session->inGame) continue;
                if(!player->VisiblityList()) Log(MSG_WARNING, "Visibility False: %u", player->clientid );        
                if( GServer->Config.AUTOSAVE == 1 )
                {                         
                    clock_t etime = clock() - player->lastSaveTime;   
                    if( etime >= GServer->Config.SAVETIME*1000 )  
                    {
                        player->savedata( );
                        player->lastSaveTime = clock();
                    }
                }                  
            }                                      
        }
        pthread_mutex_unlock( &GServer->MapMutex );
        pthread_mutex_unlock( &GServer->PlayerMutex );        
        #ifdef _WIN32
        Sleep(GServer->Config.VisualDelay);        
        #else
        usleep(GServer->Config.VisualDelay);             
        #endif
    }          
    pthread_exit(NULL);
}	

// World Process
PVOID WorldProcess( PVOID TS )
{
    while( GServer->ServerOnline )
    {      
        pthread_mutex_lock( &GServer->MapMutex );
        for(UINT i=0;i<GServer->MapList.Map.size();i++)
        {          
            CMap* map = GServer->MapList.Map.at(i);
            if( map->PlayerList.size()<1 ) 
                continue;               
            map->UpdateTime( );
            pthread_mutex_lock( &map->DropMutex );              
            map->CleanDrops( );
            pthread_mutex_unlock( &map->DropMutex );              
            pthread_mutex_lock( &map->MonsterMutex );                          
            map->RespawnMonster( );
            pthread_mutex_unlock( &map->MonsterMutex );                          
        }
        pthread_mutex_unlock( &GServer->MapMutex );               
        GServer->RefreshFairy( );
        #ifdef _WIN32
        Sleep(GServer->Config.WorldDelay);        
        #else
        usleep(GServer->Config.WorldDelay);             
        #endif
    }                
    pthread_exit(NULL);   
}

// Shutdown Server Process
void ShutdownServer(PVOID TS)
{
    int minutes = (int)TS;
    #ifdef _WIN32
    Sleep(minutes*60000);        
    #else
    usleep(minutes*60000);             
    #endif    
    Log( MSG_INFO, "Saving User Information... " );
    GServer->DisconnectAll();     
    Log( MSG_INFO, "Waiting Process to ShutDown... " );   
    GServer->ServerOnline = false;
    int status,res;
    res = pthread_join( GServer->WorldThread[0], (PVOID*)&status );
    if(res)
    {
        Log( MSG_WARNING, "World thread can't be joined" );
    }
    else
    {
        Log( MSG_INFO, "World thread closed." );
    }
    res = pthread_join( GServer->WorldThread[1], (PVOID*)&status );
    if(res)
    {
        Log( MSG_WARNING, "Visibility thread can't be joined" );
    }
    else
    {
        Log( MSG_INFO, "Visibility thread closed." );
    }
    res = pthread_join( GServer->MapThread[0], (PVOID*)&status ); 
    if(res)
    {
        Log( MSG_WARNING, "Map thread can't be joined" );
    }
    else
    {
        Log( MSG_INFO, "Map thread closed." );
    }          
    Log( MSG_INFO, "All Threads Closed." );               
    GServer->isActive = false;
    pthread_exit(NULL);
}
