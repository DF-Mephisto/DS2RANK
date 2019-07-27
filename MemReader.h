#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <vector>
#include <shlobj.h>
#include <fstream>

#include "Defines.h"

using namespace std;

class Trainer{
public:
	Trainer();
	~Trainer();

	//PROCESS
	bool GetProcess();
	bool CloseProc();
	bool GetSotFS();
	MODULEENTRY32 GetModule();
	HANDLE OpenProc();
	void LoadOverlay();

	void SetOffsetP(int x);
	void SetOffsetName(int x);

	//GET PLAYER DATA
	short int GetRank(DWORD32 type, DWORD32 vtype);
	short int GetRingStat(DWORD32 type);
	short int GetMyHostility();

	//ENEMY DATA AND MISC
	wstring GetEnemyName();
	short int GetHostilityEnemy();
	int GetEnemyLevel();
	int GetHealth();
	int GetStamina();
	short int GetRedRank();

	//PARAMS
	byte GetByte(DWORD32 type, DWORD32 par);
	DWORD32 GetDWORD32(DWORD32 type, DWORD32 par);

private:
	//PROCESS
	char*	ProcessName;
	int	offsetP;
	int	offsetName;
	DWORD	pID;
	HANDLE  pHandle;
	INT64   AddressBase;
	INT64   AddressOffset;
	INT32   AddressOffset32;
};
