#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <psapi.h>
#include <string>
#include <vector>
#include <shlobj.h>
#include <fstream>
#include <filesystem>

#include "Defines.h"

using namespace std;
using std::filesystem::directory_entry;
using std::filesystem::directory_iterator;

class Trainer{
public:
	Trainer();
	~Trainer();

	//PROCESS
	bool GetProcess();
	bool CloseProc();
	bool GetSotFS();
	bool GetProcessStatus();
	MODULEENTRY32 GetModule();
	HANDLE OpenProc();
	void LoadOverlay();
	bool checkBlueAcolyte();

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
	BYTE GetByte(DWORD32 type, DWORD32 par);
	DWORD32 GetDWORD32(DWORD32 type, DWORD32 par);

private:
	//PROCESS
	char*	  ProcessName;
	int		  offsetP;
	int		  offsetName;
	DWORD	  pID;
	HANDLE    pHandle;
	INT64     AddressBase;
	INT64     AddressOffset;
	INT32     AddressOffset32;
};
