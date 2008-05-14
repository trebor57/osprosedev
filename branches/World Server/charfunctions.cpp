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

    developed with Main erose/hrose source server + some change from the original eich source        
*/
#include "character.h"
#include "worldserver.h"

// return true if this character is monster
bool CCharacter::IsMonster( )
{
    if(CharType==2 || CharType==3)
        return true;
    return false;
}

// return true if this character is player
bool CCharacter::IsPlayer( )
{
    if(CharType==1)
        return true;
    return false;
}

// return true if HP is lower to 1
bool CCharacter::IsDead( )
{
    if(Stats->HP<=0)
        return true;
    return false;
}

// return true if this character is summon
bool CCharacter::IsSummon( )
{
    if(CharType==3)
        return true;
    return false;
}

// Virtual function [return item drop]
CDrop* CCharacter::GetDrop( )
{
    return NULL;
}

// start action [attack]
void CCharacter::StartAction( CCharacter* Target, BYTE action, UINT skillid )
{
    BEGINPACKET( pak, 0 );
    switch(action)
    {
        case NORMAL_ATTACK: 
        {//Log( MSG_INFO, "case NORMAL_ATTACK");
            RESETPACKET( pak, 0x798 );
            ADDWORD    ( pak, clientid );
            ADDWORD    ( pak, Target->clientid );
            ADDWORD    ( pak, Stats->Move_Speed );
            ADDFLOAT   ( pak, Target->Position->current.x*100 );
            ADDFLOAT   ( pak, Target->Position->current.y*100 );   
            Battle->target = Target->clientid;
            Battle->atktarget = Target->clientid;       
            Battle->atktype = action;  
            Position->destiny  = Target->Position->current;            
            Position->lastMoveTime = clock();                     
        }
        break;
        case SKILL_ATTACK: //Log( MSG_INFO, "case SKILL_ATTACK");
        case SKILL_BUFF:
        {//Log( MSG_INFO, "case SKILL_BUFF");
        //Log( MSG_INFO, "clientid :%i Target id :%i skillid :%i", clientid, Target->clientid, skillid);
        	RESETPACKET( pak, 0x7b3 );
        	ADDWORD    ( pak, clientid );
        	ADDWORD    ( pak, Target->clientid );
        	ADDWORD    ( pak, skillid );
        	ADDWORD    ( pak, 0000 );
            ADDFLOAT   ( pak, Target->Position->current.x*100 );
            ADDFLOAT   ( pak, Target->Position->current.y*100 );  
            Battle->target = Target->clientid;
          
            if(action==SKILL_ATTACK) Battle->skilltarget = Target->clientid;
            else Battle->bufftarget = Target->clientid;
            
            Battle->atktype = action; 
            Position->destiny  = Target->Position->current;
            Battle->skillid = skillid;
            Position->lastMoveTime = clock();                             
        }
        break;  
        case SKILL_AOE://Log( MSG_INFO, "case SKILL_AOE");
        case BUFF_SELF://Log( MSG_INFO, "case BUFF_SELF");
        case BUFF_AOE:
        {//Log( MSG_INFO, "case BUFF_AOE");
            RESETPACKET( pak, 0x7b2);
            ADDWORD    ( pak, clientid );
            ADDWORD    ( pak, skillid );  
            Battle->atktype = action; 
            Battle->skillid = skillid;          
        }
        break;
        case AOE_TARGET:
        {//Log( MSG_INFO, "case AOE_TARGET");
            RESETPACKET( pak, 0x7b4 );
            ADDWORD    ( pak, clientid );
            ADDWORD    ( pak, skillid );
            ADDFLOAT   ( pak, Target->Position->current.x*100 );
            ADDFLOAT   ( pak, Target->Position->current.y*100 ); 
            Battle->atktype = action; 
            Battle->skillid = skillid;  
            Battle->skilltarget = Target->clientid;                      
        }
        break;
        default: return;              
    }    
    GServer->SendToVisible( &pak, this );                               
    Battle->contatk = true;      
}

bool CCharacter::IsOnBattle( )
{  
    if(Battle->atktype==0) return false;
    switch(Battle->atktype)
    {
        case NORMAL_ATTACK: 
        {
            if(Battle->atktarget!=0) return true;
        }
        break;
        case SKILL_ATTACK: 
        case AOE_TARGET: 
        {
            if(Battle->skilltarget!=0 && Battle->skillid!=0) return true;
        }
        break;        
        case SKILL_BUFF:  
        {
            if(Battle->bufftarget!=0 && Battle->skillid!=0) return true;
        }
        break;        
        case SKILL_AOE: 
        case SKILL_SELF: 
        case BUFF_SELF: 
        case BUFF_AOE:
        {
            if(Battle->skillid!=0) return true; 
        }
        break;            
        default: return false;        
    }
}

bool CCharacter::CanAttack( )
{
    clock_t etime = clock() - Battle->lastAtkTime;
    if (Stats->Attack_Speed<1)
    {
        Log( MSG_INFO, "weapon with no attackspeed ");//%i"items[7].itemnum );
        return false;
    }
    if( etime < CLOCKS_PER_SEC * 100 / Stats->Attack_Speed ) return false;         
    return true;
}

// return true if target is reached
bool CCharacter::IsTargetReached( CCharacter* Enemy, CSkills* skill )
{
    CMap* map = GServer->MapList.Index[Position->Map];
    float distance = distance = GServer->distance( Position->current, Enemy->Position->current );
    if(skill==NULL)
    {
        if(distance<=Stats->Attack_Distance)
            return true;
    }
    else
    {
        if(distance<=skill->range)
            return true;
    }
    return false;
}

// return true if the character is moving
bool CCharacter::IsMoving( )
{
    if(Position->current.x == Position->destiny.x && Position->current.y == Position->destiny.y )
        return false;
    return true;
}

// VIRTUAL [add player damage]
void CCharacter::AddDamage( CCharacter* enemy, long int hitpower)
{

}

// return character target
CCharacter* CCharacter::GetCharTarget( )
{
    return GServer->MapList.Index[Position->Map]->GetCharInMap( Battle->target );
}

// VIRTUAL [reduce ammon/bullets/cannons]
void CCharacter::ReduceABC( )
{
    
}

// VIRTUAL [get extra buff value]
int CCharacter::GetEVBuff( )
{
    return 0;
}

// VIRTUAL [return intelligence]
unsigned int CCharacter::GetInt( )
{    
    return 15;
}


// Check For Debuffs
void CCharacter::RefreshBuff( )
{
    bool bflag = false;
    for( UINT i=0;i<30;i++)
    {
        if(MagicStatus[i].Buff==0) continue;
        clock_t etime = clock() - MagicStatus[i].BuffTime;
        if( etime >= MagicStatus[i].Duration * CLOCKS_PER_SEC )
        {
            switch(MagicStatus[i].Buff)
            {
                case A_ATTACK:
                    if(i<15)
                        Status->Attack_up = 0xff;
                    else
                        Status->Attack_down = 0xff;
                    Stats->Attack_Power = GetAttackPower( );
                break;
                case A_DEFENSE:
                    if(i<15)
                        Status->Defense_up= 0xff;
                    else
                        Status->Defense_down = 0xff;
                    Stats->Defense = GetDefense( );
                break;       
                case A_ACCUR:
                    if(i<15)
                        Status->Accury_up= 0xff;
                    else
                        Status->Accury_down = 0xff;
                    Stats->Accury = GetAccury( );
                break;                                         
                case A_MRESIST:
                    if(i<15)
                        Status->Magic_Defense_up = 0xff;
                    else
                        Status->Magic_Defense_down = 0xff;
                    Stats->Magic_Defense = GetMagicDefense( );
                break;       
                case A_DODGE:
                    if(i<15)
                        Status->Dodge_up = 0xff;
                    else
                        Status->Dodge_down = 0xff;
                    Stats->Dodge = GetDodge( );
                break;      
                case A_DASH:
                    if(i<15)
                        Status->Dash_up = 0xff;
                    else
                        Status->Dash_down = 0xff;
                    Stats->Move_Speed = GetMoveSpeed( );
                break;        
                case A_HASTE:
                    if(i<15)
                        Status->Haste_up = 0xff;
                    else
                        Status->Haste_down = 0xff;
                    Stats->Attack_Speed = GetAttackSpeed( );
                break;
                case A_CRITICAL:
                    if(i<15)
                        Status->Critical_up = 0xff;
                    else
                        Status->Critical_down = 0xff;
                    Stats->Critical = GetCritical( );
                break;    
                case A_MAX_HP:
                    if(i<15)
                        Status->HP_up = 0xff;
                    else
                        Status->HP_down = 0xff;
                    Stats->MaxHP = GetMaxHP( );
                break;                            
                case A_MAX_MP:
                    if(i<15)
                        Status->MP_up = 0xff;
                    else
                        Status->MP_down = 0xff;
                    Stats->MaxMP = GetMaxMP( );
                break;
                case A_GMExtra_Damage:
                case A_Extra_Damage:
                     if(i<15)
                     {
                        Status->ExtraDamage_up = 0xff;
                        Stats->ExtraDamage = 0;
                     }
                        
                     else
                         Status->ExtraDamage_down = 0xff;
                         
                break;
            }
            MagicStatus[i].Buff = 0;
            MagicStatus[i].BuffTime = 0;            
            MagicStatus[i].Duration = 0;
            MagicStatus[i].Value = 0;
            bflag = true;
        }
    }        
    if(bflag)
    {
        BEGINPACKET( pak,0x7b7 );
        ADDWORD    ( pak, clientid );
        ADDDWORD   ( pak, GServer->BuildBuffs( this ) );            
        GServer->SendToVisible( &pak, this );        
    } 
}

// VIRTUAL [return party pointer]
CParty* CCharacter::GetParty( )
{
    return NULL;
}

bool CCharacter::IsAttacking( )
{
    if(IsOnBattle( ))
    {
        switch(Battle->atktype)
        {
            case SKILL_BUFF:
            case BUFF_SELF:
            case BUFF_AOE:
                return false;
        }
    }   
    else return false; 
    return true;
}
