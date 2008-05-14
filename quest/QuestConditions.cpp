// Added By Raven0123
#include "../WorldServer.h"

int CPlayerQuests::OperateValues(byte btOp, int* value1PTR, int value2){
	int value1 = *value1PTR;
	#ifdef debugging
	 Log(MSG_DEBUG, "switch:%i val1:%i val2: %i",btOp,value1,value2);
	 #endif
	switch(btOp){
		case 0:
			return (value1 == value2);
		case 1:
			return (value1 != value2);
		case 2:
			return (value1 >= value2);
		case 3:
			return (value1 < value2);
		case 4:
			return (value1 <= value2);
		case 5:
			(*value1PTR) = value2;
			return 1;
		case 6:
			(*value1PTR) = value1 + value2;
			return 1;
		case 7:
			(*value1PTR) = value1 - value2;
			return 1;
		default:
			return 0;
	}
}
int CPlayerQuests::OperateValues(byte btOp, byte* value1PTR, int value2){
	int value1 = *value1PTR;
	#ifdef debugging
	 Log(MSG_DEBUG, "switch:%i val1:%i val2: %i",btOp,value1,value2);
	 #endif
	switch(btOp){
		case 0:
			return (value1 == value2);
		case 1:
			return (value1 != value2);
		case 2:
			return (value1 >= value2);
		case 3:
			return (value1 < value2);
		case 4:
			return (value1 <= value2);
		case 5:
			(*value1PTR) = value2;
			return 1;
		case 6:
			(*value1PTR) = value1 + value2;
			return 1;
		case 7:
			(*value1PTR) = value1 - value2;
			return 1;
		case 9:
			(*value1PTR) += 1;
			return 1;
		default:
            Log(MSG_DEBUG, "unknown btop: %i",btOp);
			return 0;
	}
}

//Check Quest
QUESTCOND(000){
    #ifdef debugging
        Log(MSG_DEBUG, "c000");
    #endif
	GETCONDDATA(000);
	if( client->questdebug ) Log(MSG_DEBUG, "Quest # needed: %i", data->iQuestSN);
    
	for(dword i = 0; i < 10; i++){
        if( client->questdebug ) Log(MSG_DEBUG, "Current quest #: %i", client->quest.quests[i].QuestID);
        
		if(client->quest.quests[i].QuestID == data->iQuestSN)
		{
            client->ActiveQuestnumber=i;
			return QUEST_SUCCESS;
        }
	}
	return QUEST_FAILURE;
}

//Check Quest Variable
QUESTCOND(001){
    #ifdef debugging
        Log(MSG_DEBUG, "c001");
    #endif
    bool ok = false;
    CPlayerQuests *pquests;
    GETCONDDATA(001);
    for(int i = 0; i < data->iDataCnt; i++){
		qword address = i * 0x0C;
		address += (qword)data;
		address += 4;
		STR_QUEST_DATA* variable = (STR_QUEST_DATA*)address;
		
		#ifdef debugging
		Log(MSG_DEBUG, "variable:%i = %i",variable->m_wVarNO, client->quest.quests[client->ActiveQuestnumber].Variables[variable->m_wVarNO]);
       // Log(MSG_DEBUG, "iType      : %i",thisflag->iType);
        Log(MSG_DEBUG, "m_wVarNO   : %i",variable->m_wVarNO);
        Log(MSG_DEBUG, "m_wVarNO2  : %i",variable->m_wVarNO2);
        Log(MSG_DEBUG, "m_wVarTYPE : %i",variable->m_wVarTYPE);
        Log(MSG_DEBUG, "m_wVarTYPE2: %i",variable->m_wVarTYPE2);
        Log(MSG_DEBUG, "nValue     : %i",variable->nValue);
        Log(MSG_DEBUG, "btOp       : %i",variable->btOp);
        #endif
        if(pquests->OperateValues(variable->btOp, &client->quest.quests[client->ActiveQuestnumber].Variables[variable->m_wVarNO], variable->nValue))
            ok = true;
		    //return QUEST_FAILURE;
    }
    if (ok==true)
        return QUEST_SUCCESS;
    else
        return QUEST_FAILURE;
}

//Check Quest Variable
QUESTCOND(002){
    #ifdef debugging
        Log(MSG_DEBUG, "c002");
    #endif
    CPlayerQuests *pquests;
    GETCONDDATA(002);
    for(int i = 0; i < data->iDataCnt; i++){
		qword address = i * 0x0C;
		address += (qword)data;
		address += 4;
		STR_QUEST_DATA* variable = (STR_QUEST_DATA*)address;
		switch(variable->m_wVarTYPE2)
		{
            case 3:
                if(!pquests->OperateValues(variable->btOp, &client->quest.variable[0], variable->nValue))
                return QUEST_FAILURE;
                break;
            case 4:
               if(!pquests->OperateValues(variable->btOp, &client->quest.variable[10+variable->m_wVarNO], variable->nValue))
                return QUEST_FAILURE;
                break;
            default:
               if(!pquests->OperateValues(variable->btOp, &client->quest.variable[variable->m_wVarNO], variable->nValue))
               Log(MSG_DEBUG, "unknown variable location   : %i",variable->m_wVarTYPE2);
                return QUEST_FAILURE;
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

//Check Stats
QUESTCOND(003){
    #ifdef debugging
        Log(MSG_DEBUG, "c003");
    #endif
	GETCONDDATA(003);
	CPlayerQuests *pquests;
	int jobnumber=0;
	for(int i = 0; i < data->iDataCnt; i++){
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
             jobnumber = client->CharInfo->Job/100;
             if(!pquests->OperateValues(curAbil->btOp, (int*)&jobnumber, curAbil->iValue))
			//if(!pquests->OperateValues(curAbil->btOp, (int*)&client->CharInfo->Job, curAbil->iValue))
				return QUEST_FAILURE;
			break;

		/*case sUnion:
			if(!pquests->OperateValues(curAbil->btOp, (int*)&client->Union], curAbil->iValue))
				return QUEST_FAILURE;
			break;*/

		case sStrength:
			if(!pquests->OperateValues(curAbil->btOp, (int*)&client->Attr->Str, curAbil->iValue))
				return QUEST_FAILURE;
			break;

		case sDexterity:
			if(!pquests->OperateValues(curAbil->btOp, (int*)&client->Attr->Dex, curAbil->iValue))
				return QUEST_FAILURE;
			break;

		case sIntelligence:
			if(!pquests->OperateValues(curAbil->btOp, (int*)&client->Attr->Int, curAbil->iValue))
				return QUEST_FAILURE;
			break;

		case sConcentration:
			if(!pquests->OperateValues(curAbil->btOp, (int*)&client->Attr->Con, curAbil->iValue))
				return QUEST_FAILURE;
			break;

		case sCharm:
			if(!pquests->OperateValues(curAbil->btOp, (int*)&client->Attr->Cha, curAbil->iValue))
				return QUEST_FAILURE;
			break;

		case sSensibility:
			if(!pquests->OperateValues(curAbil->btOp, (int*)&client->Attr->Sen, curAbil->iValue))
				return QUEST_FAILURE;
			break;

		case sLevel:
			if(!pquests->OperateValues(curAbil->btOp, (int*)&client->Stats->Level, curAbil->iValue))
				return QUEST_FAILURE;
			break;

		case sStatPoints:
			if(!pquests->OperateValues(curAbil->btOp, (int*)&client->CharInfo->StatPoints, curAbil->iValue))
				return QUEST_FAILURE;
			break;
		case sMoney:
			if(!pquests->OperateValues(curAbil->btOp, (int*)&client->CharInfo->Zulies, curAbil->iValue))
				return QUEST_FAILURE;
			break;
		default:
			Log(MSG_WARNING, "Type Unknown: '%i'", curAbil->iType);
		}
	}
	return QUEST_SUCCESS;
}

//Check Items
QUESTCOND(004){
    #ifdef debugging
        Log(MSG_DEBUG, "c004");
    #endif
	GETCONDDATA(004);
	CPlayerQuests *pquests;
	for(int i = 0; i < data->iDataCnt; i++){
		//QWord for 64bit safe pointers.
		qword address = i * 0x10;
		address += (qword)data;
		address += 4;
		STR_ITEM_DATA* curItem = (STR_ITEM_DATA*)address;//Log(MSG_INFO,"checking items %i %i %i %i",curItem->uiItemSN, curItem->iWhere, curItem->btOp ,curItem->iRequestCnt);
		if (curItem->iWhere==13)
		{
            for (unsigned int j=0;j<10;j++)
            {
                for (unsigned int k=0;k<5;k++)
                {
                    if (client->quest.quests[i].Items[j].itemnum==curItem->uiItemSN)
                    {
                        if(!pquests->OperateValues(curItem->btOp, &client->quest.quests[i].Items[j].count, curItem->iRequestCnt))
                        	return QUEST_FAILURE;
                        else
                            return QUEST_SUCCESS;
                    }
                }
            }
        }
		if(curItem->uiItemSN == 0 && curItem->iWhere != 0){
			int itemHead = client->items[curItem->iWhere].itemtype;
			if(!pquests->OperateValues(curItem->btOp, &itemHead, curItem->uiItemSN))
				return QUEST_FAILURE;
		}
		if( client->questdebug )Log(MSG_INFO,"uiItemSN: %u iWhere: %u iRequestCnt: %u btOp: %u", curItem->uiItemSN, curItem->iWhere, curItem->iRequestCnt, curItem->btOp);
	}
	return QUEST_SUCCESS;
}

//Check Party
QUESTCOND(005){
    #ifdef debugging
        Log(MSG_DEBUG, "c005");
    #endif
	return QUEST_SUCCESS;
}

//Near Point
QUESTCOND(006){
    #ifdef debugging
        Log(MSG_DEBUG, "c006");
    #endif
	GETCONDDATA(006);
	client->UpdatePosition();
	if(data->iZoneSN != 0){
		if(client->Position->Map != data->iZoneSN)
			return QUEST_FAILURE;
	}

	float dx = client->Position->current.x - (float)data->iX;
	float dy = client->Position->current.y - (float)data->iY;
	float distance = sqrt((dx*dx) + (dy*dy));
	return (distance < data->iRadius)?QUEST_SUCCESS:QUEST_FAILURE;
}

//World Time
QUESTCOND(007){
    #ifdef debugging
        Log(MSG_DEBUG, "c007");
	#endif
    return QUEST_SUCCESS;
}

//Quest Time
QUESTCOND(008){
    #ifdef debugging
        Log(MSG_DEBUG, "c008");
	#endif
    return QUEST_SUCCESS;
}

//Check Skill
QUESTCOND(009){
    #ifdef debugging
        Log(MSG_DEBUG, "c009");
	#endif
    GETCONDDATA(009);
	int checkVal = (data->btOp)?QUEST_SUCCESS:QUEST_FAILURE;
	for(dword i = 0; i < 30; i++){
        if(client->bskills[i] == data->iSkillSN1)
        	return (data->btOp)?QUEST_SUCCESS:QUEST_FAILURE;
        if(client->askill[i] == data->iSkillSN1)
        	return (data->btOp)?QUEST_SUCCESS:QUEST_FAILURE;
        if(client->pskill[i] == data->iSkillSN1)
        	return (data->btOp)?QUEST_SUCCESS:QUEST_FAILURE;
    }

//	for(dword i = 0; i < 90; i++){
//		if(client->cskills[i].id == data->iSkillSN1)
//			return (data->btOp)?QUEST_SUCCESS:QUEST_FAILURE;
//	}
	return (data->btOp)?QUEST_FAILURE:QUEST_SUCCESS;
}

//Unknown
QUESTCOND(010){
    #ifdef debugging
        Log(MSG_DEBUG, "c010");
    #endif
    GETCONDDATA(010);
    #ifdef debugging
     Log( MSG_DEBUG, "Flags btOp: %i", data->btLowPcnt);
    Log( MSG_DEBUG, "Flags nSN: %i", data->btHighPcnt );
    #endif
	return QUEST_SUCCESS;
}

//Quest Variable
QUESTCOND(011){
    #ifdef debugging
        Log(MSG_DEBUG, "c011");
    #endif
	return QUEST_SUCCESS;
}

//Execute Trigger in Zone
QUESTCOND(012){
    #ifdef debugging
        Log(MSG_DEBUG, "c012");
    #endif
	return QUEST_SUCCESS;
}

//Near NPC
QUESTCOND(013){
    #ifdef debugging
        Log(MSG_DEBUG, "c013");
    #endif
	return QUEST_SUCCESS;
}

//Check Quest Flag
QUESTCOND(014){
    #ifdef debugging
        Log(MSG_DEBUG, "c014");
	#endif
    GETCONDDATA(014);
    #ifdef debugging
     Log( MSG_DEBUG, "check flag %i %i", data->btOp, data->nSN );
    Log( MSG_DEBUG, "Flag %i = %i", data->nSN, client->quest.GetFlag(data->nSN) );
    #endif
	bool dataOp = (data->btOp)?true:false;
	return (client->quest.GetFlag(data->nSN) == dataOp)?QUEST_SUCCESS:QUEST_FAILURE;
}

//Unknown
QUESTCOND(015){
    #ifdef debugging
        Log(MSG_DEBUG, "c015");
	#endif
    return QUEST_SUCCESS;
}

//Zone Time
QUESTCOND(016){
    #ifdef debugging
        Log(MSG_DEBUG, "c016");
	#endif
    return QUEST_SUCCESS;
}

//NPC Variables
QUESTCOND(017){
    #ifdef debugging
        Log(MSG_DEBUG, "c017");
	#endif
    return QUEST_SUCCESS;
}

//Time on Date
QUESTCOND(018){
    #ifdef debugging
        Log(MSG_DEBUG, "c018");
	#endif
    return QUEST_SUCCESS;
}

//Time on Day
QUESTCOND(019){
    #ifdef debugging
        Log(MSG_DEBUG, "c019");
	#endif
    return QUEST_SUCCESS;
}

//Unknown
QUESTCOND(020){
    #ifdef debugging
        Log(MSG_DEBUG, "c020");
	#endif
    return QUEST_SUCCESS;
}

//Unknown
QUESTCOND(021){
    #ifdef debugging
        Log(MSG_DEBUG, "c021");
	#endif
    return QUEST_SUCCESS;
}

//Check Server/Channel
QUESTCOND(022){
    #ifdef debugging
        Log(MSG_DEBUG, "c022");
	#endif
    return QUEST_SUCCESS;
}

//In Clan
QUESTCOND(023){
    #ifdef debugging
        Log(MSG_DEBUG, "c023");
	#endif
    return QUEST_SUCCESS;
}

//Clan Position
QUESTCOND(024){
    #ifdef debugging
        Log(MSG_DEBUG, "c024");
	#endif
    return QUEST_SUCCESS;
}

//Clan Contribution
QUESTCOND(025){
    #ifdef debugging
        Log(MSG_DEBUG, "c025");
	#endif
    return QUEST_SUCCESS;
}

//Clan Grade
QUESTCOND(026){
    #ifdef debugging
        Log(MSG_DEBUG, "c026");
	#endif
    return QUEST_SUCCESS;
}

//Clan Points
QUESTCOND(027){
    #ifdef debugging
        Log(MSG_DEBUG, "c027");
    #endif
    GETCONDDATA(027);
    #ifdef debugging
        Log( MSG_WARNING, "Unknown Clanpoint btOP: %i", data->btOP );
    #endif
    switch(data->btOP)
    {
        case 0x01:
             BEGINPACKET( pak, 0x7e0 );
             ADDBYTE    ( pak, 0x5C ); //action to update clan points (charserver)
	         ADDWORD    ( pak, client->Clan->clanid);
	         ADDWORD    ( pak, client->clientid );
             ADDDWORD    ( pak, client->Clan->CP );
             ADDWORD    ( pak, 0x00);
             GServer->SendISCPacket( &pak );
             break;
    }
	return QUEST_SUCCESS;
}

//Clan Funds
QUESTCOND(028){
    #ifdef debugging
        Log(MSG_DEBUG, "c028");
	#endif
    return QUEST_SUCCESS;
}

//Clan Members
QUESTCOND(029){
    #ifdef debugging
        Log(MSG_DEBUG, "c029");
	#endif
    return QUEST_SUCCESS;
}

//Clan Skills
QUESTCOND(030){
    #ifdef debugging
        Log(MSG_DEBUG, "c030");
	#endif
    return QUEST_SUCCESS;
}
