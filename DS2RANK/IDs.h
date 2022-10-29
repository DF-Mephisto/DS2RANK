#pragma once

#include <Windows.h>
#include <map>
#include <string>

using namespace std;

const wstring Inf[10] = {L" ", L"Fire", L"Magic", L"Lightning", L"Dark", L"Poison", L"Bleed", L"Raw", L"Enchanted", L"Mundane"};

extern map<DWORD32, wstring> WeaponID;
extern map<DWORD32, wstring> ArmorID;
extern map<DWORD32, wstring> RingID;
extern map<DWORD32, wstring> SpellID;
extern map<DWORD32, wstring> ConsumableID;

//FUNCTIONS
void FillIDs();
void FillWeaponID();
void FillArmorID();
void FillRingID();
void FillSpellID();
void FillConsumableID();