#pragma once

#include <Windows.h>
#include <string>
#include <vector>

#include "Defines.h"

using namespace std;

enum par
{
	pLVL, pHOST, pNAME, pRES, pDATE, pRANK, pCOUNTS, pHEALTH, pSTM, pVIG, pEND, pVIT, pATN, pSTR, pDEX, pADP, pINT, pFTH,
	pR1, pR2, pR3, pL1, pL2, pL3, pR1L, pR2L, pR3L, pL1L, pL2L, pL3L, pR1I, pR2I, pR3I, pL1I, pL2I, pL3I, pHEAD, pBODY, 
	pHAND, pLEGS, pHEADL, pBODYL, pHANDL, pLEGSL, pRING1, pRING2, pRING3, pRING4, pSPELLS, pCONS
};

class Enemy {
private:
	//LEVEL AND MISC
	wstring name;
	wstring result;
	wstring date;

	int level;
	int Hostil;
	int RedRank;

	int Counts[14];

	//STATS
	int Health;
	int Stamina;
	int Vigor;
	int Endurance;
	int Vitality;
	int Attunement;
	int Strength;
	int Dexterity;
	int Adaptability;
	int Intelligence;
	int Faith;

	//WEAPONS
	vector<DWORD32> R1;
	vector<DWORD32> R2;
	vector<DWORD32> R3;
	vector<DWORD32> L1;
	vector<DWORD32> L2;
	vector<DWORD32> L3;

	//WEAPONS ENCHANTMENTS
	vector<DWORD32> R1Level;
	vector<DWORD32> R2Level;
	vector<DWORD32> R3Level;
	vector<DWORD32> L1Level;
	vector<DWORD32> L2Level;
	vector<DWORD32> L3Level;

	//INFUSIONS
	vector<DWORD32> R1Infusion;
	vector<DWORD32> R2Infusion;
	vector<DWORD32> R3Infusion;
	vector<DWORD32> L1Infusion;
	vector<DWORD32> L2Infusion;
	vector<DWORD32> L3Infusion;

	//ARMOR
	vector<DWORD32> Head;
	vector<DWORD32> Body;
	vector<DWORD32> Hands;
	vector<DWORD32> Legs;

	//ARMOR ENCHANTMETS
	vector<DWORD32> HeadLevel;
	vector<DWORD32> BodyLevel;
	vector<DWORD32> HandsLevel;
	vector<DWORD32> LegsLevel;

	//RINGS
	vector<DWORD32> Ring1;
	vector<DWORD32> Ring2;
	vector<DWORD32> Ring3;
	vector<DWORD32> Ring4;

	//SPELLS
	DWORD32 Spells[10];

	//CONSUMABLES
	DWORD32 Cons[10];

public:
	//PUBLIC FUNCTIONS
	Enemy();

	void SetDwordPar(par, DWORD32, int = 0);
	DWORD32 GetDwordPar(par, int);

	void SetIntPar(par, int, int = 0);
	int GetIntPar(par, int = 0);

	void SetStrPar(par, wstring);
	wstring GetStrPar(par);
};
