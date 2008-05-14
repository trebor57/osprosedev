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
TeleportPlayer
    developed with Main erose/hrose source server + some change from the original eich source        
*/
#include "player.h"
#include "worldserver.h"
//532920592 raise clan grade to two
//1120605254 get quest for grade 3 -> 1120446495 upgrade to clan grade 3
//learn Clan House 533397332

/*bool CWorldServer::ProcessTrigger( CPlayer* thisclient, CNPCTrigger* thisaction )
{
	CPacket pakout;
	unsigned short curaction = 0;
	unsigned short curquest = 0;
	for( unsigned char i = 0; i < thisaction->conditioncount; i++ )
	{//Log( MSG_INFO, "switch 1 thisaction->actions[curaction][0] %d", thisaction->actions[curaction][0] );
		switch( thisaction->actions[curaction][0] )
		{
			case 0x00:
			{
				if( thisclient->CheckQuestFlag( *((unsigned short*)&thisaction->actions[curaction][1]) ) != (thisaction->actions[curaction][3]==1)?true:false )
					return false;
				break;
			}
			case 0x01:
			{
				// Check if we are close enough to the target npc...
				// Fuck you :) for now...
				break;
			}
			case 0x02:
			{
				curquest = *((unsigned short*)&thisaction->actions[curaction][1]);
				if( thisclient->CheckQuest( *((unsigned short*)&thisaction->actions[curaction][1]) ) != (thisaction->actions[curaction][3]==1)?true:false )
					return false;
				break;
			}
			case 0x03:
			{
				unsigned char updatecount = thisaction->actions[curaction][1];
				for( unsigned char k = 0; k < updatecount; k++ ) {
					unsigned short mycount = thisclient->CountQuestItem( curquest, *((unsigned short*)&thisaction->actions[curaction][2+(k*5)]) );
					switch( thisaction->actions[curaction][4+(k*5)] )
					{
						/* ==  case 0: { if( mycount != *((unsigned short*)&thisaction->actions[curaction][5+(k*5)]) ) return false; break; }
						/* !=  case 1: { if( mycount == *((unsigned short*)&thisaction->actions[curaction][5+(k*5)]) ) return false; break; }
						/* >=  case 2: { if( mycount < *((unsigned short*)&thisaction->actions[curaction][5+(k*5)]) ) return false; break; }
						/* <   case 3: { if( mycount >= *((unsigned short*)&thisaction->actions[curaction][5+(k*5)]) ) return false; break; }
						/* <=  case 4: { if( mycount > *((unsigned short*)&thisaction->actions[curaction][5+(k*5)]) ) return false; break;}
						/* >   case 5: { if( mycount <= *((unsigned short*)&thisaction->actions[curaction][5+(k*5)]) ) return false; break;}
					}
				}

				break;
			}
			case 0xFF:
			{
				// UNKNOWN
				break;
			}
		}	
		curaction++;
	}
	for( unsigned char i = 0; i < thisaction->rewardcount; i++ )
	{//Log( MSG_INFO, "switch 2 thisaction->actions[curaction][0] %d", thisaction->actions[curaction][0] );
		switch( thisaction->actions[curaction][0] )
		{
			case 0x80://128
			{
				thisclient->SetQuestFlag( *((unsigned short*)&thisaction->actions[curaction][1]), (thisaction->actions[curaction][3]==1)?true:false );
				break;
			}
			case 0x81://129
			{
				pakGiveItem( thisclient, thisaction->actions[curaction][1], *((unsigned short*)&thisaction->actions[curaction][2]), *((unsigned short*)&thisaction->actions[curaction][4]) );
				break;
			}
			case 0x82://130
			{
				pakGiveExp( thisclient, *((unsigned*)&thisaction->actions[curaction][1]) );
				break;
			}
			case 0x83://131
			{
				unsigned char updatecount = thisaction->actions[curaction][1];
				for( unsigned char k = 0; k < updatecount; k++ ) {
					unsigned char statid = thisaction->actions[curaction][2+(k*6)];
					unsigned char action = thisaction->actions[curaction][3+(k*6)];
					unsigned myval = *((unsigned*)&thisaction->actions[curaction][4+(k*6)]);

					switch( action )
					{
						case 5:
						{
                             
							pakout.StartPacket( 0x721 );
							pakout.AddWord( statid );
							pakout.AddDWord( myval );
							thisclient->client->SendPacket( &pakout );
							if (statid==4)
                            {
                                thisclient->CharInfo->Job=myval;//change job
                                //switch(myval)
                                //case 0xd3: //muse
                                   // {
                                     //   RESETPACKET( pak, 0x730);
                                     //   ADDBYTE(pak, 0x05);
                                     //   ADDBYTE(pak, 0x00);    
                                     //   ADDWORD(pak, 0xdc3b);
                                     //   ADDWORD(pak, 0x20cd);
                                    //}
                            }
                            else
                            {
                             Log(MSG_INFO, "player statid:%i sett myval:%i", statid, myval );
                            }
							break;
						}
						case 6:
						{
                             
							//thisclient->stats_base[statid]+=myval;
							switch(statid)
							{
                                  case 40://zully
                                       thisclient->CharInfo->Zulies+=myval;
                                       break;
                                  default:
                                       Log(MSG_INFO, "player statid:%i add myval:%i", statid, myval );
                                       break;
                            }
							pakout.StartPacket( 0x720 );
							pakout.AddWord( statid );
							pakout.AddDWord( myval );
							thisclient->client->SendPacket( &pakout );
							break;
						}
						case 7:
						{
                             

							switch(statid)
							{
                                  case 40://zully
                                       thisclient->CharInfo->Zulies-=myval;
                                       break;
                                  default:
                                       Log(MSG_INFO, "player statid:%i remove myval:%i", statid, myval );
                                       break;
                            }
							pakout.StartPacket( 0x720 );
							pakout.AddWord( statid );
							pakout.AddDWord( -((int)myval) );
							thisclient->client->SendPacket( &pakout );
							break;
						}
						default:
                        {
                            Log(MSG_INFO, "unknown code 131 action");
                            Log(MSG_INFO, "player statid:%i myval:%i", statid, myval );
                            break;
                        }
					}
				}
				break;
			}
			case 0x84://132
			{
				if( thisaction->actions[curaction][1] == 0 ) {
					thisclient->EndQuest( *((unsigned short*)&thisaction->actions[curaction][2]) );
				}else if( thisaction->actions[curaction][1] == 1 ) {
					thisclient->StartQuest( *((unsigned short*)&thisaction->actions[curaction][2]) );
				}else if( thisaction->actions[curaction][1] == 2 || thisaction->actions[curaction][1] == 3 ) {
					thisclient->ChangeQuest( curquest, *((unsigned short*)&thisaction->actions[curaction][2]) );
				}else if( thisaction->actions[curaction][1] == 4 ) {
					curquest = *((unsigned short*)&thisaction->actions[curaction][2]);
				}
				break;
			}
			case 0x85://133 increase de crease questitem amount
			{
				if( thisaction->actions[curaction][1] == 1 ) {
					thisclient->UpdateQuestItem( curquest, *((unsigned short*)&thisaction->actions[curaction][2]), *((unsigned short*)&thisaction->actions[curaction][4]) );
				}else if( thisaction->actions[curaction][1] == 0 ) {
					thisclient->UpdateQuestItem( curquest, *((unsigned short*)&thisaction->actions[curaction][2]), - *((unsigned short*)&thisaction->actions[curaction][4]) );
				}
				break;
			}
			case 0x86://134
			{
                fPoint dest;
                dest.x = (float)*((unsigned*)&thisaction->actions[curaction][3]);
                dest.y = (float)*((unsigned*)&thisaction->actions[curaction][7]);
				pakTeleport( thisclient, *((unsigned short*)&thisaction->actions[curaction][1]), (float)*((unsigned*)&thisaction->actions[curaction][3]), (float)*((unsigned*)&thisaction->actions[curaction][7]) );
				break;
			}
			case 0x87://135
			{
				LearnSkill( thisclient, *((unsigned short*)&thisaction->actions[curaction][1]) );
				break;
			}
			case 0x88://136 execute trigger
			{
				std::map<unsigned,CNPCTrigger*>::iterator myit = NPCActionList.find( *((unsigned*)&thisaction->actions[curaction][1]) );
				if( myit == NPCActionList.end( ) ) continue;
				ProcessTrigger( thisclient, myit->second );
				break;
			}
			case 0x89://137
			{
				pakGiveMoney( thisclient, *((unsigned*)&thisaction->actions[curaction][1]) );
				break;
			}
			case 0x8a://138 //spawn mob
			{
                 fPoint position = GServer->RandInCircle( thisclient->Position->current, 5 );
                 CMap* map = GServer->MapList.Index[thisclient->Position->Map];
                 UINT mob = *((unsigned short*)&thisaction->actions[curaction][1]);
                 CMonster* newmonster=map->AddMonster( mob, position, 0, NULL, NULL, 0, true );
                 break;
            }
            case 0x8b://139 //135
			{
                 if(thisclient->Stats->Level>15)break;
                 thisclient->Attr->Str=15;
                 thisclient->Attr->Dex=15;
                 thisclient->Attr->Int=15;
                 thisclient->Attr->Con=15;
                 thisclient->Attr->Cha=10;
                 thisclient->Attr->Sen=10;
                 switch (thisclient->Stats->Level)
                 {
                     case 2:thisclient->CharInfo->StatPoints=11;break;
                     case 3:thisclient->CharInfo->StatPoints=23;break;
                     case 4:thisclient->CharInfo->StatPoints=36;break;
                     case 5:thisclient->CharInfo->StatPoints=50;break;
                     case 6:thisclient->CharInfo->StatPoints=64;break;
                     case 7:thisclient->CharInfo->StatPoints=79;break;
                     case 8:thisclient->CharInfo->StatPoints=95;break;
                     case 9:thisclient->CharInfo->StatPoints=112;break;
                     case 10:thisclient->CharInfo->StatPoints=130;break;
                     case 11:thisclient->CharInfo->StatPoints=148;break;
                     case 12:thisclient->CharInfo->StatPoints=167;break;
                     case 13:thisclient->CharInfo->StatPoints=187;break;
                     case 14:thisclient->CharInfo->StatPoints=208;break;
                     case 15:thisclient->CharInfo->StatPoints=230;break;
                     default: thisclient->CharInfo->StatPoints=0;break;
                 }
				break;
			}
			case 0x90://144 //clanfield teleport
			{
                if (thisclient->Clan->clanid ==0)break;
                fPoint dest;
                dest.x = (float)*((unsigned*)&thisaction->actions[curaction][3]);
                dest.y = (float)*((unsigned*)&thisaction->actions[curaction][7]);
                if (thisclient->Party->party>0){
                CParty* party = thisclient->Party->party;
                if(party->Members.size()>1)
                {
                    for(UINT i=0;i<party->Members.size();i++)
                    {
                        CPlayer* othermember = party->Members.at(i);
                        if (thisclient->Clan->clanid == othermember->Clan->clanid)
                           pakTeleport( othermember, *((unsigned short*)&thisaction->actions[curaction][1]), dest.x, dest.y );
                    }
                }}
                else
				pakTeleport( thisclient, *((unsigned short*)&thisaction->actions[curaction][1]), dest.x, dest.y );
                //pakTeleport( thisclient, *((unsigned short*)&thisaction->actions[curaction][1]), (float)*((unsigned*)&thisaction->actions[curaction][3]), (float)*((unsigned*)&thisaction->actions[curaction][7]) );
				break;
			}
			case 0xFF:
			{
				// UNKNOWN
				break;
			}
		}
		curaction++;
	}
	return true;
}

bool CWorldServer::pakEventTrigger( CPlayer* thisclient, unsigned triggerid )
{
	CPacket pakout;
	bool retval = false;

	std::map<unsigned,CNPCTrigger*>::iterator myit = NPCActionList.find( triggerid );
	if( myit == NPCActionList.end( ) ) return false;
	CNPCTrigger* thisaction = myit->second;

	retval = ProcessTrigger( thisclient, thisaction );
	if( !retval ) {
		for( unsigned char i = 0; i < thisaction->linkcount; i++ ) {
			std::map<unsigned,CNPCTrigger*>::iterator myit2 = NPCActionList.find( thisaction->links[i] );
			if( myit2 == NPCActionList.end( ) ) break;
			retval = ProcessTrigger( thisclient, myit2->second );
			if( retval ) break;
		}
	}

	if( retval ) {
		pakout.StartPacket( 0x730 );
		pakout.AddWord( 5 );
		pakout.AddDWord( thisaction->hash );
		thisclient->client->SendPacket( &pakout );
	}
	else{
         Log( MSG_INFO, "quest %i might be broken",triggerid);
         pakout.StartPacket( 0x730 );
         pakout.AddWord( 5 );
         pakout.AddDWord( triggerid );
         thisclient->client->SendPacket( &pakout );
         }
	return true;
}

void CPlayer::SetQuestFlag( unsigned flagid, bool value )
{
	unsigned byteid = flagid / 8;
	unsigned bitid = flagid % 8;

	questflags[byteid] = questflags[byteid] | ( (value?1:0) << bitid );
}

bool CPlayer::CheckQuestFlag( unsigned flagid )
{
	unsigned byteid = flagid / 8;
	unsigned bitid = flagid % 8;

	return ( ( questflags[byteid] >> bitid ) & 1 );
}

bool CPlayer::CheckQuest( unsigned short questid )
{
	for( unsigned char i = 0; i < 10; i++ ){
		if( quest.quests[i].QuestID == questid ) return true;
	}
	return false;
}

bool CPlayer::StartQuest( unsigned short questid )
{
	for( unsigned char i = 0; i < 10; i++ ){
		if( quest.quests[i].QuestID == questid ) return false;
	}

	for( unsigned char i = 0; i < 10; i++ )
	{
		if( quest.quests[i].QuestID == 0 )
		{
			quest.quests[i].QuestID = questid;
			quest.quests[i].Time = (unsigned)time(NULL) / 10;
			return true;
		}
	}
}

void CPlayer::ChangeQuest( unsigned short questid, unsigned newquestid )
{
	for( unsigned char i = 0; i < 10; i++ )
	{
		if( quest.quests[i].QuestID == questid )
		{
			quest.quests[i].QuestID = newquestid;
			quest.quests[i].Time = (unsigned)time(NULL) / 10;
			return;
		}
	}
}*/

bool CPlayer::EndQuest( unsigned short questid )
{
	for( unsigned char i = 0; i < 10; i++ )
	{
		if( quest.quests[i].QuestID == questid )
		{
			quest.quests[i].QuestID = 0;
			quest.quests[i].Time = 0;
			for( unsigned j = 0; j < 5; j++ )
				quest.quests[i].Items[j].Clear( );
			return true;
		}
	}
}

/*void CPlayer::UpdateQuestItem( unsigned short questid, unsigned itemid, int count )
{
	for( unsigned char i = 0; i < 10; i++ )
	{
		if( quest.quests[i].QuestID == questid )
		{
			for( unsigned char j = 0; j < 5; j++ )
			{
                 if (count<0 && quest.quests[i].Items[j].count>0&&quest.quests[i].Items[j].itemnum == itemid )
                 {
                     quest.quests[i].Items[j].count += count;
					break;
                }
                else if (count<0 && quest.quests[i].Items[j].count>0&& quest.quests[i].Items[j].itemnum == (itemid+1) )
                 {
                     quest.quests[i].Items[j].count += count;
					break;
                }
                else if (count<0 && quest.quests[i].Items[j].count>0&& quest.quests[i].Items[j].itemnum == (itemid+2) )
                 {
                     quest.quests[i].Items[j].count += count;
					break;
                }
                else
				if( count>0&& ( quest.quests[i].Items[j].count <= 0 || quest.quests[i].Items[j].itemnum == itemid )) {
					quest.quests[i].Items[j].itemnum = itemid;
					quest.quests[i].Items[j].itemtype = 13;
					quest.quests[i].Items[j].count += count;
					if( quest.quests[i].Items[j].count <= 0 )
						quest.quests[i].Items[j].Clear( );
					break;
				}
			}
			break;
		}
	}
}

unsigned short CPlayer::CountQuestItem( unsigned short questid, unsigned itemid )
{
	unsigned short mycount = 0;

	for( unsigned char i = 0; i < 10; i++ )
	{
		if( quest.quests[i].QuestID == questid )
		{
			for( unsigned char j = 0; j < 5; j++ )
			{
				if( quest.quests[i].Items[j].itemnum == itemid )
					mycount += quest.quests[i].Items[j].count;
			}
		}
	}
	
	return mycount;
}

unsigned char CPlayer::AddInvItem( CItem* thisitem )
{
	unsigned short maxval = 0;
	char itemtypes[15] = {4,0,0,0,0,0,0,0,0,0,1,2,2,4,3};
	unsigned char itemtab = itemtypes[ thisitem->itemtype ];
	if( itemtab == 4 ) return 0xff;
	if( thisitem->count > 999 ) return 0xff;

	if( itemtab == 1 || itemtab == 2 ) {
		// STACKABLE
		for( unsigned char i=12+(itemtab*30); i<42+(itemtab*30); i++ ) {
			if( items[i].itemnum == thisitem->itemnum && items[i].itemtype == thisitem->itemtype && items[i].count<999 ) {
				if( items[i].count + thisitem->count > 999 ) break;
				items[i].count += thisitem->count;
				thisitem->count = 0;
				return i;
			}
		}
	}
	for( unsigned char i=12+(itemtab*30); i<42+(itemtab*30); i++ ) {
		if( items[i].count == 0 ) {
            items[i].gem=0;
            items[i].refine=0;
			items[i] = *thisitem;
			thisitem->count = 0;
			
			return i;
		}
	}

	return 0xff;
}

// Search Quest by QuestID
/*QUESTS* CPlayer::GetQuestByQuestID( unsigned long int questid )
{
    for(int i=0;i<MyQuest.size();i++)
    {
        QUESTS* myquest = MyQuest.at( i );
        if( myquest->questid == questid )
            return myquest;
    }
    return 0;
}*/

// Get MyQuest by ItemID
/*QUESTS* CPlayer::GetQuestByItemID( unsigned long int itemid )
{    
	for(int j=0; j<MyQuest.size( ); j++) 
    {
        QUESTS* myquest = MyQuest.at( j );
		for(int i=0;i<5;i++)
		{   // fixed by Drakia [If a quest had multiple monsters, but only one counter (Like the first hunting quest, kill 3 Jelly Bean's, or Mini-Jelly Bean's) you would need to kill 3 of the first creature in the list to finish the quest]
            if( itemid == myquest->thisquest->itemid[i] && myquest->active )
                return myquest;
        }
	}
	return 0;
}

// Get MyQuest by ItemID
QUESTS* CPlayer::GetQuestByMob( unsigned int mob )
{    
	for(int j=0; j<MyQuest.size( ); j++) 
    {
        QUESTS* myquest = MyQuest.at( j );
		for(int i=0;i<5;i++)
		{
            if( mob == myquest->thisquest->mobs[i] )
                return myquest;
        }
	}
	return 0;
}*/
