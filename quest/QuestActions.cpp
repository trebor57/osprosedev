// Added By Raven0123
#include "../WorldServer.h"

dword CPlayerQuests::GetRewardValue(dword function, dword amount, CPlayer* client, word nDupCNT){
	switch(function){
		case 0:
		{
			dword tempVal = amount + 0x1E;
			tempVal *= client->Attr->Cha + 0x0a;
			tempVal *= (100 & 0xFFFF);//World Rate
			tempVal *= 0x14;//Fame + 0x14
			tempVal = ((tempVal / (client->Stats->Level + 0x46)) / 0x7530) + amount;
			return tempVal;
		}
		break;
		case 1:
		{
			dword tempVal = (client->Stats->Level + 3) * amount;
			tempVal *= (client->Attr->Cha >> 1) + client->Stats->Level + 0x28;
			tempVal *= (100 & 0xFFFF);//World Rate
			return tempVal / 0x2710;
		}
		break;
		case 2:
			return (amount * nDupCNT);
		break;
		case 3:
		case 5:
		{
			dword tempVal = amount + 0x14;
			tempVal *= client->Attr->Cha + 0x0a;
			tempVal *= (100 & 0xFFFF);//World Rate
			tempVal *= 0x14;//Fame + 0x14
			tempVal = ((tempVal / (client->Stats->Level + 0x46)) / 0x7530) + amount;
			return tempVal;	
		}
		break;
		case 4:
		{
			dword tempVal = amount + 0x2;
			tempVal *= (client->Attr->Cha + client->Stats->Level + 0x28);
			tempVal *= 0x28;//Fame + 0x28
			tempVal *= (100 & 0xFFFF);//World Rate
			return tempVal / 0x222E0;
		}
		break;
		case 6:
		{
			dword tempVal = amount + 0x14;
			tempVal *= client->Attr->Cha + client->Stats->Level;
			tempVal *= 0x14;//Fame + 0x14
			tempVal *= (100 & 0xFFFF);//World Rate
			return (tempVal / 0x2DC6C0) + amount;
		}
		break;
	}
	return amount;
}

//Update Quest
QUESTREWD(000){
	GETREWDDATA(000);
	#ifdef debugging
	Log( MSG_DEBUG, "a000 btOp: %i", data->btOp );
	#endif
	switch(data->btOp){//0 remove, 1 start, 2 replace quest keep items, 3 replace quest delete items, 4 select
		case 0:
		{bool removed = false;
			for(dword i = 0; i < 10; i++){
				if(client->quest.quests[i].QuestID != data->iQuestSN) continue;
				#ifdef debugging
                Log(MSG_DEBUG, "a000 remove Quest: '%i'",data->iQuestSN);
				#endif
				memset(&client->quest.quests[i], 0, sizeof(SQuest));
				removed = true;
				break;
			}
			if(removed==false)
			{
    			for(dword i = 0; i < 10; i++){
    				if(client->quest.quests[i].QuestID != client->ActiveQuest) continue;
    				#ifdef debugging
                    Log(MSG_DEBUG, "a000 remove Quest: '%i'",data->iQuestSN);
    				#endif
    				memset(&client->quest.quests[i], 0, sizeof(SQuest));
    				removed = true;
    				break;
    			}
            }
		}
		break;
		case 1:
		{
             #ifdef debugging
             Log(MSG_DEBUG, "a000 start Quest: '%i'",data->iQuestSN);
             #endif
			for(dword i = 0; i < 10; i++){
				if(client->quest.quests[i].QuestID != 0) continue;
				memset(&client->quest.quests[i], 0, sizeof(SQuest));
				client->quest.quests[i].QuestID = data->iQuestSN;
				client->quest.quests[i].Time = clock_t();
				break;
			}
		}
		break;
		case 2:
		{
             #ifdef debugging
             Log(MSG_DEBUG, "a000 2 replace Quest: '%i' with '%i'",client->ActiveQuest,data->iQuestSN);
             #endif
			for(dword i = 0; i < 10; i++){
                #ifdef debugging
                Log(MSG_DEBUG, "a000 2 compare Quest: '%i' with '%i'",client->ActiveQuest,client->quest.quests[i].QuestID);
                #endif
				if(client->quest.quests[i].QuestID != client->ActiveQuest) continue;
				client->quest.quests[i].QuestID = data->iQuestSN;
				client->quest.quests[i].Time = clock_t();
				break;
            }
		}
		break;
		case 3:
		{
             #ifdef debugging
             Log(MSG_DEBUG, "a000 3 replace Quest: '%i' with '%i'",client->ActiveQuest,data->iQuestSN);
             #endif
			for(dword i = 0; i < 10; i++){
                #ifdef debugging
                Log(MSG_DEBUG, "a000 3 compare Quest: '%i' with '%i'",client->ActiveQuest,client->quest.quests[i].QuestID);
                #endif
				if(client->quest.quests[i].QuestID != client->ActiveQuest) continue;
				memset(&client->quest.quests[i], 0, sizeof(SQuest));
				client->quest.quests[i].QuestID = data->iQuestSN;
				client->quest.quests[i].Time = clock_t();
				break;
            }
		}
		break;
		case 4:
		{
             #ifdef debugging
             Log(MSG_DEBUG, "a000 4 select Quest: '%i'",data->iQuestSN);
             #endif
			client->ActiveQuest = data->iQuestSN;
		}
		break;
		default:
             #ifdef debugging
             Log(MSG_DEBUG, "a000 update unknown action: %i",data->btOp);
             #endif
             return QUEST_FAILURE;
        break;
	}
	return QUEST_SUCCESS;
}

//Update Quest Items
QUESTREWD(001){
    #ifdef debugging
    Log(MSG_DEBUG, "a001");
    #endif
    GETREWDDATA(001);
    for(dword i = 0; i < 10; i++)
    {
		if(client->quest.quests[i].QuestID == client->ActiveQuest)
		{
            for( unsigned j = 0; j < 5; j++ )
            {
                 if (client->quest.quests[i].Items[j].itemnum==data->uiItemSN)
                 {
                     if (data->btOp==1)
                     {
                         client->quest.quests[i].Items[j].count+=data->nDupCNT;
                         #ifdef debugging
                         Log(MSG_DEBUG, "quest item count: '%i'",client->quest.quests[i].Items[j].count);
                         #endif
                         return QUEST_SUCCESS;
                     }
                     else
                     {
                         client->quest.quests[i].Items[j].count-=data->nDupCNT;
                         if (client->quest.quests[i].Items[j].count==0)
                         {
                            client->quest.quests[i].Items[j].itemnum=0;
                         }
                         
                         return QUEST_SUCCESS;
                     }
                 }
            }
            for( unsigned j = 0; j < 5; j++ )
            {
                 #ifdef debugging
                 Log(MSG_DEBUG, "check item slot: %i '%i'",j ,client->quest.quests[i].Items[j].itemnum);
                 #endif
                 if (client->quest.quests[i].Items[j].itemnum==0)
                 {
                    if (data->btOp==1)
                     {
                         #ifdef debugging
                         Log(MSG_DEBUG, "itemnum: '%i' %i",data->uiItemSN, data->nDupCNT);
                         #endif
                         client->quest.quests[i].Items[j].itemnum=data->uiItemSN;
                         client->quest.quests[i].Items[j].count+=data->nDupCNT;
                         return QUEST_SUCCESS;
                     }
                      else
                     {
                         #ifdef debugging
                         Log(MSG_DEBUG, "quest item not found");
                         #endif
                         return QUEST_FAILURE;
                     }
                 }
             }
        }
    }
    #ifdef debugging
    Log(MSG_DEBUG, "quest item not found");
    #endif
	return QUEST_FAILURE;
}

//Set Quest Variable
QUESTREWD(002){
    #ifdef debugging
    Log(MSG_DEBUG, "a002");
    #endif
    CPlayerQuests *pquests;
    GETREWDDATA(002);
    for(int i = 0; i < data->iDataCnt; i++){
		qword address = i * 0x0C;
		address += (qword)data;
		address += 4;
		STR_QUEST_DATA* variable = (STR_QUEST_DATA*)address;
		pquests->OperateValues(variable->btOp, &client->quest.quests[client->ActiveQuestnumber].Variables[variable->m_wVarNO], variable->nValue);
		#ifdef debugging
       // Log(MSG_DEBUG, "iType      : %i",thisflag->iType);
        Log(MSG_DEBUG, "m_wVarNO   : %i",variable->m_wVarNO);
        Log(MSG_DEBUG, "m_wVarNO2  : %i",variable->m_wVarNO2);
        Log(MSG_DEBUG, "m_wVarTYPE : %i",variable->m_wVarTYPE);
        Log(MSG_DEBUG, "m_wVarTYPE2: %i",variable->m_wVarTYPE2);
        Log(MSG_DEBUG, "nValue     : %i",variable->nValue);
        Log(MSG_DEBUG, "btOp       : %i",variable->btOp);
        #endif
    }
	return QUEST_SUCCESS;
}

//Update Stats
QUESTREWD(003){
    #ifdef debugging
    Log(MSG_DEBUG, "a003");
    #endif
	GETREWDDATA(003);
	CPlayerQuests *pquests;
	CPacket pakout;
	for(dword i = 0; i < data->iDataCnt; i++){
		qword address = i * 0x0C;
		address += (qword)data;
		address += 4;
		STR_ABIL_DATA* curAbil = (STR_ABIL_DATA*)address;
		switch( curAbil->iType )
		{
		case sGender:
			if(!pquests->OperateValues(curAbil->btOp, (int*)&client->CharInfo->Sex, curAbil->iValue))
				return QUEST_FAILURE;
			break;

		case sJob:
			if(!pquests->OperateValues(curAbil->btOp, (int*)&client->CharInfo->Job, curAbil->iValue))
				return QUEST_FAILURE;
			pakout.StartPacket( 0x721 );
			pakout.AddWord( curAbil->iType );
			pakout.AddDWord( curAbil->iValue );
			client->client->SendPacket( &pakout );
			break;

		case sUnion:
			if(!pquests->OperateValues(curAbil->btOp, (int*)&client->Stats[sUnion], curAbil->iValue))
				return QUEST_FAILURE;
			break;

		case sStrength:
			if(!pquests->OperateValues(curAbil->btOp, (int*)&client->Stats[sStrength], curAbil->iValue))
				return QUEST_FAILURE;
			break;

		case sDexterity:
			if(!pquests->OperateValues(curAbil->btOp, (int*)&client->Stats[sDexterity], curAbil->iValue))
				return QUEST_FAILURE;
			break;

		case sIntelligence:
			if(!pquests->OperateValues(curAbil->btOp, (int*)&client->Stats[sIntelligence], curAbil->iValue))
				return QUEST_FAILURE;
			break;

		case sConcentration:
			if(!pquests->OperateValues(curAbil->btOp, (int*)&client->Stats[sConcentration], curAbil->iValue))
				return QUEST_FAILURE;
			break;

		case sCharm:
			if(!pquests->OperateValues(curAbil->btOp, (int*)&client->Stats[sCharm], curAbil->iValue))
				return QUEST_FAILURE;
			break;

		case sSensibility:
			if(!pquests->OperateValues(curAbil->btOp, (int*)&client->Stats[sSensibility], curAbil->iValue))
				return QUEST_FAILURE;
			break;

		case sLevel:
			if(!pquests->OperateValues(curAbil->btOp, (int*)&client->Stats[sLevel], curAbil->iValue))
				return QUEST_FAILURE;
			break;

		case sStatPoints:
			if(!pquests->OperateValues(curAbil->btOp, (int*)&client->Stats[sStatPoints], curAbil->iValue))
				return QUEST_FAILURE;
			break;
		case sMoney:
			if(!pquests->OperateValues(curAbil->btOp, (int*)&client->Stats[sMoney], curAbil->iValue))
				return QUEST_FAILURE;
			break;
		default:
			if( client->questdebug )Log(MSG_WARNING, "Type Unknown: '%i'", curAbil->iType);
			break;
		}
	}
	return QUEST_SUCCESS;
}

//Set Quest Variable
QUESTREWD(004){
    #ifdef debugging
    Log(MSG_DEBUG, "a004");
    #endif
    CPlayerQuests *pquests;
    GETREWDDATA(004);
    for(int i = 0; i < data->iDataCnt; i++){
		qword address = i * 0x0C;
		address += (qword)data;
		address += 4;
		STR_QUEST_DATA* variable = (STR_QUEST_DATA*)address;
		switch(variable->m_wVarTYPE2)
		{
            case 3:
                 pquests->OperateValues(variable->btOp, &client->quest.variable[0], variable->nValue);
                 break;
            case 4:
                 pquests->OperateValues(variable->btOp, &client->quest.variable[10+variable->m_wVarNO], variable->nValue);
                 break;
            default:
                pquests->OperateValues(variable->btOp, &client->quest.variable[variable->m_wVarNO2], variable->nValue);
                Log(MSG_DEBUG, "unknown variable location   : %i",variable->m_wVarTYPE2);
                break;
        }
		#ifdef debugging
       // Log(MSG_DEBUG, "iType      : %i",thisflag->iType);
        Log(MSG_DEBUG, "m_wVarNO   : %i",variable->m_wVarNO);
        Log(MSG_DEBUG, "m_wVarNO2  : %i",variable->m_wVarNO2);
        Log(MSG_DEBUG, "m_wVarTYPE : %i",variable->m_wVarTYPE);
        Log(MSG_DEBUG, "m_wVarTYPE2: %i",variable->m_wVarTYPE2);
        Log(MSG_DEBUG, "nValue     : %i",variable->nValue);
        Log(MSG_DEBUG, "btOp       : %i",variable->btOp);
        #endif
    }
	return QUEST_SUCCESS;
}

//Give Reward
QUESTREWD(005){
    #ifdef debugging
    Log(MSG_DEBUG, "a005");
    #endif
	GETREWDDATA(005);
	CPlayerQuests *pquests;
	switch(data->btTarget){
		case 0://EXP
		{
			client->CharInfo->Exp += pquests->GetRewardValue(data->btEquation, data->iValue, client, 0);
			if(!client->CheckPlayerLevelUP( ))
			{
    			BEGINPACKET(pak,0x79b);
    			ADDDWORD(pak,client->CharInfo->Exp);
    			ADDDWORD(pak,client->CharInfo->stamina);
    			ADDWORD(pak,0);
    			client->client->SendPacket(&pak);
              }
		}
		break;
		case 1://Zuly
		{
			client->CharInfo->Zulies += pquests->GetRewardValue(data->btEquation, data->iValue, client, 1);//dunno nDupCount for this one!
			BEGINPACKET(pak,0x71D);
			ADDQWORD(pak,client->CharInfo->Zulies);
			client->client->SendPacket(&pak);
		}
		break;
		case 2://Item
		{
			CItem nItem;
			nItem.itemtype = data->iItemSN / 1000;
			nItem.itemnum = data->iItemSN - (nItem.itemtype * 1000);
			if(nItem.itemtype == 1 || nItem.itemtype == 2 ){
				nItem.count = pquests->GetRewardValue(data->btEquation, data->iValue, client, 0);
				if( nItem.count > 1 ){
				    nItem.count = 1;}
			    nItem.gem = 0;
				nItem.stats = 0;
				nItem.refine = 0;
				nItem.socketed = 0;
				nItem.durability = GServer->STB_ITEM[nItem.itemtype-1].rows[nItem.itemnum][29];//40;
				nItem.lifespan = 100;
				nItem.appraised = 1;
			}else{
			    nItem.count = pquests->GetRewardValue(data->btEquation, data->iValue, client, 0);
			    nItem.gem = 0;
				nItem.stats = 0;
				nItem.refine = 0;
				nItem.socketed = 0;
				nItem.durability = GServer->STB_ITEM[nItem.itemtype-1].rows[nItem.itemnum][29];//40;
				nItem.lifespan = 100;
				nItem.appraised = 1;
			}
			unsigned int thisslot = client->AddItem( nItem );
    		if( thisslot == 0xffff ) break;
    		client->items[thisslot]=nItem;
			BEGINPACKET(pak, 0x71f );
        	ADDBYTE(pak, 1 );
    		ADDBYTE(pak, thisslot );
    		ADDWORD(pak, client->items[thisslot].GetPakHeader( ) );
    		ADDDWORD(pak, client->items[thisslot].GetPakData( ) );
        	client->client->SendPacket( &pak );
		}
		break;
		default:
            if( client->questdebug )Log(MSG_DEBUG, "a002 unknown reward type %i",data->btTarget);
        break;
	}

	return QUEST_SUCCESS;
}

//Restore HP/MP
QUESTREWD(006){
    #ifdef debugging
    Log(MSG_DEBUG, "a006");
    #endif
    client->Stats->HP = client->Stats->MaxHP;
    client->Stats->MP = client->Stats->MaxMP;
	return QUEST_SUCCESS;
}

//Teleport
QUESTREWD(007){
    #ifdef debugging
    Log(MSG_DEBUG, "a007");
    #endif
	GETREWDDATA(007);
	if( client->questdebug ) Log( MSG_DEBUG, "Map ID: %i X: %i Y: %i", data->iZoneSN, data->iX, data->iY );
	fPoint thispoint;
	thispoint.x = floor(((float)data->iX)/100);
	thispoint.y = floor(((float)data->iY)/100);
	GServer->TeleportTo(client, data->iZoneSN, thispoint);
	return QUEST_SUCCESS;
}

//Spawn Monster
QUESTREWD(008){
    #ifdef debugging
    Log(MSG_DEBUG, "a008");
    #endif
    GETREWDDATA(008);
    fPoint position;
    position.x = data->iX;
    position.y = data->iY;
    for( int i=0; i<data->iHowMany; i++ )
    {
        fPoint position;
        position.x = data->iX;
        position.y = data->iY;
        #ifdef debugging
        Log(MSG_DEBUG, "pos.x %u",position.x);
        Log(MSG_DEBUG, "pos.y %u",position.y);
        #endif
        CMap* map = GServer->MapList.Index[data->iZoneSN];
        if (position.x==0||position.y==0||map==0)
        {
            CPlayer* thisclient = client;
            position.x=thisclient->Position->current.x;
            position.y=thisclient->Position->current.y;
            map=GServer->MapList.Index[thisclient->Position->Map];
        #ifdef debugging
        Log(MSG_DEBUG, "Position[%i]: (%.0f,%.0f)",thisclient->Position->Map,thisclient->Position->current.x,thisclient->Position->current.y);
        Log(MSG_DEBUG, "monid[%i]:",data->iMonsterSN);
        #endif
        }
        dword monid = data->iMonsterSN;
        if (monid==253)monid=251;
        position = GServer->RandInCircle( position, data->iRange );
        map->AddMonster( monid, position, 0, NULL, NULL, 0 , true );
    }
	return QUEST_SUCCESS;
}

//Execute Quest Trigger
QUESTREWD(009){
    #ifdef debugging
    Log(MSG_DEBUG, "a009");
    #endif
    //GETREWDDATA(009);
    return QUEST_SUCCESS;
}

//Reset Stats
QUESTREWD(010){
    #ifdef debugging
    Log(MSG_DEBUG, "a010");
    #endif
	client->CharInfo->StatPoints = 0;

	client->Attr->Str = 15;
	client->Attr->Dex = 15;
	client->Attr->Int = 15;
	client->Attr->Con = 15;
	client->Attr->Cha = 10;
	client->Attr->Sen = 10;

	for(int i = 0; i <= client->Stats->Level; i++)
		client->CharInfo->StatPoints += (i + 9);
	
	return QUEST_SUCCESS;
}

//Update Quest Var
QUESTREWD(011){
    #ifdef debugging
    Log(MSG_DEBUG, "a011");
    #endif
	return QUEST_SUCCESS;
}

//NPC Speak
QUESTREWD(012){
    #ifdef debugging
    Log(MSG_DEBUG, "a012");
    #endif
	return QUEST_SUCCESS;
}

//Unknown
QUESTREWD(013){
    #ifdef debugging
    Log(MSG_DEBUG, "a013");
    #endif
	return QUEST_SUCCESS;
}

//Learn Skill
QUESTREWD(014){
    #ifdef debugging
    Log(MSG_DEBUG, "a014");
    #endif
    GETREWDDATA(014);
    GServer->LearnSkill(client, data->iSkillNo);
	return QUEST_SUCCESS;
}

//Set Quest Flag
QUESTREWD(015){
    #ifdef debugging
    Log(MSG_DEBUG, "a015");
    #endif
	GETREWDDATA(015);
	//if( client->questdebug )
	 #ifdef debugging
    Log( MSG_DEBUG, "Flags btOp: %i", data->btOp );
    Log( MSG_DEBUG, "Flags nSN: %i", data->nSN );
    #endif
	client->quest.SetFlag(data->nSN, (data->btOp == 1)?true:false);
	return QUEST_SUCCESS;
}

//Unknown
QUESTREWD(016){
    #ifdef debugging
    Log(MSG_DEBUG, "a016");
    #endif
	return QUEST_SUCCESS;
}

//Reset All Quest Flags
QUESTREWD(017){
    #ifdef debugging
    Log(MSG_DEBUG, "a017");
    #endif
	memset(&client->quest.flags, 0, 244);
	return QUEST_SUCCESS;
}

//Send Announcement
QUESTREWD(018){
    #ifdef debugging
    Log(MSG_DEBUG, "a018");
    #endif
    GETREWDDATA(018);
    
    BEGINPACKET( pak, 0x702 );
	ADDSTRING( pak, data->Data);
	ADDBYTE( pak, 0x00);
	GServer->SendToAll( &pak );
	return QUEST_SUCCESS;
}

//Execute Quest Trigger in Other Map
QUESTREWD(019){
    #ifdef debugging
    Log(MSG_DEBUG, "a019");
    #endif
	return QUEST_SUCCESS;
}

//PvP Status
QUESTREWD(020){
    #ifdef debugging
    Log(MSG_DEBUG, "a020");
    #endif
	return QUEST_SUCCESS;
}

//Set Respawn Position
QUESTREWD(021){
    #ifdef debugging
    Log(MSG_DEBUG, "a021");
    #endif
    GETREWDDATA(021);
	return QUEST_SUCCESS;
}

//Unknown
QUESTREWD(022){
    #ifdef debugging
    Log(MSG_DEBUG, "a022");
    #endif
	return QUEST_SUCCESS;
}

//Raise Clan Grade
QUESTREWD(023){
    #ifdef debugging
    Log(MSG_DEBUG, "a023");
    #endif
    GETREWDDATA(023);
    
    for(UINT i=0;i<GServer->MapList.Map.size();i++)
    {
        CMap* map = GServer->MapList.Map.at(i);
        if( map->PlayerList.size()<1 )
            continue;
        for(UINT j=0;j<map->PlayerList.size();j++)
        {
        CPlayer* player = map->PlayerList.at(j);
        if( player->Clan->clanid != client->Clan->clanid ) continue;
        player->Clan->grade++;
    
        //load clan info in char server
	    BEGINPACKET( pak, 0x7e0 );
	    ADDBYTE    ( pak, 0xfb ); //action to update clan informacion (charserver)
	    ADDWORD    ( pak, player->Clan->clanid );
	    ADDWORD    ( pak, player->Clan->grade );
        GServer->SendISCPacket( &pak );
    
        //Send to other players
        RESETPACKET( pak, 0x7e0 );
        ADDBYTE    ( pak, 0x35 );
        ADDWORD    ( pak, player->clientid );
        ADDWORD    ( pak, player->Clan->clanid);
        ADDWORD    ( pak, 0x0000 );//???
        ADDWORD    ( pak, player->Clan->back );
        ADDWORD    ( pak, player->Clan->logo );
        ADDBYTE    ( pak, player->Clan->grade );//clan grade
        ADDBYTE    ( pak, 0x06 );//clan rank
        ADDSTRING  ( pak, player->Clan->clanname );
        ADDBYTE    ( pak, 0x00 ); 
        GServer->SendToVisible( &pak, player );
        }
    }
    GServer->DB->QExecute("UPDATE list_clan SET grade=%i WHERE id=%i", client->Clan->grade, client->Clan->clanid);
	return QUEST_SUCCESS;
}

//Clan Money
QUESTREWD(024){
    #ifdef debugging
    Log(MSG_DEBUG, "a024");
    #endif
	return QUEST_SUCCESS;
}

//Clan Points
QUESTREWD(025){
    #ifdef debugging
    Log(MSG_DEBUG, "a025");
    #endif
	return QUEST_SUCCESS;
}

//Clan Skill
QUESTREWD(026){
    #ifdef debugging
    Log(MSG_DEBUG, "a026");
    #endif
	return QUEST_SUCCESS;
}

//Clan Contribution
QUESTREWD(027){
    #ifdef debugging
    Log(MSG_DEBUG, "a027");
    #endif
	return QUEST_SUCCESS;
}

//Clan Teleportation
QUESTREWD(028){
    #ifdef debugging
    Log(MSG_DEBUG, "a028");
    #endif
    GETREWDDATA(028);
    fPoint telepos;
    fPoint newPos;
    telepos.x = data->iX/100;
    telepos.y = data->iY/100;
    for(UINT i=0;i<GServer->MapList.Map.size();i++)
    {
        CMap* map = GServer->MapList.Map.at(i);
        if( map->PlayerList.size()<1 )
            continue;
        for(UINT j=0;j<map->PlayerList.size();j++)
        {
            CPlayer* player = map->PlayerList.at(j);
            if( player->Clan->clanid != client->Clan->clanid ) continue;
            newPos = GServer->RandInCircle(telepos, data->iRange);
            GServer->TeleportTo(player, data->nZoneNo, newPos );
        }
    }
	return QUEST_SUCCESS;
}
