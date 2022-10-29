#pragma once
#include <Windows.h>
#include <gdiplus.h>
#include <atlbase.h>
#include <string>
#include <shlobj.h>
#include <vector>
#include <map>
#include <ctime>
#include <fstream>
#include <sstream>

#include "MemReader.h"
#include "Enemy.h"
#include "IDs.h"
#include "Defines.h"


using namespace std;
using namespace Gdiplus;

#define ID_START		1
#define ID_BLOODSTAT	2
#define ID_WORLDSTAT	3
#define ID_ADDITION		4
#define ID_STATWINDOW	5

#define ID_RETURNBTN	100

#define ENTRYCOUNT		16
#define STATCOUNT		56
#define EQUIPCOUNT		14
#define PLAYERCOUNT		5

#define X_NAME_POS		10
#define X_LEVEL_POS		240
#define X_HOST_POS		320
#define X_RES_POS		320
#define X_LIST_BTN		450
#define X_LDATA_BTN		95
#define X_RDATA_BTN		145
#define X_RET_BTN		440

#define X_DATA_LABEL	250
#define X_DATA_NAME		10
#define X_DATA_PAR		180
#define X_DATA_COUNT	130

#define Y_START_POS		3
#define Y_BTN_START_POS	5

#define Y_OFFSET		22

#define XLISTBTN_SZ		15
#define YLISTBTN_SZ		15
#define XDATA_SZ		20
#define YDATA_SZ		15
#define XRET_SZ			30
#define YRET_SZ			20


enum StatisticsType
{
	bloodStat = 0,
	worldStat = 1,
	emptyStat = 2
};

enum DataType
{
	label = 0,
	sublabel = 1,
	parameter = 2,
	equip = 3
};


enum Stages
{
	stage0 = 0,
	stage1 = 1
};

struct playerData
{
	DataType type;
	wstring	name;
	wstring count;
	wstring parameter;
};

HFONT GetFont(int, int, string);
Image* LoadGdiImage(LPCSTR, LPCSTR, HINSTANCE);

bool ConnectProc(Trainer*, bool);
wstring DeleteUnderline(wstring);
void FillFileString(wstring*, wfstream*);
void ClearData(int*, int);

//STATISTIC FUNCTIONS
void Statistic(int, int, int&, int, vector<Enemy>*, vector<Enemy>*, Trainer*);
wstring GetDate();
void GetMainData(Enemy*, Trainer*);
void GetResult(int, Enemy*, Trainer*);
void CheckWeapon(Enemy*, par, par, par, Trainer*, int, int, int, int);
void CheckArmor(Enemy*, par, par, Trainer*, int, int, int);
void CheckRing(Enemy*, par, Trainer*, int, int);
void GetNewEquip(Enemy*, Trainer*);

//FILE FUNCTIONS
wchar_t* GetFolderPath();
void LoadFile(vector<Enemy>*, vector<Enemy>*, wchar_t*);
void SaveFile(vector<Enemy>*, vector<Enemy>*, wchar_t*);
void FillEquipVector(Enemy&, wstringstream&, par, int);
void ReadFileToStr(vector<Enemy>*, vector<Enemy>*, wstring*);
void WriteFileToStr(vector<Enemy>*, wfstream*);
void encryptStr(wstring*);
void decryptStr(wstring*);
void LoadData(LPCSTR, LPCSTR, HINSTANCE, wstring);

template<class T>
T readValueFromStream(wstringstream& stream);

//DRAW FUNCTIONS
wstring TestHostility(int);
void FillequipPosY(int*);
void DrawPlayerList(HDC, vector<Enemy>*, int, StatisticsType);
void DrawPlayerData(HDC, Enemy*, int*, int);
void DrawRank(HDC, Trainer*);

wstring MakeCountStr(int, int);
wstring MakeWeaponStr(DWORD32, int, int);
wstring MakeArmorStr(DWORD32, int);
wstring MakeRingsStr(DWORD32);
wstring MakeSpellsStr(DWORD32);
wstring MakeConsStr(DWORD32);

//BUTTONS
void CreateListBtn(HWND**, HWND, HINSTANCE);
void ShowListBtn(HWND*, vector<Enemy>*);

void CreateDataBtn(HWND**, HWND, HINSTANCE);
void ShowDataBtn(HWND*, Enemy*, int*, int);
void DetectDataBtn(bool*, Enemy*, int*, int);

void HideBtn(HWND*, int Counts);
void DestroyBtn(HWND*, int Counts);

void ChangeEquipCount(WPARAM, Enemy*, int*, int*, int);