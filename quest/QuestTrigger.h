// Added By Raven0123
class CQuestTrigger {
public:
	struct SQuestDatum {
		int size;
		int opcode;
		byte* data;
	};
	
	strings TriggerName;
	dword TriggerHash;

	byte CheckNext;
	dword id;

	dword ConditionCount;
	SQuestDatum** Conditions;

	dword ActionCount;
	SQuestDatum** Actions;
private:
};

struct STR_NPCVAR {
	dword iNpcNo;
	word nVarNo;
	word unused;
};

struct STR_ITEM_DATA {
	dword uiItemSN;
	dword iWhere;
	dword iRequestCnt;
	byte btOp;
};

struct STR_QUEST_DATA {
	union {
		dword iType;
		struct {
			byte m_wVarNO;
			byte m_wVarNO2;
			byte m_wVarTYPE;
			byte m_wVarTYPE2;
		};
	};
	word nValue;
	byte btOp;
};

struct STR_ABIL_DATA {
	dword iType;
	int iValue;
	byte btOp;
};

#define QUEST_FAILURE 0
#define QUEST_SUCCESS 1

#include "QuestConditions.h"
#include "QuestActions.h"
class CPlayerQuests {
public:
dword GetRewardValue(dword function, dword amount, CPlayer* client, word nDupCNT);
int OperateValues(byte btOp, int* value1PTR, int value2);
int OperateValues(byte btOp, byte* value1PTR, int value2);

};
