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

// Add Player buffs
bool CWorldServer::AddBuffs( CSkills* thisskill, CCharacter* character, int Evalue )
{
    bool bflag = false;
    for(unsigned int i=0;i<3;i++)            
    {
        if(thisskill->buff[i]!=0)
        {
            bflag = CheckABuffs( thisskill, character, Evalue, i );
        }
    }
    return bflag;
}

// Add Player DeBuffs
bool CWorldServer::AddDeBuffs( CSkills* thisskill, CCharacter* character, int Evalue )
{
    bool bflag = false;
    for(unsigned int i=0;i<3;i++)            
    {
        if(thisskill->buff[i]!=0)
        {
            bflag = CheckDBuffs( thisskill, character, Evalue, i );
        }
    }
    return bflag;
}

// Add Player Dbuffs
bool CWorldServer::AddBuffs( CSkills* thisskill, CCharacter* character, int Evalue, bool bflag )
{
    if(thisskill->buff[0]!=0)  
    {
        CheckABuffs( thisskill, character, Evalue, 0 );
    }
    if(thisskill->buff[1]!=0 && thisskill->buff[2]==0)
    {
        CheckDBuffs( thisskill, character, Evalue, 1 );
    }
    else
    {
        CheckABuffs( thisskill, character, Evalue, 1 );
    }
    if(thisskill->buff[2]!=0)
    {
        CheckDBuffs( thisskill, character, Evalue, 2 );
    }
    return true;
}

// Add DeBuffs
bool CWorldServer::CheckABuffs( CSkills* thisskill, CCharacter* character, int Evalue ,int i)
{//Log( MSG_INFO, "checkabuffs %i",( thisskill->buff[i] ));
    bool bflag = false;
    switch( thisskill->buff[i] )
    {
        case A_GMExtra_Damage:    
        case A_Extra_Damage:
        {
             CBValue BuffValue = GetBuffValue( thisskill, character, Evalue, i, 
                                            character->Status->ExtraDamage_up, 
                                            character->Status->ExtraDamage_down, 
                                            character->Stats->ExtraDamage, true );
             if(BuffValue.NewValue!=0)
            {                       
                UINT j = BuffValue.Position;   
                character->Stats->ExtraDamage = thisskill->atkpower;
                character->Status->ExtraDamage_up = j;
                character->MagicStatus[j].Buff = thisskill->buff[i];
                character->MagicStatus[j].BuffTime = clock();
                character->MagicStatus[j].Duration = thisskill->duration;  
                character->MagicStatus[j].Value = BuffValue.Value;                  
                bflag = true;
            }
        }
        break;
        case A_ATTACK:
        {
            CBValue BuffValue = GetBuffValue( thisskill, character, Evalue, i, 
                                            character->Status->Attack_up, 
                                            character->Status->Attack_down, 
                                            character->Stats->Attack_Power, true );                      
            if(BuffValue.NewValue!=0)
            {                       
                UINT j = BuffValue.Position;   
                character->Stats->Attack_Power = BuffValue.NewValue;
                character->Status->Attack_up = j;
                character->MagicStatus[j].Buff = thisskill->buff[i];
                character->MagicStatus[j].BuffTime = clock();
                character->MagicStatus[j].Duration = thisskill->duration;  
                character->MagicStatus[j].Value = BuffValue.Value;                  
                bflag = true;
            }                                            
        }
        break;
        case A_DEFENSE:
        {
            CBValue BuffValue = GetBuffValue( thisskill, character, Evalue, i, 
                                            character->Status->Defense_up, 
                                            character->Status->Defense_down, 
                                            character->Stats->Defense, true );                      
            if(BuffValue.NewValue!=0)
            {       
                UINT j = BuffValue.Position;           
                character->Stats->Defense = BuffValue.NewValue;
                character->Status->Defense_up = j;
                character->MagicStatus[j].Buff = thisskill->buff[i];
                character->MagicStatus[j].BuffTime = clock();
                character->MagicStatus[j].Duration = thisskill->duration;  
                character->MagicStatus[j].Value = BuffValue.Value;                  
                bflag = true;
            } 
        }
        break;
        case A_ACCUR:
        {
            CBValue BuffValue = GetBuffValue( thisskill, character, Evalue, i, 
                                            character->Status->Accury_up, 
                                            character->Status->Accury_down, 
                                            character->Stats->Accury, true );                      
            if(BuffValue.NewValue!=0)
            {       
                UINT j = BuffValue.Position;           
                character->Stats->Accury = BuffValue.NewValue;
                character->Status->Accury_up = j;
                character->MagicStatus[j].Buff = thisskill->buff[i];
                character->MagicStatus[j].BuffTime = clock();
                character->MagicStatus[j].Duration = thisskill->duration;  
                character->MagicStatus[j].Value = BuffValue.Value;                  
                bflag = true;
            } 
        }
        break;
        case A_MRESIST:
        {
            CBValue BuffValue = GetBuffValue( thisskill, character, Evalue, i, 
                                            character->Status->Magic_Defense_up, 
                                            character->Status->Magic_Defense_down, 
                                            character->Stats->Magic_Defense, true );
            if(BuffValue.NewValue!=0)
            {       
                UINT j = BuffValue.Position;           
                character->Stats->Magic_Defense = BuffValue.NewValue;
                character->Status->Magic_Defense_up = j;
                character->MagicStatus[j].Buff = thisskill->buff[i];
                character->MagicStatus[j].BuffTime = clock();
                character->MagicStatus[j].Duration = thisskill->duration;  
                character->MagicStatus[j].Value = BuffValue.Value;                  
                bflag = true;
            } 
        }
        break;
        case A_DODGE:
        {
            CBValue BuffValue = GetBuffValue( thisskill, character, Evalue, i, 
                                            character->Status->Dodge_up, 
                                            character->Status->Dodge_down, 
                                            character->Stats->Dodge, true );                      
            if(BuffValue.NewValue!=0)
            {       
                UINT j = BuffValue.Position;           
                character->Stats->Dodge = BuffValue.NewValue;
                character->Status->Dodge_up = j;
                character->MagicStatus[j].Buff = thisskill->buff[i];
                character->MagicStatus[j].BuffTime = clock();
                character->MagicStatus[j].Duration = thisskill->duration;  
                character->MagicStatus[j].Value = BuffValue.Value;                  
                bflag = true;
            } 
        }
        break;    
        case A_DASH:
        {
            CBValue BuffValue = GetBuffValue( thisskill, character, Evalue, i, 
                                            character->Status->Dash_up, 
                                            character->Status->Dash_down, 
                                            character->Stats->Move_Speed, true );                      
            if(BuffValue.NewValue!=0)
            {       
                UINT j = BuffValue.Position;           
                character->Stats->Move_Speed = BuffValue.NewValue;
                character->Status->Dash_up = j;
                character->MagicStatus[j].Buff = thisskill->buff[i];
                character->MagicStatus[j].BuffTime = clock();
                character->MagicStatus[j].Duration = thisskill->duration;  
                character->MagicStatus[j].Value = BuffValue.Value;                  
                bflag = true;
            } 
        }
        break;  
        case A_HASTE:
        {
            CBValue BuffValue = GetBuffValue( thisskill, character, Evalue, i, 
                                            character->Status->Haste_up, 
                                            character->Status->Haste_down, 
                                            (UINT)character->Stats->Attack_Speed, true );                      
            if(BuffValue.NewValue!=0)
            {       
                UINT j = BuffValue.Position;           
                character->Stats->Attack_Speed = BuffValue.NewValue;
                character->Status->Haste_up = j;
                character->MagicStatus[j].Buff = thisskill->buff[i];
                character->MagicStatus[j].BuffTime = clock();
                character->MagicStatus[j].Duration = thisskill->duration;  
                character->MagicStatus[j].Value = BuffValue.Value;                  
                bflag = true;
            } 
        }
        break;  
        case A_CRITICAL:
        {
            CBValue BuffValue = GetBuffValue( thisskill, character, Evalue, i, 
                                            character->Status->Critical_up, 
                                            character->Status->Critical_down, 
                                            character->Stats->Critical, true );                      
            if(BuffValue.NewValue!=0)
            {       
                UINT j = BuffValue.Position;           
                character->Stats->Critical = BuffValue.NewValue;
                character->Status->Critical_up = j;
                character->MagicStatus[j].Buff = thisskill->buff[i];
                character->MagicStatus[j].BuffTime = clock();
                character->MagicStatus[j].Duration = thisskill->duration;  
                character->MagicStatus[j].Value = BuffValue.Value;                  
                bflag = true;
            } 
        }
        break;                 
        case A_HP:
        case A_MAX_HP:
        {
            if(thisskill->duration<=0)//is heal
            {
                if(character->Stats->HP<=0)
                    return true;
                character->Stats->HP += (long int)thisskill->value1[i] + Evalue - 8;
                if(character->Stats->HP > character->Stats->MaxHP )
                    character->Stats->HP = character->Stats->MaxHP;                  
                return true;
            }
            CBValue BuffValue = GetBuffValue( thisskill, character, Evalue, i, 
                                            character->Status->HP_up, 
                                            character->Status->HP_down, 
                                            character->Stats->MaxHP, true );                      
            if(BuffValue.NewValue!=0)
            {       
                UINT j = BuffValue.Position;           
                character->Stats->MaxHP = BuffValue.NewValue;
                character->Status->HP_up = j;
                character->MagicStatus[j].Buff = thisskill->buff[i];
                character->MagicStatus[j].BuffTime = clock();
                character->MagicStatus[j].Duration = thisskill->duration;  
                character->MagicStatus[j].Value = BuffValue.Value;                  
                bflag = true;
            } 
        }
        break;  
        case A_MP:
        case A_MAX_MP:
        {
            if(thisskill->duration<=0)
            {//i don't think there are a skill with this (heal mp)
                character->Stats->MP += (long int)thisskill->value1;
                if(character->Stats->MP > character->Stats->MaxMP)
                    character->Stats->MP = character->Stats->MaxMP;                  
                return true;
            }            
            CBValue BuffValue = GetBuffValue( thisskill, character, Evalue, i, 
                                            character->Status->MP_up, 
                                            character->Status->MP_down, 
                                            character->Stats->MaxMP, true );                      
            if(BuffValue.NewValue!=0)
            {       
                UINT j = BuffValue.Position;           
                character->Stats->MaxMP = BuffValue.NewValue;
                character->Status->MP_up = j;
                character->MagicStatus[j].Buff = thisskill->buff[i];
                character->MagicStatus[j].BuffTime = clock();
                character->MagicStatus[j].Duration = thisskill->duration;  
                character->MagicStatus[j].Value = BuffValue.Value;                  
                bflag = true;
            } 
        }
        break;                                                                      
    }    
    return bflag;       
}

// Add Buffs
bool CWorldServer::CheckDBuffs( CSkills* thisskill, CCharacter* character, int Evalue ,int i)
{ //Log( MSG_INFO, "checkdbuffs %i",( thisskill->buff[i] ));
    bool bflag = false;
    switch( thisskill->buff[i] )
    {
        case A_GMExtra_Damage:    
        case A_Extra_Damage:
        {
             CBValue BuffValue = GetBuffValue( thisskill, character, Evalue, i, 
                                            character->Status->ExtraDamage_up, 
                                            character->Status->ExtraDamage_down, 
                                            character->Stats->ExtraDamage, true );
             if(BuffValue.NewValue!=0)
            {                       
                UINT j = BuffValue.Position;   
                character->Stats->ExtraDamage = thisskill->atkpower;
                character->Status->ExtraDamage_up = j;
                character->MagicStatus[j].Buff = thisskill->buff[i];
                character->MagicStatus[j].BuffTime = clock();
                character->MagicStatus[j].Duration = thisskill->duration;  
                character->MagicStatus[j].Value = BuffValue.Value;                  
                bflag = true;
            }
        }
        break;    
        case A_HP:
        {
            bflag = true;
            character->Stats->HP -= (long int)thisskill->value1;
            if(character->Stats->HP < 0)
                character->Stats->HP = 0;            
        }
        break;
        case A_ATTACK:
        {
            CBValue BuffValue = GetBuffValue( thisskill, character, Evalue, i, 
                                            character->Status->Attack_up, 
                                            character->Status->Attack_down, 
                                            character->Stats->Attack_Power, false );                      
            if(BuffValue.NewValue!=0)
            {       
                UINT j = BuffValue.Position;           
                character->Stats->Attack_Power = BuffValue.NewValue;
                character->Status->Attack_down = j;
                character->MagicStatus[j].Buff = thisskill->buff[i];
                character->MagicStatus[j].BuffTime = clock();
                character->MagicStatus[j].Duration = thisskill->duration;  
                character->MagicStatus[j].Value = BuffValue.Value;                  
                bflag = true;
            }                                            
        }
        break;
        case A_DEFENSE:
        {
            CBValue BuffValue = GetBuffValue( thisskill, character, Evalue, i, 
                                            character->Status->Defense_up, 
                                            character->Status->Defense_down, 
                                            character->Stats->Defense, false );                      
            if(BuffValue.NewValue!=0)
            {       
                UINT j = BuffValue.Position;           
                character->Stats->Defense = BuffValue.NewValue;
                character->Status->Defense_down = j;
                character->MagicStatus[j].Buff = thisskill->buff[i];
                character->MagicStatus[j].BuffTime = clock();
                character->MagicStatus[j].Duration = thisskill->duration;  
                character->MagicStatus[j].Value = BuffValue.Value;                  
                bflag = true;
            } 
        }
        break;
        case A_ACCUR:
        {
            CBValue BuffValue = GetBuffValue( thisskill, character, Evalue, i, 
                                            character->Status->Accury_up, 
                                            character->Status->Accury_down, 
                                            character->Stats->Accury, false );                      
            if(BuffValue.NewValue!=0)
            {       
                UINT j = BuffValue.Position;           
                character->Stats->Accury = BuffValue.NewValue;
                character->Status->Accury_down = j;
                character->MagicStatus[j].Buff = thisskill->buff[i];
                character->MagicStatus[j].BuffTime = clock();
                character->MagicStatus[j].Duration = thisskill->duration;  
                character->MagicStatus[j].Value = BuffValue.Value;                  
                bflag = true;
            } 
        }
        break;
        case A_MRESIST:
        {
            CBValue BuffValue = GetBuffValue( thisskill, character, Evalue, i, 
                                            character->Status->Magic_Defense_up, 
                                            character->Status->Magic_Defense_down, 
                                            character->Stats->Magic_Defense, false );                      
            if(BuffValue.NewValue!=0)
            {       
                UINT j = BuffValue.Position;           
                character->Stats->Magic_Defense = BuffValue.NewValue;
                character->Status->Magic_Defense_down = j;
                character->MagicStatus[j].Buff = thisskill->buff[i];
                character->MagicStatus[j].BuffTime = clock();
                character->MagicStatus[j].Duration = thisskill->duration;  
                character->MagicStatus[j].Value = BuffValue.Value;                  
                bflag = true;
            } 
        }
        break;
        case A_DODGE:
        {
            CBValue BuffValue = GetBuffValue( thisskill, character, Evalue, i, 
                                            character->Status->Dodge_up, 
                                            character->Status->Dodge_down, 
                                            character->Stats->Dodge, false );                      
            if(BuffValue.NewValue!=0)
            {       
                UINT j = BuffValue.Position;           
                character->Stats->Dodge = BuffValue.NewValue;
                character->Status->Dodge_down = j;
                character->MagicStatus[j].Buff = thisskill->buff[i];
                character->MagicStatus[j].BuffTime = clock();
                character->MagicStatus[j].Duration = thisskill->duration;  
                character->MagicStatus[j].Value = BuffValue.Value;                  
                bflag = true;
            } 
        }
        break;    
        case A_DASH:
        {
            CBValue BuffValue = GetBuffValue( thisskill, character, Evalue, i, 
                                            character->Status->Dash_up, 
                                            character->Status->Dash_down, 
                                            character->Stats->Move_Speed, false );                      
            if(BuffValue.NewValue!=0)
            {       
                UINT j = BuffValue.Position;           
                character->Stats->Move_Speed = BuffValue.NewValue;
                character->Status->Dash_down = j;
                character->MagicStatus[j].Buff = thisskill->buff[i];
                character->MagicStatus[j].BuffTime = clock();
                character->MagicStatus[j].Duration = thisskill->duration;  
                character->MagicStatus[j].Value = BuffValue.Value;                  
                bflag = true;
            } 
        }
        break;  
        case A_HASTE:
        {
            CBValue BuffValue = GetBuffValue( thisskill, character, Evalue, i, 
                                            character->Status->Haste_up, 
                                            character->Status->Haste_down, 
                                            (UINT)character->Stats->Attack_Speed, false );                      
            if(BuffValue.NewValue!=0)
            {       
                UINT j = BuffValue.Position;           
                character->Stats->Attack_Speed = BuffValue.NewValue;
                character->Status->Haste_down = j;
                character->MagicStatus[j].Buff = thisskill->buff[i];
                character->MagicStatus[j].BuffTime = clock();
                character->MagicStatus[j].Duration = thisskill->duration;  
                character->MagicStatus[j].Value = BuffValue.Value;                  
                bflag = true;
            } 
        }
        break;  
        case A_CRITICAL:
        {
            CBValue BuffValue = GetBuffValue( thisskill, character, Evalue, i, 
                                            character->Status->Critical_up, 
                                            character->Status->Critical_down, 
                                            character->Stats->Critical, false );                      
            if(BuffValue.NewValue!=0)
            {       
                UINT j = BuffValue.Position;           
                character->Stats->Critical = BuffValue.NewValue;
                character->Status->Critical_down = j;
                character->MagicStatus[j].Buff = thisskill->buff[i];
                character->MagicStatus[j].BuffTime = clock();
                character->MagicStatus[j].Duration = thisskill->duration;  
                character->MagicStatus[j].Value = BuffValue.Value;                  
                bflag = true;
            } 
        }
        break;  
        case A_MAX_HP:
        {
            CBValue BuffValue = GetBuffValue( thisskill, character, Evalue, i, 
                                            character->Status->HP_up, 
                                            character->Status->HP_down, 
                                            character->Stats->MaxHP, false );                      
            if(BuffValue.NewValue!=0)
            {       
                UINT j = BuffValue.Position;           
                character->Stats->MaxHP = BuffValue.NewValue;
                character->Status->HP_down = j;
                character->MagicStatus[j].Buff = thisskill->buff[i];
                character->MagicStatus[j].BuffTime = clock();
                character->MagicStatus[j].Duration = thisskill->duration;  
                character->MagicStatus[j].Value = BuffValue.Value;                  
                bflag = true;
            } 
        }
        break;  
        case A_MAX_MP:
        {
            CBValue BuffValue = GetBuffValue( thisskill, character, Evalue, i, 
                                            character->Status->MP_up, 
                                            character->Status->MP_down, 
                                            character->Stats->MaxMP, false );                      
            if(BuffValue.NewValue!=0)
            {       
                UINT j = BuffValue.Position;           
                character->Stats->MaxMP = BuffValue.NewValue;
                character->Status->MP_down = j;
                character->MagicStatus[j].Buff = thisskill->buff[i];
                character->MagicStatus[j].BuffTime = clock();
                character->MagicStatus[j].Duration = thisskill->duration;  
                character->MagicStatus[j].Value = BuffValue.Value;                  
                bflag = true;
            } 
        }
        break;                                                                      
    }
    return bflag;   
}   


// Get the new buffs values (character)
CBValue CWorldServer::GetBuffValue( CSkills* thisskill, CCharacter* character, UINT Evalue, UINT i, UINT up, UINT down, UINT CurrentValue, bool Buff )
{
    CBValue NewValue;
    NewValue.Position = 0xff;
    NewValue.NewValue = 0;
    NewValue.Value = 0;
    UINT Value = 0;
    UINT NormalValue = 0;                 
    UINT UpValue = 0;
    UINT DownValue = 0;
    if(up!=0xff)
    {
        UpValue = character->MagicStatus[up].Value;
        NormalValue = CurrentValue - character->MagicStatus[up].Value;
        if(Buff)
            NewValue.Position = up;        
    }    
    else
    {
        if(Buff)
        {
            for(UINT z=0;z<15;z++)
            {
                if(character->MagicStatus[z].Buff==0)
                {
                    NewValue.Position = z;
                    break;
                }
            }
            if(NewValue.Position==0xff)
                return NewValue;
            NormalValue = CurrentValue;
        }
    }
    if(down!=0xff)
    {
        DownValue = character->MagicStatus[down].Value;
        if(NormalValue!=0)
            NormalValue += character->MagicStatus[down].Value;
        else
            NormalValue = CurrentValue + character->MagicStatus[up].Value;
        if(!Buff)
            NewValue.Position = down;              
    }   
    else
    {
        if(!Buff)
        {
            for(UINT z=15;z<30;z++)
            {
                if(character->MagicStatus[z].Buff==0)
                {
                    NewValue.Position = z;
                    break;
                }
            }
            if(NewValue.Position==0xff)
                return NewValue;
            NormalValue = CurrentValue;
        }        
    }
    if( thisskill->value2[i] > 0 )
    {
        Value = NormalValue * thisskill->value2[i] / 100;
    }                        
    if( thisskill->value1[i] > 0 )
    {        
        Value += thisskill->value1[i];
        if(thisskill->buff[i]==A_DASH || thisskill->buff[i]==MOV_SPEED) Value += (Evalue-15)/10;        
        else Value += (Evalue-15)/32;
    }    
    if(Buff)
    {
        if( Value+CurrentValue-DownValue>=CurrentValue )
        {
            CurrentValue = NormalValue - DownValue + Value;            
        }
        else
        {
            CurrentValue = 0;
        }
    }
    else
    {
        CurrentValue = NormalValue + UpValue - Value;
    }
    NewValue.NewValue = CurrentValue;
    NewValue.Value = Value;
    return NewValue;
}

// Build Buffs to Show
unsigned int CWorldServer::BuildBuffs( CCharacter* character )
{
    BYTE buff1 = 0;
    BYTE buff2 = 0;
    BYTE buff3 = 0;
    BYTE buff4 = 0;     
    //Build Debuffs and Buffs 
        //Up
    if(character->Status->Attack_up != 0xff)//A_ATTACK:
                buff2 += ATTACK_UP;
    if(character->Status->Defense_up != 0xff)//A_DEFENSE:
                buff2 += DEFENSE_UP;
    if(character->Status->Magic_Defense_up != 0xff)//A_MDEFENSE_UP
                buff2 += MDEFENSE_UP;
    if(character->Status->Accury_up != 0xff)//A_ACCUR:
                buff3 += HITRATE_UP;
    if(character->Status->Dash_up != 0xff)//A_DASH:   
                buff1 += DASH_UP;
    if(character->Status->Haste_up != 0xff)//A_HASTE:
                buff2 += HASTE_UP;                
    if(character->Status->HP_up != 0xff)//A_HP:
                buff1 += HP_UP;
    if(character->Status->MP_up != 0xff)//A_MP:
                buff1 += MP_UP;    
    if(character->Status->Critical_up != 0xff)//A_CRITICAL:
                buff3 += CRITICAL_UP;
    if(character->Status->Dodge_up != 0xff)//A_DODGE:
                buff1 += DODGE_UP;
    if(character->Status->ExtraDamage_up != 0xff)//A_Extra_Damage:
                buff4 += DAMAGE_UP;
        //Down
    if(character->Status->Attack_down != 0xff) // A_ATTACK:
                buff2 += ATTACK_DOWN;
    if(character->Status->Defense_down != 0xff)//A_DEFENSE:
                buff2 += DEFENSE_DOWN;
    if(character->Status->Magic_Defense_down != 0xff)
                buff2 += MDEFENSE_DOWN;
    if(character->Status->Accury_down != 0xff)//A_ACCUR:
                buff3 += HITRATE_DOWN;
    if(character->Status->Dash_down != 0xff)//A_DASH:   
                buff1 += DASH_DOWN;
    if(character->Status->Haste_down != 0xff)//A_HASTE:
                buff2 += HASTE_DOWN;                
    if(character->Status->HP_down != 0xff)//A_HP:
                buff1 += 0;
    if(character->Status->MP_down != 0xff)//A_MP:
                buff1 += 0; 
    if(character->Status->Critical_down != 0xff)//A_CRITICAL:
                buff3 += CRITICAL_DOWN;                   
    if(character->IsSummon( ))
                buff4 += SUMMON;
    return (buff1 * 0x01) + (buff2 * 0x100 ) + (buff3 * 0x10000) + (buff4 * 0x1000000);
}
