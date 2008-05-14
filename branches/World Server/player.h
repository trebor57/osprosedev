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
#ifndef _ROSE_PLAYER_
#define _ROSE_PLAYER_
#include "character.h"

class CPlayer: public CCharacter
{
    
    public:
        CPlayer( CClientSocket* );
        ~CPlayer( );
    
    //
    CClientSocket* client;
    bool questdebug;// Show Quest message in console?
    bool Saved;// already save this player?(used ondisconnect/returnchar)
    bool isInvisibleMode;    	
    
    // Fairy
    bool Fairy;
    UINT FairyListIndex;
            
    // variables
    USEDITEM* UsedItem; 
    INFO* CharInfo;
    RIDE* Ride;
    TRADE* Trade;
    PARTY* Party;
    SHOP* Shop;
    SESSION* Session;
    CLAN* Clan;
    ATTRIBUTES* Attr;
    CRespawnPoints GMRespawnPoints;
    
    // Inventory/storage
    CItem storageitems[MAX_STORAGE];    
    unsigned int nstorageitems;		
    CItem items[MAX_INVENTORY];    
    
    // skills/quickbar
    SKILLS cskills[MAX_SKILL];
    UINT bskills[MAX_BASICSKILL];			
    UINT quickbar[MAX_QUICKBAR];
   	unsigned short askill[30];
	unsigned char  askilllvl[30];
	unsigned short pskill[30];
	unsigned char  pskilllvl[30];
    int p_skills;    
    
    // Quest info
    BYTE ActiveQuestnumber;  //active quest #
    DWORD ActiveQuest;  //active quest #
    strings TriggerName[10]; // Raven0123
    //CQuest quests[10]; // Raven0123
	SQuestData quest; // New Quest Loader // Raven0123
       
    // Time
	clock_t lastRegenTime;
	clock_t lastSaveTime;	
	clock_t lastGG;	
	
	// Visible Lists
    vector<CPlayer*>	        VisiblePlayers;	   // Visible players
    vector<CDrop*>		        VisibleDrops;	   // Visible drops
    vector<unsigned int>		    VisibleMonsters;   // Visible monsters
    vector<CNPC*>			    VisibleNPCs;	   // Visible npcs
    
    // Functions
        bool CheckPlayerLevelUP( );
        void SetStats( );
        bool GetPlayerInfo( ); 
        bool IsMonster( );
        bool CleanPlayerVector( );
       	bool loaddata( );
        void savedata( );   
        void savequests( CPlayer* thisclient );
        UINT GetNewStorageItemSlot( CItem thisitem );
        UINT GetNewItemSlot( CItem thisitem );        
        bool ClearObject( unsigned int otherclientid );
        void RestartPlayerVal( );
        bool Regeneration( );
        bool PlayerHeal( );
        bool VisiblityList( );
        bool SpawnToPlayer( CPlayer* player, CPlayer* otherclient );
        bool UpdateValues( );
        void ReduceABC( );
        CParty* GetParty( );
        unsigned int AddItem( CItem item );
        void UpdateInventory(unsigned int slot1, unsigned int slot2=0xffff );
        
        // quest
        bool ProcessTrigger( CPlayer* thisclient, CNPCTrigger* thisaction );
        bool DelQuest( unsigned long int questid );
        void SetQuestFlag( unsigned flagid, bool value );
        bool CheckQuestFlag( unsigned flagid );
        bool CheckQuest( unsigned short questid );
        bool StartQuest( unsigned short questid );
        void ChangeQuest( unsigned short questid, unsigned newquestid );
        bool EndQuest( unsigned short questid );
        void UpdateQuestItem( unsigned short questid, unsigned itemid, int count );
        unsigned short CountQuestItem( unsigned short questid, unsigned itemid );
        unsigned char AddInvItem( CItem* thisitem );
        bool GiveQuestReward( CQuest* thisquest );        
        
    	// Player Stats
        unsigned int GetAttackPower( );
        unsigned int GetDefense( );
        unsigned int GetDodge( );
        unsigned int GetAccury( );
        unsigned int GetMagicDefense( );
        unsigned int GetCritical( );
        unsigned int GetAttackSpeed( );
        unsigned int GetMoveSpeed( );   
        unsigned int GetMaxHP( );   
        unsigned int GetMaxMP( );   
        float GetAttackDistance( ); 	
        unsigned int GetHPRegenAmount( );
        unsigned int GetMPRegenAmount( ); 
        unsigned int GetXPRate( );       
        unsigned int GetMaxWeight( );      	
      	unsigned int GetCurrentWeight( );
      	unsigned int GetMPReduction( );        
        unsigned int GetMaxSummonGauge( );        
        void GetExtraStats( );      
        unsigned int GetLevelEXP( );             
        unsigned int GetInt( );
		// No description
		int GetNewSkillSlot(char skilltype);
};

#endif
