#include "Enemy.h"

Enemy::Enemy() {
	result = L"None";

	level = 0;
	Hostil = 0;
	name = L"None";
	result = L"None";
	date = L"None";
	RedRank = 0;

	Health = 0;
	Stamina = 0;
	Vigor = 0;
	Endurance = 0;
	Vitality = 0;
	Attunement = 0;
	Strength = 0;
	Dexterity = 0;
	Adaptability = 0;
	Intelligence = 0;
	Faith = 0;

	memset(Spells, NULL, sizeof(Spells));
	memset(Spells, NULL, sizeof(Cons));

	for (int i = 0; i < 14; i++) Counts[i] = 1;
}

wstring Enemy::GetStrPar(par p)
{
	switch (p)
	{
	case pNAME:
		return name;

	case pRES:
		return result;

	case pDATE:
		return date;
	}
}

void Enemy::SetStrPar(par p, wstring val)
{
	switch (p)
	{
	case pNAME:
		name = val;
		break;

	case pRES:
		result = val;
		break;

	case pDATE:
		date = val;
		break;
	}
}

int Enemy::GetIntPar(par p, int num)
{
	switch (p)
	{
	case pLVL:
		return level;

	case pHOST:
		return Hostil;

	case pRANK:
		return RedRank;

	case pHEALTH:
		return Health;

	case pSTM:
		return Stamina;

	case pVIG:
		return Vigor;

	case pEND:
		return Endurance;

	case pVIT:
		return Vitality;

	case pATN:
		return Attunement;

	case pSTR:
		return Strength;

	case pDEX:
		return Dexterity;

	case pADP:
		return Adaptability;

	case pINT:
		return Intelligence;

	case pFTH:
		return Faith;

	case pCOUNTS:
		return Counts[num];
	}
}

void Enemy::SetIntPar(par p, int val, int num)
{
	switch (p)
	{
	case pLVL:
		level = val;
		break;

	case pHOST:
		Hostil = val;
		break;

	case pRANK:
		RedRank = val;
		break;

	case pHEALTH:
		Health = val;
		break;

	case pSTM:
		Stamina = val;
		break;

	case pVIG:
		Vigor = val;
		break;

	case pEND:
		Endurance = val;
		break;

	case pVIT:
		Vitality = val;
		break;

	case pATN:
		Attunement = val;
		break;

	case pSTR:
		Strength = val;
		break;

	case pDEX:
		Dexterity = val;
		break;

	case pADP:
		Adaptability = val;
		break;

	case pINT:
		Intelligence = val;
		break;

	case pFTH:
		Faith = val;
		break;

	case pCOUNTS:
		Counts[num] = val;
		break;

	}
}

DWORD32 Enemy::GetDwordPar(par p, int num)
{
	switch (p)
	{
	case pR1:
		return R1[num];

	case pR2:
		return R2[num];

	case pR3:
		return R3[num];

	case pL1:
		return L1[num];

	case pL2:
		return L2[num];

	case pL3:
		return L3[num];

	case pR1L:
		return R1Level[num];

	case pR2L:
		return R2Level[num];

	case pR3L:
		return R3Level[num];

	case pL1L:
		return L1Level[num];

	case pL2L:
		return L2Level[num];

	case pL3L:
		return L3Level[num];

	case pR1I:
		return R1Infusion[num];

	case pR2I:
		return R2Infusion[num];

	case pR3I:
		return R3Infusion[num];

	case pL1I:
		return L1Infusion[num];

	case pL2I:
		return L2Infusion[num];

	case pL3I:
		return L3Infusion[num];

	case pHEAD:
		return Head[num];

	case pBODY:
		return Body[num];

	case pHAND:
		return Hands[num];

	case pLEGS:
		return Legs[num];

	case pHEADL:
		return HeadLevel[num];

	case pBODYL:
		return BodyLevel[num];

	case pHANDL:
		return HandsLevel[num];

	case pLEGSL:
		return LegsLevel[num];

	case pRING1:
		return Ring1[num];

	case pRING2:
		return Ring2[num];

	case pRING3:
		return Ring3[num];

	case pRING4:
		return Ring4[num];

	case pSPELLS:
		return Spells[num];

	case pCONS:
		return Cons[num];

	}
}

void Enemy::SetDwordPar(par p, DWORD32 val, int num)
{
	switch (p)
	{
	case pR1:
		R1.push_back(val);
		break;

	case pR2:
		R2.push_back(val);
		break;

	case pR3:
		R3.push_back(val);
		break;

	case pL1:
		L1.push_back(val);
		break;

	case pL2:
		L2.push_back(val);
		break;

	case pL3:
		L3.push_back(val);
		break;

	case pR1L:
		R1Level.push_back(val);
		break;

	case pR2L:
		R2Level.push_back(val);
		break;

	case pR3L:
		R3Level.push_back(val);
		break;

	case pL1L:
		L1Level.push_back(val);
		break;

	case pL2L:
		L2Level.push_back(val);
		break;

	case pL3L:
		L3Level.push_back(val);
		break;

	case pR1I:
		R1Infusion.push_back(val);
		break;

	case pR2I:
		R2Infusion.push_back(val);
		break;

	case pR3I:
		R3Infusion.push_back(val);
		break;

	case pL1I:
		L1Infusion.push_back(val);
		break;

	case pL2I:
		L2Infusion.push_back(val);
		break;

	case pL3I:
		L3Infusion.push_back(val);
		break;

	case pHEAD:
		Head.push_back(val);
		break;

	case pBODY:
		Body.push_back(val);
		break;

	case pHAND:
		Hands.push_back(val);
		break;

	case pLEGS:
		Legs.push_back(val);
		break;

	case pHEADL:
		HeadLevel.push_back(val);
		break;

	case pBODYL:
		BodyLevel.push_back(val);
		break;

	case pHANDL:
		HandsLevel.push_back(val);
		break;

	case pLEGSL:
		LegsLevel.push_back(val);
		break;

	case pRING1:
		Ring1.push_back(val);
		break;

	case pRING2:
		Ring2.push_back(val);
		break;

	case pRING3:
		Ring3.push_back(val);
		break;

	case pRING4:
		Ring4.push_back(val);
		break;

	case pSPELLS:
		Spells[num] = val;
		break;

	case pCONS:
		Cons[num] = val;
		break;
	}
}