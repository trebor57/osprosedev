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
#include "worldmonster.h"

// called when a monster is attacked  [attack/use atk skill/use buff/run/summon]//Enemy is the one attacking?
bool CMonster::OnBeAttacked( CCharacter* Enemy )
{
    if(!IsOnBattle( ))
        StartAction( Enemy, NORMAL_ATTACK, 0 );
 /*   if(Enemy->CharType==1)
    {
        CPlayer* thismon=GServer->GetClientByID(Enemy->clientid,0);
        Log( MSG_INFO, "Attacking player is %s", thismon->CharInfo->charname);//thismon->montype );
    }*/
    //if(thismon->montype==
    return true;        
}

// called when a monster die [give exp/give extra drop]
bool CMonster::OnDie( )
{
    CMap* map = GServer->MapList.Index[Position->Map];
    if(map->ghost!=0)
    {
     if((map->IsNight( ) || map->ghost==2) && !IsGhost( ) && !IsGhostSeed( ) && !IsSummon( ))
       {
        UINT gs = GServer->RandNumber( 0, 100 );
        if(gs<30) // 30% / 70%
            {   // spawn a ghost seed [for now will be 701 [level 1 ghost seed] ]
                map->AddMonster( 701, Position->current, 0, NULL, NULL, Position->respawn, true );
            }
        }
    }
    
    GServer->GiveExp( this );
    return true; 
}
// called when we create a new monster [attack players?]
bool CMonster::OnSpawn( bool Attack )
{
    if(IsGhost( ) && Attack )
    {
        CPlayer* player = GetNearPlayer( );
        if(player==NULL) // no players or too far
            return true;
        StartAction( (CCharacter*)player, NORMAL_ATTACK, 0 ); 
    }
    return true;    
}

// [not]called when a monster is almost dead [run/use only skills/call a healer? :S xD]
bool CMonster::OnAlmostDie( )
{
    return true;    
}

// called when a enemy is on sight 
bool CMonster::OnEnemyOnSight( CCharacter* Enemy )
{
    clock_t etime = clock() - lastSighCheck;
    if(etime<5000) return true;
    if(!IsOnBattle( ))
    {
        if(thisnpc->aggresive>1)
        {
            UINT aggro = GServer->RandNumber(2,15);
            if(thisnpc->aggresive>=aggro && !IsGhostSeed( ))
            {
                StartAction( (CCharacter*) Enemy, NORMAL_ATTACK, 0 );
            }
            else
            if(IsGhostSeed( ) || thisnpc->aggresive>5)
                MoveTo( Enemy->Position->current, true );
        }
        lastSighCheck = clock();
    }
    return true;    
}

// called when enemy die
bool CMonster::OnEnemyDie( CCharacter* Enemy )
{    
    Position->destiny = Position->source;
    ClearBattle( Battle );
    MoveTo( Position->source );
    return true;        
}

bool CMonster::OnFar( )
{
    Position->destiny = Position->source;
    ClearBattle( Battle );
    MoveTo( Position->source );
    return true;
}
bool CMonster::Guardiantree(CMonster* monster,CMap* map)
{
     if(Stats->HP>9000)monster->hitcount=0;
     switch (monster->hitcount)
     {
         case 0:
              if(Stats->HP<9000)
              {
                  for(unsigned char i=0;i<5;i++){
                      fPoint position = GServer->RandInCircle( monster->Position->current, 5 );
                     CMonster* newmonster=map->AddMonster( 210, position, 0, NULL, NULL, 0, true );
                     newmonster->hitcount=0;
                  }
                  monster->hitcount=1;
              }
              break;
         case 1:
              if(Stats->HP<8000)
              {
                  for(unsigned char i=0;i<5;i++){
                      fPoint position = GServer->RandInCircle( monster->Position->current, 5 );
                      CMonster* newmonster=map->AddMonster( 210, position, 0, NULL, NULL, 0, true );
                     newmonster->hitcount=0;
                  }
                  monster->hitcount=2;
              }
              break;
         case 2:
              if(Stats->HP<7000)
              {
                  for(unsigned char i=0;i<5;i++){
                      fPoint position = GServer->RandInCircle( monster->Position->current, 5 );
                      CMonster* newmonster=map->AddMonster( 210, position, 0, NULL, NULL, 0, true );
                     newmonster->hitcount=0;
                  }
                  monster->hitcount=3;
              }
              break;
         case 3:
              if(Stats->HP<6000)
              {
                  for(unsigned char i=0;i<5;i++){
                      fPoint position = GServer->RandInCircle( monster->Position->current, 5 );
                      CMonster* newmonster=map->AddMonster( 210, position, 0, NULL, NULL, 0, true );
                     newmonster->hitcount=0;
                  }
                  monster->hitcount=4;
              }
              break;
         case 4:
              if(Stats->HP<5000)
              {
                  for(unsigned char i=0;i<5;i++)
                  {
                      fPoint position = GServer->RandInCircle( monster->Position->current, 5 );
                      CMonster* newmonster=map->AddMonster( 210, position, 0, NULL, NULL, 0, true );
                     newmonster->hitcount=0;
                  }
                  monster->hitcount=5;
                  CDrop* thisdrop = new CDrop;
		          assert(thisdrop);
		          thisdrop->clientid = GServer->GetNewClientID();
		          thisdrop->type = 2; // ITEM
		          thisdrop->pos = GServer->RandInCircle( monster->Position->current, 3 );
		          thisdrop->posMap = monster->Position->Map;
		          thisdrop->droptime = time(NULL);
		          thisdrop->amount = 1;
		          thisdrop->item.count = thisdrop->amount;
		          thisdrop->owner = 0;
		          thisdrop->item.itemtype=10;
		          thisdrop->item.itemnum=rand()%4+10;
		          map->AddDrop( thisdrop );
              }
              break;
         case 5:
              if(Stats->HP<4000)
              {
                  for(unsigned char i=0;i<5;i++){
                      fPoint position = GServer->RandInCircle( monster->Position->current, 5 );
                      CMonster* newmonster=map->AddMonster( 210, position, 0, NULL, NULL, 0, true );
                     newmonster->hitcount=0;
                  }
                  monster->hitcount=6;
              }
              break;
         case 6:
              if(Stats->HP<3000)
              {
                  for(unsigned char i=0;i<5;i++){
                      fPoint position = GServer->RandInCircle( monster->Position->current, 5 );
                     CMonster* newmonster=map->AddMonster( 210, position, 0, NULL, NULL, 0, true );
                     newmonster->hitcount=0;
                  }
                  monster->hitcount=7;
                  CDrop* thisdrop = new CDrop;
		          assert(thisdrop);
		          thisdrop->clientid = GServer->GetNewClientID();
		          thisdrop->type = 2; // ITEM
		          thisdrop->pos = GServer->RandInCircle( monster->Position->current, 3 );
		          thisdrop->posMap = monster->Position->Map;
		          thisdrop->droptime = time(NULL);
		          thisdrop->amount = 1;
		          thisdrop->item.count = thisdrop->amount;
		          thisdrop->owner = 0;
		          thisdrop->item.itemtype=10;
		          thisdrop->item.itemnum=rand()%4+29;
		          map->AddDrop( thisdrop );
              }
              break;
         case 7:
              if(Stats->HP<2000)
              {
                  for(unsigned char i=0;i<5;i++){
                      fPoint position = GServer->RandInCircle( monster->Position->current, 5 );
                      CMonster* newmonster=map->AddMonster( 210, position, 0, NULL, NULL, 0, true );
                     newmonster->hitcount=0;
                  }
                  monster->hitcount=8;
              }
              break;
         default:
                 monster->hitcount=9;
                 
     }
	 return true;
}
bool CMonster::Scarab(CMonster* monster,CMap* map)
{
     fPoint position;
     CMonster* othermon;
     CMonster* newmonster;
     if (monster->hitcount>5)monster->hitcount=0;
    switch (monster->hitcount)
    {
        case 0:
             position = GServer->RandInCircle( monster->Position->current, 1 );
             newmonster=map->AddMonster( 210, position, 0, NULL, NULL, 0, true );
             newmonster->hitcount=1;
             GServer->ClearClientID( monster->clientid );
             for(UINT i=0;i<map->MonsterList.size();i++)
            {
                othermon = map->MonsterList.at(i);
                if(othermon==monster)
                {
                    map->MonsterList.erase( map->MonsterList.begin()+i );
                    delete monster;
                }
            }
             break;
        case 1:
             position = GServer->RandInCircle( monster->Position->current, 1 );
             newmonster=map->AddMonster( 210, position, 0, NULL, NULL, 0, true );
             newmonster->hitcount=2;
             GServer->ClearClientID( monster->clientid );
              for(UINT i=0;i<map->MonsterList.size();i++)
            {
                othermon = map->MonsterList.at(i);
                if(othermon==monster)
                {
                    map->MonsterList.erase( map->MonsterList.begin()+i );
                    delete monster;
                }
            }
             break;
        case 2:
             CDrop* thisdrop = new CDrop;
		     assert(thisdrop);
		     thisdrop->clientid = GServer->GetNewClientID();
		     thisdrop->type = 2; // ITEM
		     thisdrop->pos = GServer->RandInCircle( monster->Position->current, 3 );
		     thisdrop->posMap = monster->Position->Map;
		     thisdrop->droptime = time(NULL);
		     thisdrop->amount = rand()%3+1;
		     thisdrop->item.count = thisdrop->amount;
		     thisdrop->owner = 0;
		     thisdrop->item.itemtype=12;
		     thisdrop->item.itemnum=rand()%10+11;
		     map->AddDrop( thisdrop );
             GServer->ClearClientID( monster->clientid );
              for(UINT i=0;i<map->MonsterList.size();i++)
            {
                othermon = map->MonsterList.at(i);
                if(othermon==monster)
                {
                    map->MonsterList.erase( map->MonsterList.begin()+i );
                    delete monster;
                }
            }
             break;
    }
	return true;
}
bool CMonster::SummonUpdate(CMonster* monster, CMap* map, UINT j)
{
     BEGINPACKET( pak, 0x799 );
 	clock_t etime = clock() - lastDegenTime;
    if( etime >= 4 * CLOCKS_PER_SEC && Stats->HP > 0 )
    {
        //Log(MSG_INFO, "Summon HP :%i",Stats->HP );
        //Log(MSG_INFO, "Summon owner :%i",owner );
        Stats->HP -= 10;               
        lastDegenTime = clock();
        if (Stats->HP < 1)
        {
            //BEGINPACKET( pak, 0x799 );
            ADDWORD    ( pak, monster->clientid );
            ADDWORD    ( pak, monster->clientid );
            ADDDWORD   ( pak, 0x8005 );
            GServer->SendToVisible( &pak, monster );
            map->DeleteMonster( monster, true, j );
        }
        else
        if (montype>800&&montype<811)
        {
            CPlayer* ownerclient = monster->GetOwner( );
            float distance = 0x9;
            for(UINT i=0;i<GServer->MapList.Index[Position->Map]->PlayerList.size();i++)
            {
                CPlayer* thisclient = GServer->MapList.Index[Position->Map]->PlayerList.at(i);
                float tempdist = GServer->distance( Position->current, thisclient->Position->current );
                if(tempdist<distance)
                {
                    thisclient->Stats->HP+=((((montype-800)*5)+((ownerclient->Stats->Level)/10))+10);
                    thisclient->Stats->MP+=(((montype-800)*5)+((ownerclient->Stats->Level)/25));
                    BEGINPACKET( pak, 0x7b2);
                    ADDWORD    ( pak, monster->clientid );
                    ADDWORD    ( pak, 0x0b9d );
                    ADDBYTE    ( pak, 0x06 );
                    thisclient->client->SendPacket(&pak);
                    
                    RESETPACKET( pak, 0x7b5);
                    ADDWORD    ( pak, thisclient->clientid );
                    ADDWORD    ( pak, monster->clientid );
                    ADDWORD    ( pak, 0x0b9d+0x3000+((ownerclient->Stats->Level%4)*0x4000) );
                    ADDBYTE    ( pak, ownerclient->Stats->Level/4 );
                    thisclient->client->SendPacket(&pak);
                    
                    RESETPACKET( pak, 0x7b9);
                    ADDWORD    ( pak, monster->clientid );
                    ADDWORD    ( pak, 0x0b9d );
                    thisclient->client->SendPacket(&pak);
                }
            }
        }
    }
    else
    if (Stats->HP < 1)
        {
            ADDWORD    ( pak, monster->clientid );
            ADDWORD    ( pak, monster->clientid );
            ADDDWORD   ( pak, 0x8005 );
            GServer->SendToVisible( &pak, monster );
            map->DeleteMonster( monster, true, j );
        }
    return true;
}
     
