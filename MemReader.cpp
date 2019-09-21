#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "MemReader.h"

using std::string;

//PROCESS
Trainer::Trainer() {
	offsetP =     0x1e8;
	offsetName =  0x234;
	pHandle =     0;
	pID =         0;
	AddressBase = 0;

	ProcessName = new char[16];
	strcpy_s(ProcessName, 16, "DarkSoulsII.exe");
}

Trainer::~Trainer() {
	delete[] ProcessName;
}

void Trainer::SetOffsetP(int x) {
	offsetP = x;
}
void Trainer::SetOffsetName(int x) {
	offsetName = x;
}

bool Trainer::GetProcess() {
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pInfo;
	pInfo.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(snapshot, &pInfo)) {
		do {
			if (!strcmp(ProcessName, pInfo.szExeFile)) {
				pID = pInfo.th32ProcessID;
				CloseHandle(snapshot);
				return true;
			}
		} while (Process32Next(snapshot, &pInfo));
	}
	CloseHandle(snapshot);
	return false;
}

MODULEENTRY32 Trainer::GetModule() {
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID);
	MODULEENTRY32 mInfo;
	mInfo.dwSize = sizeof(MODULEENTRY32);
	Module32First(snapshot, &mInfo);
	AddressBase = (INT64)mInfo.modBaseAddr;
	CloseHandle(snapshot);
	return mInfo;
}

HANDLE Trainer::OpenProc() {
	if (pHandle) CloseHandle(pHandle);
	pHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, pID);
	return pHandle;
}

bool Trainer::GetProcessStatus()
{
	DWORD exitCode = 0;
	GetExitCodeProcess(pHandle, &exitCode);

	return exitCode == STILL_ACTIVE ? true : false;
}

bool Trainer::CloseProc() {
	if (CloseHandle(pHandle))
	{
		pHandle = 0;
		pID = 0;
	}
	return !(bool)pHandle;
}

void Trainer::LoadOverlay()
{
	//CHECK LOADED DLL
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID);
	MODULEENTRY32 mInfo;

	mInfo.dwSize = sizeof(MODULEENTRY32);
	if (Module32First(snapshot, &mInfo)) {
		do {
			if (!strcmp("DS2Overlay.dll", mInfo.szModule)) {
				CloseHandle(snapshot);
				return;
			}
		} while (Module32Next(snapshot, &mInfo));
	}
	CloseHandle(snapshot);

	//GET PATH
	PWSTR MyDoc = new wchar_t[MAX_PATH];
	wchar_t MyDocFolder[MAX_PATH];
	char path[MAX_PATH];
	ZeroMemory(path, MAX_PATH);

	SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_DEFAULT, NULL, &MyDoc);
	wcscpy_s(MyDocFolder, MyDoc);
	wcscat_s(MyDocFolder, L"\\DS2RANK");
	wcscat_s(MyDocFolder, L"\\DS2Overlay.dll");
	
	locale old = std::locale::global(std::locale(""));
	wcstombs(path, MyDocFolder, MAX_PATH);
	std::locale::global(old);
	
	HANDLE Thread;
	HMODULE ll = GetModuleHandle("kernel32.dll");

	LPVOID func = GetProcAddress(ll, "LoadLibraryA");
	LPVOID arg_address = (LPVOID)VirtualAllocEx(pHandle, 0, strlen(path), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	WriteProcessMemory(pHandle, (LPVOID)arg_address, path, strlen(path), 0);
	Thread = CreateRemoteThread(pHandle, 0, 0, (LPTHREAD_START_ROUTINE)func, arg_address, 0, 0);

	delete MyDoc;
}

short int Trainer::GetRingStat(DWORD32 type)
{
	short int RingStat;
	ReadProcessMemory(pHandle, (LPVOID)(AddressBase + 0x160DCD8), &AddressOffset, 8, NULL);
	ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + 0x30), &AddressOffset, 8, NULL);
	if (AddressOffset == 0) return 0;
	ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + 0x68), &AddressOffset, 8, NULL);
	ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + type), &RingStat, 2, NULL);
	return RingStat;
}

//PLAYER DATA
bool Trainer::GetSotFS()
{
	if (AddressBase > 0xFFFFFFFF) return true;
	else return false;
}

short int Trainer::GetRank(DWORD32 type, DWORD32 vtype){
	short int Rank;
	if (GetSotFS()) {
		ReadProcessMemory(pHandle, (LPVOID)(AddressBase + 0x160B8D0), &AddressOffset, 8, NULL);
		ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + 0xd0), &AddressOffset, 8, NULL);
		if (AddressOffset == 0) return 0;
		ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + 0x490), &AddressOffset, 8, NULL);
		ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + type), &Rank, 2, NULL);
	}
	else {
		ReadProcessMemory(pHandle, (LPVOID)(AddressBase + 0x11493F4), &AddressOffset32, 4, NULL);
		ReadProcessMemory(pHandle, (LPVOID)(AddressOffset32 + 0x74), &AddressOffset32, 4, NULL);
		if (AddressOffset == 0) return 0;
		ReadProcessMemory(pHandle, (LPVOID)(AddressOffset32 + 0x378), &AddressOffset32, 4, NULL);
		ReadProcessMemory(pHandle, (LPVOID)(AddressOffset32 + vtype), &Rank, 2, NULL);
	}
	return Rank;
}

short int Trainer::GetMyHostility() {
	short int MyHostility;
	ReadProcessMemory(pHandle, (LPVOID)(AddressBase + 0x160B8D0), &AddressOffset, 8, NULL);
	ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + 0xd0), &AddressOffset, 8, NULL);
	if (AddressOffset == 0) return errorHostil;
	ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + 0xb0), &AddressOffset, 8, NULL);
	ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + 0x3c), &MyHostility, 2, NULL);
	return MyHostility;
}

//LEVEL AND MISC
short int Trainer::GetRedRank() {
	short int RedRank;
	ReadProcessMemory(pHandle, (LPVOID)(AddressBase + 0x160DCD8), &AddressOffset, 8, NULL);
	ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + 0x20), &AddressOffset, 8, NULL);
	ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + offsetP), &AddressOffset, 8, NULL);
	ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + 0x390), &AddressOffset, 8, NULL);
	ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + 0x2b8), &RedRank, 2, NULL);
	return RedRank;
}

wstring Trainer::GetEnemyName() {
	wchar_t letter;
	wstring EnemyNameStr;

	ReadProcessMemory(pHandle, (LPVOID)(AddressBase + 0x160DCD8), &AddressOffset, 8, NULL);
	ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + 0x20), &AddressOffset, 8, NULL);

	for (int i = offsetName, j = 0; j < 30; i += 0x2, j++) {
		ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + i), &letter, 2, NULL);
		if (letter == L' ') letter = L'_';
		if (letter == NULL) continue;
		EnemyNameStr += letter;
	}
	return EnemyNameStr;
}

int Trainer::GetEnemyLevel() {
	int EnemyLevel;
	ReadProcessMemory(pHandle, (LPVOID)(AddressBase + 0x160DCD8), &AddressOffset, 8, NULL);
	ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + 0x20), &AddressOffset, 8, NULL);
	ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + offsetP), &AddressOffset, 8, NULL);
	ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + 0x390), &AddressOffset, 8, NULL);
	ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + 0x1c0), &EnemyLevel, 4, NULL);
	return EnemyLevel;
}

short int Trainer::GetHostilityEnemy() {
	short int HostilityEnemy;
	ReadProcessMemory(pHandle, (LPVOID)(AddressBase + 0x160DCD8), &AddressOffset, 8, NULL);
	ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + 0x20), &AddressOffset, 8, NULL);
	ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + offsetP), &AddressOffset, 8, NULL);
	if (AddressOffset == 0) return errorHostil;
	ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + 0xb0), &AddressOffset, 8, NULL);
	ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + 0x3c), &HostilityEnemy, 2, NULL);
	return HostilityEnemy;
}

int Trainer::GetHealth() {
	int Health;
	ReadProcessMemory(pHandle, (LPVOID)(AddressBase + 0x160DCD8), &AddressOffset, 8, NULL);
	ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + 0x20), &AddressOffset, 8, NULL);
	ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + offsetP), &AddressOffset, 8, NULL);
	ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + 0x170), &Health, 4, NULL);
	return Health;
}

int Trainer::GetStamina() {
	float Stamina;
	ReadProcessMemory(pHandle, (LPVOID)(AddressBase + 0x160DCD8), &AddressOffset, 8, NULL);
	ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + 0x20), &AddressOffset, 8, NULL);
	ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + offsetP), &AddressOffset, 8, NULL);
	ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + 0x1b4), &Stamina, 4, NULL);
	return (int)(Stamina / 10);
}

//PARAMS
byte Trainer::GetByte(DWORD32 type, DWORD32 par){
	byte val;
	ReadProcessMemory(pHandle, (LPVOID)(AddressBase + 0x160DCD8), &AddressOffset, 8, NULL);
	ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + 0x20), &AddressOffset, 8, NULL);
	ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + offsetP), &AddressOffset, 8, NULL);
	ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + par), &AddressOffset, 8, NULL);
	ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + type), &val, 1, NULL);
	return val;
}

DWORD32 Trainer::GetDWORD32(DWORD32 type, DWORD32 par){
	DWORD32 val;
	ReadProcessMemory(pHandle, (LPVOID)(AddressBase + 0x160DCD8), &AddressOffset, 8, NULL);
	ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + 0x20), &AddressOffset, 8, NULL);
	ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + offsetP), &AddressOffset, 8, NULL);
	ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + par), &AddressOffset, 8, NULL);
	ReadProcessMemory(pHandle, (LPVOID)(AddressOffset + type), &val, 4, NULL);
	return val;
}
