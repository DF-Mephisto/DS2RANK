#include "Source.h"


HFONT GetFont(int height, int width, string font)
{
	LOGFONT logfont;

	logfont.lfHeight = height;// Устанавливает высоту шрифта или символа
	logfont.lfWidth = 0;// Устанавливает среднюю ширину символов в шрифте
	logfont.lfEscapement = 0;// Устанавливает угол, между вектором наклона и осью X устройства
	logfont.lfOrientation = 0;// Устанавливает угол, между основной линией каждого символа и осью X устройства
	logfont.lfWeight = width;// Устанавливает толщину шрифта в диапазоне от 0 до 1000
	logfont.lfItalic = 0;// Устанавливает курсивный шрифт
	logfont.lfUnderline = 0;// Устанавливает подчеркнутый шрифт
	logfont.lfStrikeOut = 0;// Устанавливает зачеркнутый шрифт
	logfont.lfCharSet = 0;// Устанавливает набор символов
	logfont.lfOutPrecision = 0;// Устанавливает точность вывода
	logfont.lfClipPrecision = 0;// Устанавливает точность отсечения
	logfont.lfQuality = 0;// Устанавливает качество вывода
	logfont.lfPitchAndFamily = 0;// Устанавливает ширину символов и семейство шрифта
	strcpy_s(logfont.lfFaceName, font.length() + 1, font.c_str());//  устанавливает название шрифта

	return CreateFontIndirect(&logfont);
}

Image* LoadGdiImage(LPCSTR Res, LPCSTR type, HINSTANCE hInst)
{
	HRSRC hResInfo;
	HANDLE memRes;
	HGLOBAL hRes;
	DWORD imageSize;
	void* pBuffer;
	HGLOBAL m_hBuffer;
	CComPtr<IStream> stream;
	Image* img;

	hResInfo = FindResource(hInst, Res, type);
	hRes = LoadResource(hInst, hResInfo);
	memRes = LockResource(hRes);
	imageSize = SizeofResource(hInst, hResInfo);
	m_hBuffer = GlobalAlloc(GMEM_MOVEABLE, imageSize);
	pBuffer = GlobalLock(m_hBuffer);
	CopyMemory(pBuffer, memRes, imageSize);
	stream.Attach(SHCreateMemStream((BYTE*)pBuffer, imageSize));
	img = Bitmap::FromStream(stream);

	stream.Release();
	UnlockResource(hRes);
	FreeResource(hRes);
	GlobalUnlock(m_hBuffer);
	GlobalFree(m_hBuffer);

	return img;
}

bool ConnectProc(Trainer* ReadRank, bool msg)
{
	if (ReadRank->GetProcess())
	{
		ReadRank->GetModule();
		if (!ReadRank->OpenProc())
		{
			if (msg) MessageBox(NULL, "Can't open \"Dark Souls II\" process", "Error", MB_OK | MB_ICONINFORMATION);
			return false;
		}
		return true;
	}
	else
	{
		if (msg) MessageBox(NULL, "Can't find \"Dark Souls II\" process", "Error", MB_OK | MB_ICONINFORMATION);
		return false;
	}
}

wstring DeleteUnderline(wstring str)
{
	wstring temp = str;

	for (int i = 0; i < temp.length(); i++) {
		if (temp[i] == L'_') temp[i] = L' ';
	}
	return temp;
}

void FillFileString(wstring* str, wfstream* file)
{
	int endFile;
	file->seekg(0, ios::end);
	endFile = file->tellg();
	file->seekg(0, ios::beg);
	
	wchar_t* buffer = new wchar_t[endFile];
	file->read(buffer, endFile);
	*str = buffer;
	delete[] buffer;
}

void ClearData(int* Data, int Count)
{
	for (int i = 0; i < Count; i++) Data[i] = 0;
}

//STATISTIC FUNCTIONS

void Statistic(int offsetP, int offsetName, int& RCstage, int player, vector<Enemy>* statistic, vector<Enemy>* statisticWorld, Trainer* ReadRank)
{
	static Enemy *cur[PLAYERCOUNT];
	static int Rank = 0;
	static bool Arena = false;

	ReadRank->SetOffsetP(offsetP);
	ReadRank->SetOffsetName(offsetName);

	EnemyType enemyHostil = (EnemyType)ReadRank->GetHostilityEnemy();

	if (RCstage == stage0) {
		if (enemyHostil == errorHostil) return;
		if (enemyHostil == arenaHostil)
		{
			Rank = ReadRank->GetRank(BLOODBRO_offset, BLOODBRO_offset_v);
			Arena = true;
		}

			cur[player] = new Enemy;
			GetMainData(cur[player], ReadRank);

			RCstage = stage1;
			return;
	}

	if (RCstage == stage1) {

			//SAVE DATA

			//IF NOT ARENA
			if (Arena == false && enemyHostil == errorHostil) {
				RCstage = stage0;
				statisticWorld->push_back(*cur[player]);
				delete cur[player];
				return;
			}

			//IF ARENA
			else if(Arena == true && ReadRank->GetMyHostility() == hostHostil)
			{
				GetResult(Rank, cur[player], ReadRank);
				Arena = false;

				RCstage = stage0;
				statistic->push_back(*cur[player]);
				delete cur[player];
				return;
			}

			//TEST ENEMY EQUIP CHANGES
			if (enemyHostil != errorHostil)
			{
				GetNewEquip(cur[player], ReadRank);
			}
	}
}
wstring GetDate()
{
	time_t tim = time(NULL);
	tm tms;
	localtime_s(&tms, &tim);

	wstring day;
	wstring month;
	wstring hour;
	wstring min;

	if (tms.tm_mday < 10) day = L"0" + to_wstring(tms.tm_mday);
	else day = to_wstring(tms.tm_mday);

	if (tms.tm_mon < 9) month = L"0" + to_wstring(tms.tm_mon + 1);
	else month = to_wstring(tms.tm_mon + 1);

	if (tms.tm_hour < 10) hour = L"0" + to_wstring(tms.tm_hour);
	else hour = to_wstring(tms.tm_hour);

	if (tms.tm_min < 10) min = L"0" + to_wstring(tms.tm_min);
	else min = to_wstring(tms.tm_min);

	wstring date = day + L"." + month + L"." + to_wstring(tms.tm_year % 100) + L"___" + hour + L":" + min;
	return date;
}
void GetMainData(Enemy* cur, Trainer* ReadRank)
{
	//NAME AND MISC
	cur->SetStrPar(pNAME, ReadRank->GetEnemyName());
	cur->SetStrPar(pDATE, GetDate());
	cur->SetIntPar(pHOST, ReadRank->GetHostilityEnemy());
	cur->SetIntPar(pRANK, ReadRank->GetRedRank());
	cur->SetIntPar(pLVL, ReadRank->GetEnemyLevel());
	cur->SetIntPar(pHEALTH, ReadRank->GetHealth());
	cur->SetIntPar(pSTM, ReadRank->GetStamina());

	//STATS
	cur->SetIntPar(pVIG, ReadRank->GetByte(VGR_offset, STAT_offset));
	cur->SetIntPar(pEND, ReadRank->GetByte(END_offset, STAT_offset));
	cur->SetIntPar(pVIT, ReadRank->GetByte(VIT_offset, STAT_offset));
	cur->SetIntPar(pATN, ReadRank->GetByte(ATN_offset, STAT_offset));
	cur->SetIntPar(pSTR, ReadRank->GetByte(STR_offset, STAT_offset));
	cur->SetIntPar(pDEX, ReadRank->GetByte(DEX_offset, STAT_offset));
	cur->SetIntPar(pADP, ReadRank->GetByte(ADP_offset, STAT_offset));
	cur->SetIntPar(pINT, ReadRank->GetByte(INT_offset, STAT_offset));
	cur->SetIntPar(pFTH, ReadRank->GetByte(FTH_offset, STAT_offset));

	//WEAPONS
	cur->SetDwordPar(pR1, ReadRank->GetDWORD32(R1_offset, WEP_offset));
	cur->SetDwordPar(pR2, ReadRank->GetDWORD32(R2_offset, WEP_offset));
	cur->SetDwordPar(pR3, ReadRank->GetDWORD32(R3_offset, WEP_offset));
	cur->SetDwordPar(pL1, ReadRank->GetDWORD32(L1_offset, WEP_offset));
	cur->SetDwordPar(pL2, ReadRank->GetDWORD32(L2_offset, WEP_offset));
	cur->SetDwordPar(pL3, ReadRank->GetDWORD32(L3_offset, WEP_offset));

	//WEAPONS ENCHANTMENTS
	cur->SetDwordPar(pR1L, ReadRank->GetByte(R1_ench_offset, WEP_offset));
	cur->SetDwordPar(pR2L, ReadRank->GetByte(R2_ench_offset, WEP_offset));
	cur->SetDwordPar(pR3L, ReadRank->GetByte(R3_ench_offset, WEP_offset));
	cur->SetDwordPar(pL1L, ReadRank->GetByte(L1_ench_offset, WEP_offset));
	cur->SetDwordPar(pL2L, ReadRank->GetByte(L2_ench_offset, WEP_offset));
	cur->SetDwordPar(pL3L, ReadRank->GetByte(L3_ench_offset, WEP_offset));

	//INFUSIONS
	cur->SetDwordPar(pR1I, ReadRank->GetByte(R1_inf_offset, WEP_offset));
	cur->SetDwordPar(pR2I, ReadRank->GetByte(R2_inf_offset, WEP_offset));
	cur->SetDwordPar(pR3I, ReadRank->GetByte(R3_inf_offset, WEP_offset));
	cur->SetDwordPar(pL1I, ReadRank->GetByte(L1_inf_offset, WEP_offset));
	cur->SetDwordPar(pL2I, ReadRank->GetByte(L2_inf_offset, WEP_offset));
	cur->SetDwordPar(pL3I, ReadRank->GetByte(L3_inf_offset, WEP_offset));

	//ARMOR
	cur->SetDwordPar(pHEAD, ReadRank->GetDWORD32(HEAD_offset, ARM_offset));
	cur->SetDwordPar(pBODY, ReadRank->GetDWORD32(BODY_offset, ARM_offset));
	cur->SetDwordPar(pHAND, ReadRank->GetDWORD32(HANDS_offset, ARM_offset));
	cur->SetDwordPar(pLEGS, ReadRank->GetDWORD32(LEGS_offset, ARM_offset));

	//ARMOR ENCHANTMENTS
	cur->SetDwordPar(pHEADL, ReadRank->GetByte(HEAD_ench_offset, ARM_offset));
	cur->SetDwordPar(pBODYL, ReadRank->GetByte(BODY_ench_offset, ARM_offset));
	cur->SetDwordPar(pHANDL, ReadRank->GetByte(HANDS_ench_offset, ARM_offset));
	cur->SetDwordPar(pLEGSL, ReadRank->GetByte(LEGS_ench_offset, ARM_offset));

	//RINGS
	cur->SetDwordPar(pRING1, ReadRank->GetDWORD32(RING1_offset, RING_offset));
	cur->SetDwordPar(pRING2, ReadRank->GetDWORD32(RING2_offset, RING_offset));
	cur->SetDwordPar(pRING3, ReadRank->GetDWORD32(RING3_offset, RING_offset));
	cur->SetDwordPar(pRING4, ReadRank->GetDWORD32(RING4_offset, RING_offset));

	//SPELLS
	DWORD32 sp = SP1_offset;
	for (int i = 0; i < 10; i++)
	{
		cur->SetDwordPar(pSPELLS, ReadRank->GetDWORD32(sp, SPELL_offset), i);
		sp += SPINCR_offset;
	}

	//CONSUMABLES
	DWORD32 cn = CONS1_offset;
	for (int i = 0; i < 10; i++)
	{
		cur->SetDwordPar(pCONS, ReadRank->GetDWORD32(cn, CONS_offset), i);
		cn += CONSINCR_offset;
	}


}
void GetResult(int Rank, Enemy* cur, Trainer* ReadRank)
{
	if (ReadRank->GetRank(BLOODBRO_offset, BLOODBRO_offset_v) > Rank) {
		cur->SetStrPar(pRES, L"Win");
	}
	else if (ReadRank->GetRank(BLOODBRO_offset, BLOODBRO_offset_v) <= Rank) {
		cur->SetStrPar(pRES, L"Loss");
	}
}
void CheckWeapon(Enemy* cur, par wep, par wep_ench, par wep_inf, Trainer* ReadRank, int countID, int WEP_TYPE, int WEP_ENCH_TYPE, int WEP_INF_TYPE)
{
	int count = cur->GetIntPar(pCOUNTS, countID);
	if (cur->GetDwordPar(wep, count - 1) != ReadRank->GetDWORD32(WEP_TYPE, WEP_offset))
	{
		cur->SetDwordPar(wep, ReadRank->GetDWORD32(WEP_TYPE, WEP_offset));
		cur->SetDwordPar(wep_ench, ReadRank->GetByte(WEP_ENCH_TYPE, WEP_offset));
		cur->SetDwordPar(wep_inf, ReadRank->GetByte(WEP_INF_TYPE, WEP_offset));
		cur->SetIntPar(pCOUNTS, count + 1, countID);
	}
}
void CheckArmor(Enemy* cur, par arm, par arm_ench, Trainer* ReadRank, int countID, int ARM_TYPE, int ARM_ENCH_TYPE)
{
	int count = cur->GetIntPar(pCOUNTS, countID);
	if (cur->GetDwordPar(arm, count - 1) != ReadRank->GetDWORD32(ARM_TYPE, ARM_offset))
	{
		cur->SetDwordPar(arm, ReadRank->GetDWORD32(ARM_TYPE, ARM_offset));
		cur->SetDwordPar(arm_ench, ReadRank->GetByte(ARM_ENCH_TYPE, ARM_offset));
		cur->SetIntPar(pCOUNTS, count + 1, countID);
	}
}
void CheckRing(Enemy* cur, par ring, Trainer* ReadRank, int countID, int RING_TYPE)
{
	int count = cur->GetIntPar(pCOUNTS, countID);
	if (cur->GetDwordPar(ring, count - 1) != ReadRank->GetDWORD32(RING_TYPE, RING_offset))
	{
		cur->SetDwordPar(ring, ReadRank->GetDWORD32(RING_TYPE, RING_offset));
		cur->SetIntPar(pCOUNTS, count + 1, countID);
	}
}
void GetNewEquip(Enemy* cur, Trainer* ReadRank)
{
	//WEAPONS
	CheckWeapon(cur, pR1, pR1L, pR1I, ReadRank, 0, R1_offset, R1_ench_offset, R1_inf_offset);
	CheckWeapon(cur, pR2, pR2L, pR2I, ReadRank, 1, R2_offset, R2_ench_offset, R2_inf_offset);
	CheckWeapon(cur, pR3, pR3L, pR3I, ReadRank, 2, R3_offset, R3_ench_offset, R3_inf_offset);
	CheckWeapon(cur, pL1, pL1L, pL1I, ReadRank, 3, L1_offset, L1_ench_offset, L1_inf_offset);
	CheckWeapon(cur, pL2, pL2L, pL2I, ReadRank, 4, L2_offset, L2_ench_offset, L2_inf_offset);
	CheckWeapon(cur, pL3, pL3L, pL3I, ReadRank, 5, L3_offset, L3_ench_offset, L3_inf_offset);

	//ARMOR
	CheckArmor(cur, pHEAD, pHEADL, ReadRank, 6, HEAD_offset, HEAD_ench_offset);
	CheckArmor(cur, pBODY, pBODYL, ReadRank, 7, BODY_offset, BODY_ench_offset);
	CheckArmor(cur, pHAND, pHANDL, ReadRank, 8, HANDS_offset, HANDS_ench_offset);
	CheckArmor(cur, pLEGS, pLEGSL, ReadRank, 9, LEGS_offset, LEGS_ench_offset);

	//RINGS
	CheckRing(cur, pRING1, ReadRank, 10, RING1_offset);
	CheckRing(cur, pRING2, ReadRank, 11, RING2_offset);
	CheckRing(cur, pRING3, ReadRank, 12, RING3_offset);
	CheckRing(cur, pRING4, ReadRank, 13, RING4_offset);
}

//FILE FUNCTIONS
wchar_t* GetFolderPath()
{
	PWSTR MyDoc = new wchar_t[MAX_PATH];
	wchar_t MyDocFolder[MAX_PATH];

	SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_DEFAULT, NULL, &MyDoc);
	wcscpy_s(MyDocFolder, MyDoc);
	wcscat_s(MyDocFolder, L"\\DS2RANK");
	CreateDirectoryW(MyDocFolder, NULL);
	wcscat_s(MyDocFolder, L"\\DS2RANK.ds2r");

	delete[] MyDoc;
	return MyDocFolder;
}

void LoadFile(vector<Enemy>* statistic, vector<Enemy>* statisticWorld, wchar_t* path)
{
	wfstream StatFile;

	StatFile.open(path, ios::in | ios::binary);
	if (StatFile.is_open()) {
		wstring str;
		FillFileString(&str, &StatFile);
		StatFile.close();
		decryptStr(&str);
		ReadFileToStr(statistic, statisticWorld, &str);
	}
}

void SaveFile(vector<Enemy>* statistic, vector<Enemy>* statisticWorld, wchar_t* path)
{
	wfstream StatFile;

	StatFile.open(path, ios::out | ios::binary);
	if (StatFile.is_open()) {
		if (statistic->size()) WriteFileToStr(statistic, &StatFile);
		if (statisticWorld->size())WriteFileToStr(statisticWorld, &StatFile);
		StatFile.close();
	}
}

void FillEquipVector(Enemy& enemy, wstringstream& stream, par Equip, int count)
{
	DWORD32 temp = 0;
	for (int i = 0; i < count; i++) {

		if (!(stream >> temp))
			throw runtime_error("Statistics input error");

		enemy.SetDwordPar(Equip, temp);
	}
}

template<class T>
T readValueFromStream(wstringstream& stream)
{
	T val;
	if (!(stream >> val))
		throw runtime_error("Statistics input error");

	return val;
}

void ReadFileToStr(vector<Enemy>* bloodlist, vector<Enemy>* worldlist, wstring* str)
{

	wstringstream stream(*str);
	
	while (!stream.eof()) {
		try {
			Enemy p;

			//STATS
			p.SetStrPar(pNAME, readValueFromStream<wstring>(stream));
			p.SetIntPar(pLVL, readValueFromStream<int>(stream));
			p.SetStrPar(pRES, readValueFromStream<wstring>(stream));
			p.SetIntPar(pHEALTH, readValueFromStream<int>(stream));
			p.SetIntPar(pSTM, readValueFromStream<int>(stream));
			p.SetIntPar(pVIG, readValueFromStream<int>(stream));
			p.SetIntPar(pEND, readValueFromStream<int>(stream));
			p.SetIntPar(pVIT, readValueFromStream<int>(stream));
			p.SetIntPar(pATN, readValueFromStream<int>(stream));
			p.SetIntPar(pSTR, readValueFromStream<int>(stream));
			p.SetIntPar(pDEX, readValueFromStream<int>(stream));
			p.SetIntPar(pADP, readValueFromStream<int>(stream));
			p.SetIntPar(pINT, readValueFromStream<int>(stream));
			p.SetIntPar(pFTH, readValueFromStream<int>(stream));

			//WEAPONS
			p.SetIntPar(pCOUNTS, readValueFromStream<int>(stream), 0);
			FillEquipVector(p, stream, pR1, p.GetIntPar(pCOUNTS, 0));

			p.SetIntPar(pCOUNTS, readValueFromStream<int>(stream), 1);
			FillEquipVector(p, stream, pR2, p.GetIntPar(pCOUNTS, 1));

			p.SetIntPar(pCOUNTS, readValueFromStream<int>(stream), 2);
			FillEquipVector(p, stream, pR3, p.GetIntPar(pCOUNTS, 2));

			p.SetIntPar(pCOUNTS, readValueFromStream<int>(stream), 3);
			FillEquipVector(p, stream, pL1, p.GetIntPar(pCOUNTS, 3));

			p.SetIntPar(pCOUNTS, readValueFromStream<int>(stream), 4);
			FillEquipVector(p, stream, pL2, p.GetIntPar(pCOUNTS, 4));

			p.SetIntPar(pCOUNTS, readValueFromStream<int>(stream), 5);
			FillEquipVector(p, stream, pL3, p.GetIntPar(pCOUNTS, 5));

			//WEAPONS ENCHANTMENTS
			FillEquipVector(p, stream, pR1L, p.GetIntPar(pCOUNTS, 0));
			FillEquipVector(p, stream, pR2L, p.GetIntPar(pCOUNTS, 1));
			FillEquipVector(p, stream, pR3L, p.GetIntPar(pCOUNTS, 2));
			FillEquipVector(p, stream, pL1L, p.GetIntPar(pCOUNTS, 3));
			FillEquipVector(p, stream, pL2L, p.GetIntPar(pCOUNTS, 4));
			FillEquipVector(p, stream, pL3L, p.GetIntPar(pCOUNTS, 5));

			//INFUSIONS
			FillEquipVector(p, stream, pR1I, p.GetIntPar(pCOUNTS, 0));
			FillEquipVector(p, stream, pR2I, p.GetIntPar(pCOUNTS, 1));
			FillEquipVector(p, stream, pR3I, p.GetIntPar(pCOUNTS, 2));
			FillEquipVector(p, stream, pL1I, p.GetIntPar(pCOUNTS, 3));
			FillEquipVector(p, stream, pL2I, p.GetIntPar(pCOUNTS, 4));
			FillEquipVector(p, stream, pL3I, p.GetIntPar(pCOUNTS, 5));

			//ARMOR
			p.SetIntPar(pCOUNTS, readValueFromStream<int>(stream), 6);
			FillEquipVector(p, stream, pHEAD, p.GetIntPar(pCOUNTS, 6));

			p.SetIntPar(pCOUNTS, readValueFromStream<int>(stream), 7);
			FillEquipVector(p, stream, pBODY, p.GetIntPar(pCOUNTS, 7));

			p.SetIntPar(pCOUNTS, readValueFromStream<int>(stream), 8);
			FillEquipVector(p, stream, pHAND, p.GetIntPar(pCOUNTS, 8));

			p.SetIntPar(pCOUNTS, readValueFromStream<int>(stream), 9);
			FillEquipVector(p, stream, pLEGS, p.GetIntPar(pCOUNTS, 9));

			//ARMOR ENCHANTMENTS
			FillEquipVector(p, stream, pHEADL, p.GetIntPar(pCOUNTS, 6));
			FillEquipVector(p, stream, pBODYL, p.GetIntPar(pCOUNTS, 7));
			FillEquipVector(p, stream, pHANDL, p.GetIntPar(pCOUNTS, 8));
			FillEquipVector(p, stream, pLEGSL, p.GetIntPar(pCOUNTS, 9));

			//RINGS
			p.SetIntPar(pCOUNTS, readValueFromStream<int>(stream), 10);
			FillEquipVector(p, stream, pRING1, p.GetIntPar(pCOUNTS, 10));

			p.SetIntPar(pCOUNTS, readValueFromStream<int>(stream), 11);
			FillEquipVector(p, stream, pRING2, p.GetIntPar(pCOUNTS, 11));

			p.SetIntPar(pCOUNTS, readValueFromStream<int>(stream), 12);
			FillEquipVector(p, stream, pRING3, p.GetIntPar(pCOUNTS, 12));

			p.SetIntPar(pCOUNTS, readValueFromStream<int>(stream), 13);
			FillEquipVector(p, stream, pRING4, p.GetIntPar(pCOUNTS, 13));

			//SPELLS
			for (int j = 0; j < 10; j++)
				p.SetDwordPar(pSPELLS, readValueFromStream<DWORD32>(stream), j);

			//CONSUMABLES
			for (int j = 0; j < 10; j++)
				p.SetDwordPar(pCONS, readValueFromStream<DWORD32>(stream), j);

			//DATE AND MISC
			p.SetStrPar(pDATE, readValueFromStream<wstring>(stream));
			p.SetIntPar(pHOST, readValueFromStream<int>(stream));
			p.SetIntPar(pRANK, readValueFromStream<int>(stream));

			if (p.GetIntPar(pHOST) == 4113) bloodlist->push_back(p);
			else worldlist->push_back(p);
		} 
		catch (runtime_error& e)
		{
			stream.clear();
			wstring ignore;
			getline(stream, ignore);
		}
	}
}
void WriteFileToStr(vector<Enemy>* statistic, wfstream* file)
{
	for (int i = 0; i < statistic->size(); i++)
	{
		wstring str;
		Enemy en = (*statistic)[i];

		//STATS
		str += en.GetStrPar(pNAME); str += '\t';
		str += to_wstring(en.GetIntPar(pLVL)); str += '\t';
		str += en.GetStrPar(pRES); str += '\t';
		str += to_wstring(en.GetIntPar(pHEALTH)); str += '\t';
		str += to_wstring(en.GetIntPar(pSTM)); str += '\t';
		str += to_wstring(en.GetIntPar(pVIG)); str += '\t';
		str += to_wstring(en.GetIntPar(pEND)); str += '\t';
		str += to_wstring(en.GetIntPar(pVIT)); str += '\t';
		str += to_wstring(en.GetIntPar(pATN)); str += '\t';
		str += to_wstring(en.GetIntPar(pSTR)); str += '\t';
		str += to_wstring(en.GetIntPar(pDEX)); str += '\t';
		str += to_wstring(en.GetIntPar(pADP)); str += '\t';
		str += to_wstring(en.GetIntPar(pINT)); str += '\t';
		str += to_wstring(en.GetIntPar(pFTH)); str += '\t';

		//WEAPONS
		str += to_wstring(en.GetIntPar(pCOUNTS, 0)); str += '\t';
		for (int j = 0; j < en.GetIntPar(pCOUNTS, 0); j++) { str += to_wstring(en.GetDwordPar(pR1, j)); str += '\t'; }

		str += to_wstring(en.GetIntPar(pCOUNTS, 1)); str += '\t';
		for (int j = 0; j < en.GetIntPar(pCOUNTS, 1); j++) { str += to_wstring(en.GetDwordPar(pR2, j)); str += '\t'; }

		str += to_wstring(en.GetIntPar(pCOUNTS, 2)); str += '\t';
		for (int j = 0; j < en.GetIntPar(pCOUNTS, 2); j++) { str += to_wstring(en.GetDwordPar(pR3, j)); str += '\t'; }

		str += to_wstring(en.GetIntPar(pCOUNTS, 3)); str += '\t';
		for (int j = 0; j < en.GetIntPar(pCOUNTS, 3); j++) { str += to_wstring(en.GetDwordPar(pL1, j)); str += '\t'; }

		str += to_wstring(en.GetIntPar(pCOUNTS, 4)); str += '\t';
		for (int j = 0; j < en.GetIntPar(pCOUNTS, 4); j++) { str += to_wstring(en.GetDwordPar(pL2, j)); str += '\t'; }

		str += to_wstring(en.GetIntPar(pCOUNTS, 5)); str += '\t';
		for (int j = 0; j < en.GetIntPar(pCOUNTS, 5); j++) { str += to_wstring(en.GetDwordPar(pL3, j)); str += '\t'; }

		//WEAPONS ENCHANTMENTS
		for (int j = 0; j < en.GetIntPar(pCOUNTS, 0); j++) { str += to_wstring(en.GetDwordPar(pR1L, j)); str += '\t'; }
		for (int j = 0; j < en.GetIntPar(pCOUNTS, 1); j++) { str += to_wstring(en.GetDwordPar(pR2L, j)); str += '\t'; }
		for (int j = 0; j < en.GetIntPar(pCOUNTS, 2); j++) { str += to_wstring(en.GetDwordPar(pR3L, j)); str += '\t'; }
		for (int j = 0; j < en.GetIntPar(pCOUNTS, 3); j++) { str += to_wstring(en.GetDwordPar(pL1L, j)); str += '\t'; }
		for (int j = 0; j < en.GetIntPar(pCOUNTS, 4); j++) { str += to_wstring(en.GetDwordPar(pL2L, j)); str += '\t'; }
		for (int j = 0; j < en.GetIntPar(pCOUNTS, 5); j++) { str += to_wstring(en.GetDwordPar(pL3L, j)); str += '\t'; }

		//INFUSIONS
		for (int j = 0; j < en.GetIntPar(pCOUNTS, 0); j++) { str += to_wstring(en.GetDwordPar(pR1I, j)); str += '\t'; }
		for (int j = 0; j < en.GetIntPar(pCOUNTS, 1); j++) { str += to_wstring(en.GetDwordPar(pR2I, j)); str += '\t'; }
		for (int j = 0; j < en.GetIntPar(pCOUNTS, 2); j++) { str += to_wstring(en.GetDwordPar(pR3I, j)); str += '\t'; }
		for (int j = 0; j < en.GetIntPar(pCOUNTS, 3); j++) { str += to_wstring(en.GetDwordPar(pL1I, j)); str += '\t'; }
		for (int j = 0; j < en.GetIntPar(pCOUNTS, 4); j++) { str += to_wstring(en.GetDwordPar(pL2I, j)); str += '\t'; }
		for (int j = 0; j < en.GetIntPar(pCOUNTS, 5); j++) { str += to_wstring(en.GetDwordPar(pL3I, j)); str += '\t'; }

		//ARMOR
		str += to_wstring(en.GetIntPar(pCOUNTS, 6)); str += '\t';
		for (int j = 0; j < en.GetIntPar(pCOUNTS, 6); j++) { str += to_wstring(en.GetDwordPar(pHEAD, j)); str += '\t'; }

		str += to_wstring(en.GetIntPar(pCOUNTS, 7)); str += '\t';
		for (int j = 0; j < en.GetIntPar(pCOUNTS, 7); j++) { str += to_wstring(en.GetDwordPar(pBODY, j)); str += '\t'; }

		str += to_wstring(en.GetIntPar(pCOUNTS, 8)); str += '\t';
		for (int j = 0; j < en.GetIntPar(pCOUNTS, 8); j++) { str += to_wstring(en.GetDwordPar(pHAND, j)); str += '\t'; }

		str += to_wstring(en.GetIntPar(pCOUNTS, 9)); str += '\t';
		for (int j = 0; j < en.GetIntPar(pCOUNTS, 9); j++) { str += to_wstring(en.GetDwordPar(pLEGS, j)); str += '\t'; }

		//ARMOR ENCHANTMENTS
		for (int j = 0; j < en.GetIntPar(pCOUNTS, 6); j++) { str += to_wstring(en.GetDwordPar(pHEADL, j)); str += '\t'; }
		for (int j = 0; j < en.GetIntPar(pCOUNTS, 7); j++) { str += to_wstring(en.GetDwordPar(pBODYL, j)); str += '\t'; }
		for (int j = 0; j < en.GetIntPar(pCOUNTS, 8); j++) { str += to_wstring(en.GetDwordPar(pHANDL, j)); str += '\t'; }
		for (int j = 0; j < en.GetIntPar(pCOUNTS, 9); j++) { str += to_wstring(en.GetDwordPar(pLEGSL, j)); str += '\t'; }

		//RINGS
		str += to_wstring(en.GetIntPar(pCOUNTS, 10)); str += '\t';
		for (int j = 0; j < en.GetIntPar(pCOUNTS, 10); j++) { str += to_wstring(en.GetDwordPar(pRING1, j)); str += '\t'; }

		str += to_wstring(en.GetIntPar(pCOUNTS, 11)); str += '\t';
		for (int j = 0; j < en.GetIntPar(pCOUNTS, 11); j++) { str += to_wstring(en.GetDwordPar(pRING2, j)); str += '\t'; }

		str += to_wstring(en.GetIntPar(pCOUNTS, 12)); str += '\t';
		for (int j = 0; j < en.GetIntPar(pCOUNTS, 12); j++) { str += to_wstring(en.GetDwordPar(pRING3, j)); str += '\t'; }

		str += to_wstring(en.GetIntPar(pCOUNTS, 13)); str += '\t';
		for (int j = 0; j < en.GetIntPar(pCOUNTS, 13); j++) { str += to_wstring(en.GetDwordPar(pRING4, j)); str += '\t'; }

		//SPELLS
		for (int j = 0; j < 10; j++)
		{
			str += to_wstring(en.GetDwordPar(pSPELLS, j));
			str += '\t';
		}

		//CONSUMABLES
		for (int j = 0; j < 10; j++)
		{
			str += to_wstring(en.GetDwordPar(pCONS, j));
			str += '\t';
		}

		//DATE AND MISC
		str += en.GetStrPar(pDATE); str += '\t';
		str += to_wstring(en.GetIntPar(pHOST)); str += '\t';
		str += to_wstring(en.GetIntPar(pRANK)); str += '\n';

		encryptStr(&str);
		file->write(str.c_str(), str.length());
	}
}
void encryptStr(wstring* str)
{
	for (int i = 0; i < str->length(); i++)
	{
		(*str)[i] += 0x21;
	}
}
void decryptStr(wstring* str)
{
	for (int i = 0; i < str->length(); i++)
	{
		(*str)[i] -= 0x21;
	}
}

void LoadData(LPCSTR Res, LPCSTR type, HINSTANCE hInst, wstring name)
{
	PWSTR MyDoc = new wchar_t[MAX_PATH];
	wchar_t path[MAX_PATH];

	SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_DEFAULT, NULL, &MyDoc);
	wcscpy_s(path, MyDoc);
	wcscat_s(path, L"\\DS2RANK");
	//wcscat_s(FontVS, L"\\name.vs");
	wcscat_s(path, name.c_str());

	delete[] MyDoc;
	
	//Load Res
	HRSRC hResInfo;
	HANDLE memRes;
	HGLOBAL hRes;
	DWORD resSize;
	void* pBuffer;
	HGLOBAL m_hBuffer;

	hResInfo = FindResource(hInst, Res, type);
	hRes = LoadResource(hInst, hResInfo);
	memRes = LockResource(hRes);
	resSize = SizeofResource(hInst, hResInfo);
	m_hBuffer = GlobalAlloc(GMEM_MOVEABLE, resSize);
	pBuffer = GlobalLock(m_hBuffer);
	CopyMemory(pBuffer, memRes, resSize);

	//Write Res
	fstream StatFile;
	StatFile.open(path, ios::out | ios::binary);
	if (StatFile.is_open()) {
		StatFile.write((char*)pBuffer, resSize);
		StatFile.close();
	}	

	//Release Res
	UnlockResource(hRes);
	FreeResource(hRes);
	GlobalUnlock(m_hBuffer);
	GlobalFree(m_hBuffer);
}

//DRAW FUNCTIONS
wstring TestHostility(int hostility) {
	wstring host = L"Unknown";
	switch (hostility) {
	case 0:
		host = L"Host";
		break;
	case 513:
		host = L"White Phantom";
		break;
	case 514:
		host = L"Shade";
		break;
	case 515:
		host = L"SunBro";
		break;
	case 516:
		host = L"Shade SunBro";
		break;
	case 773: case 1286: case 1287:
		host = L"Arbiter Spirit";
		break;
	case 1799: case 1800: case 1801: case 1802: case 1803:
		host = L"Dark Spirit";
		break;
	case 2315:
		host = L"Gray Spirit";
		break;
	case 3085:
		host = L"Dragon Spirit";
		break;
	case 4112:
		host = L"Blue Arena Spirit";
		break;
	case 4113:
		host = L"Red Arena Spirit";
		break;
	}
	
	return host;
}
void FillequipPosY(int* equipPosY)
{
	//WEAPONS
	equipPosY[0] = 18;
	equipPosY[1] = 19;
	equipPosY[2] = 20;
	equipPosY[3] = 21;
	equipPosY[4] = 22;
	equipPosY[5] = 23;

	//ARMOR
	equipPosY[6] = 25;
	equipPosY[7] = 26;
	equipPosY[8] = 27;
	equipPosY[9] = 28;

	//RINGS
	equipPosY[10] = 30;
	equipPosY[11] = 31;
	equipPosY[12] = 32;
	equipPosY[13] = 33;
}
void DrawPlayerList(HDC hDC, vector<Enemy>* stats, int scrollPosY, StatisticsType StatType)
{
	int cY = Y_START_POS;
	int MaxLine = stats->size() > ENTRYCOUNT ? scrollPosY + ENTRYCOUNT : stats->size();

	for (int i = scrollPosY; i < MaxLine; i++) 
	{
		wstring name = DeleteUnderline((*stats)[i].GetStrPar(pNAME));

		TextOutW(hDC, X_NAME_POS, cY, name.c_str(), name.length());
		TextOutW(hDC, X_LEVEL_POS, cY, to_wstring((*stats)[i].GetIntPar(pLVL)).c_str(), to_wstring((*stats)[i].GetIntPar(pLVL)).length());

		if (StatType == bloodStat) TextOutW(hDC, X_RES_POS, cY, (*stats)[i].GetStrPar(pRES).c_str(), (*stats)[i].GetStrPar(pRES).length());
		else if (StatType == worldStat) TextOutW(hDC, X_HOST_POS, cY, TestHostility((*stats)[i].GetIntPar(pHOST)).c_str(), TestHostility((*stats)[i].GetIntPar(pHOST)).length());

		cY += Y_OFFSET;
	}
}
void DrawPlayerData(HDC hDC, Enemy* target, int* Counts, int scrollPosY) {
	playerData Data[STATCOUNT] =
	{
		//NAME AND MISC
		{ label, DeleteUnderline(target->GetStrPar(pNAME)), L"None", L"None" }, //NAME
		{ sublabel, DeleteUnderline(target->GetStrPar(pDATE)), L"None", L"None" }, //DATE
		{ parameter, L"BloodBro Rank",	L"None",	to_wstring(target->GetIntPar(pRANK)) }, //RANK
		{ parameter, L"Hostility",		L"None",	TestHostility(target->GetIntPar(pHOST)) }, //HOSTILITY
		{ parameter, L"Level",			L"None",	to_wstring(target->GetIntPar(pLVL)) }, //LEVEL
		{ parameter, L"Health",			L"None",	to_wstring(target->GetIntPar(pHEALTH)) }, //HEALTH
		{ parameter, L"Stamina",		L"None",	to_wstring(target->GetIntPar(pSTM)) }, //STAMINA

		//STATS
		{ label, L"Stats", L"None" },
		{ parameter, L"Vigor",			L"None",	to_wstring(target->GetIntPar(pVIG)) },
		{ parameter, L"Endurance",		L"None",	to_wstring(target->GetIntPar(pEND)) },
		{ parameter, L"Vitality",		L"None",	to_wstring(target->GetIntPar(pVIT)) },
		{ parameter, L"Attunement",		L"None",	to_wstring(target->GetIntPar(pATN)) },
		{ parameter, L"Strength",		L"None",	to_wstring(target->GetIntPar(pSTR)) },
		{ parameter, L"Dexterity",		L"None",	to_wstring(target->GetIntPar(pDEX)) },
		{ parameter, L"Adaptability",	L"None",	to_wstring(target->GetIntPar(pADP)) },
		{ parameter, L"Intelligence",	L"None",	to_wstring(target->GetIntPar(pINT)) },
		{ parameter, L"Faith",			L"None",	to_wstring(target->GetIntPar(pFTH)) },
		
		//WEAPONS
		{ label, L"Weapons", L"None", L"None" },
		{ equip, L"R1",				MakeCountStr(target->GetIntPar(pCOUNTS, 0), Counts[0]),	MakeWeaponStr(target->GetDwordPar(pR1, Counts[0]), target->GetDwordPar(pR1L, Counts[0]), target->GetDwordPar(pR1I, Counts[0])) },
		{ equip, L"R2",				MakeCountStr(target->GetIntPar(pCOUNTS, 1), Counts[1]),	MakeWeaponStr(target->GetDwordPar(pR2, Counts[1]), target->GetDwordPar(pR2L, Counts[1]), target->GetDwordPar(pR2I, Counts[1])) },
		{ equip, L"R3",				MakeCountStr(target->GetIntPar(pCOUNTS, 2), Counts[2]),	MakeWeaponStr(target->GetDwordPar(pR3, Counts[2]), target->GetDwordPar(pR3L, Counts[2]), target->GetDwordPar(pR3I, Counts[2])) },
		{ equip, L"L1",				MakeCountStr(target->GetIntPar(pCOUNTS, 3), Counts[3]),	MakeWeaponStr(target->GetDwordPar(pL1, Counts[3]), target->GetDwordPar(pL1L, Counts[3]), target->GetDwordPar(pL1I, Counts[3])) },
		{ equip, L"L2",				MakeCountStr(target->GetIntPar(pCOUNTS, 4), Counts[4]),	MakeWeaponStr(target->GetDwordPar(pL2, Counts[4]), target->GetDwordPar(pL2L, Counts[4]), target->GetDwordPar(pL2I, Counts[4])) },
		{ equip, L"L3",				MakeCountStr(target->GetIntPar(pCOUNTS, 5), Counts[5]),	MakeWeaponStr(target->GetDwordPar(pL3, Counts[5]), target->GetDwordPar(pL3L, Counts[5]), target->GetDwordPar(pL3I, Counts[5])) },

		//ARMOR
		{ label, L"Armor", L"None", L"None" },
		{ equip, L"Head",			MakeCountStr(target->GetIntPar(pCOUNTS, 6), Counts[6]),	MakeArmorStr(target->GetDwordPar(pHEAD, Counts[6]), target->GetDwordPar(pHEADL, Counts[6])) },
		{ equip, L"Body",			MakeCountStr(target->GetIntPar(pCOUNTS, 7), Counts[7]),	MakeArmorStr(target->GetDwordPar(pBODY, Counts[7]), target->GetDwordPar(pBODYL, Counts[7])) },
		{ equip, L"Hands",			MakeCountStr(target->GetIntPar(pCOUNTS, 8), Counts[8]),	MakeArmorStr(target->GetDwordPar(pHAND, Counts[8]), target->GetDwordPar(pHANDL, Counts[8])) },
		{ equip, L"Legs",			MakeCountStr(target->GetIntPar(pCOUNTS, 9), Counts[9]),	MakeArmorStr(target->GetDwordPar(pLEGS, Counts[9]), target->GetDwordPar(pLEGSL, Counts[9])) },

		//RINGS
		{ label, L"Rings", L"None", L"None" },
		{ equip, L"Ring1",			MakeCountStr(target->GetIntPar(pCOUNTS, 10), Counts[10]),	MakeRingsStr(target->GetDwordPar(pRING1, Counts[10])) },
		{ equip, L"Ring2",			MakeCountStr(target->GetIntPar(pCOUNTS, 11), Counts[11]),	MakeRingsStr(target->GetDwordPar(pRING2, Counts[11])) },
		{ equip, L"Ring3",			MakeCountStr(target->GetIntPar(pCOUNTS, 12), Counts[12]),	MakeRingsStr(target->GetDwordPar(pRING3, Counts[12])) },
		{ equip, L"Ring4",			MakeCountStr(target->GetIntPar(pCOUNTS, 13), Counts[13]),	MakeRingsStr(target->GetDwordPar(pRING4, Counts[13])) },

		//SPELLS
		{ label, L"Spells", L"None", L"None" },
		{ parameter, L"Slot 1",		L"None",	MakeSpellsStr(target->GetDwordPar(pSPELLS, 0)) },
		{ parameter, L"Slot 2",		L"None",	MakeSpellsStr(target->GetDwordPar(pSPELLS, 1)) },
		{ parameter, L"Slot 3",		L"None",	MakeSpellsStr(target->GetDwordPar(pSPELLS, 2)) },
		{ parameter, L"Slot 4",		L"None",	MakeSpellsStr(target->GetDwordPar(pSPELLS, 3)) },
		{ parameter, L"Slot 5",		L"None",	MakeSpellsStr(target->GetDwordPar(pSPELLS, 4)) },
		{ parameter, L"Slot 6",		L"None",	MakeSpellsStr(target->GetDwordPar(pSPELLS, 5)) },
		{ parameter, L"Slot 7",		L"None",	MakeSpellsStr(target->GetDwordPar(pSPELLS, 6)) },
		{ parameter, L"Slot 8",		L"None",	MakeSpellsStr(target->GetDwordPar(pSPELLS, 7)) },
		{ parameter, L"Slot 9",		L"None",	MakeSpellsStr(target->GetDwordPar(pSPELLS, 8)) },
		{ parameter, L"Slot 10",	L"None",	MakeSpellsStr(target->GetDwordPar(pSPELLS, 9)) },

		//CONSUMABLES
		{ label, L"Consumables", L"None", L"None" },
		{ parameter, L"Slot 1",		L"None",	MakeConsStr(target->GetDwordPar(pCONS, 0)) },
		{ parameter, L"Slot 2",		L"None",	MakeConsStr(target->GetDwordPar(pCONS, 1)) },
		{ parameter, L"Slot 3",		L"None",	MakeConsStr(target->GetDwordPar(pCONS, 2)) },
		{ parameter, L"Slot 4",		L"None",	MakeConsStr(target->GetDwordPar(pCONS, 3)) },
		{ parameter, L"Slot 5",		L"None",	MakeConsStr(target->GetDwordPar(pCONS, 4)) },
		{ parameter, L"Slot 6",		L"None",	MakeConsStr(target->GetDwordPar(pCONS, 5)) },
		{ parameter, L"Slot 7",		L"None",	MakeConsStr(target->GetDwordPar(pCONS, 6)) },
		{ parameter, L"Slot 8",		L"None",	MakeConsStr(target->GetDwordPar(pCONS, 7)) },
		{ parameter, L"Slot 9",		L"None",	MakeConsStr(target->GetDwordPar(pCONS, 8)) },
		{ parameter, L"Slot 10",	L"None",	MakeConsStr(target->GetDwordPar(pCONS, 9)) }
	};

	int cY = Y_START_POS;
	for (int i = scrollPosY; i < scrollPosY + ENTRYCOUNT; i++)
	{
		switch (Data[i].type)
		{
		case label:
		{
			SetTextColor(hDC, RGB(0x00, 0xFF, 0x99));
			SetTextAlign(hDC, TA_CENTER);
			TextOutW(hDC, X_DATA_LABEL, cY, Data[i].name.c_str(), Data[i].name.length());
			SetTextAlign(hDC, TA_LEFT);
			SetTextColor(hDC, RGB(0xE0, 0xE0, 0xE0));

			goto exit;
		}

		case sublabel:
		{
			SetTextAlign(hDC, TA_CENTER);
			TextOutW(hDC, X_DATA_LABEL, cY, Data[i].name.c_str(), Data[i].name.length());
			SetTextAlign(hDC, TA_LEFT);

			goto exit;
		}

		case parameter:
		{
			TextOutW(hDC, X_DATA_NAME, cY, Data[i].name.c_str(), Data[i].name.length());
			TextOutW(hDC, X_DATA_PAR, cY, Data[i].parameter.c_str(), Data[i].parameter.length());

			goto exit;
		}

		case equip:
		{
			TextOutW(hDC, X_DATA_NAME, cY, Data[i].name.c_str(), Data[i].name.length());
			TextOutW(hDC, X_DATA_PAR, cY, Data[i].parameter.c_str(), Data[i].parameter.length());

			if (Data[i].count != L"-") {
				SetTextAlign(hDC, TA_CENTER);
				TextOutW(hDC, X_DATA_COUNT, cY, Data[i].count.c_str(), Data[i].count.length());
				SetTextAlign(hDC, TA_LEFT);
			}

			goto exit;
		}

		exit:
		cY += Y_OFFSET;
		}
	}
}
void DrawRank(HDC hCompatibleDC, Trainer* ReadRank)
{
	SetTextAlign(hCompatibleDC, TA_CENTER);
	string Rank = to_string(ReadRank->GetRank(BLOODBRO_offset, BLOODBRO_offset_v));
	TextOut(hCompatibleDC, 136, 190, Rank.c_str(), Rank.length());

	Rank = to_string(ReadRank->GetRank(SENTINELS_offset, SENTINELS_offset_v));
	TextOut(hCompatibleDC, 294, 190, Rank.c_str(), Rank.length());

	Rank = to_string(ReadRank->GetRank(DRAGON_offset, DRAGON_offset_v));
	TextOut(hCompatibleDC, 451, 190, Rank.c_str(), Rank.length());

	Rank = to_string(ReadRank->GetRank(BELLBONGS_offset, BELLBONGS_offset_v));
	TextOut(hCompatibleDC, 609, 190, Rank.c_str(), Rank.length());
	SetTextAlign(hCompatibleDC, TA_LEFT);
}
wstring MakeCountStr(int tCount, int Count)
{
	if (tCount == 1) return L"-";
	else return to_wstring(Count + 1);
}
wstring MakeWeaponStr(DWORD32 weapon, int level, int infusion)
{
	map<DWORD32, wstring>::iterator p;
	p = WeaponID.find(weapon);
	if (p != WeaponID.end()) {
		wstring wep = p->second;
		wstring lev = level ? L"+" + to_wstring(level) : L"";
		wstring inf = infusion ? Inf[infusion] : L"";
		wep += (L" " + lev + L" " + inf);
		return wep;
	}
	return L"-";
}
wstring MakeArmorStr(DWORD32 armor, int level)
{
	map<DWORD32, wstring>::iterator p;
	p = ArmorID.find(armor);
	if (p != ArmorID.end()) {
		wstring armor = p->second;
		wstring lev = level ? L"+" + to_wstring(level) : L"";
		armor += (L" " + lev);
		return armor;
	}
	return L"-";
}
wstring MakeRingsStr(DWORD32 ring)
{
	map<DWORD32, wstring>::iterator p;
	p = RingID.find(ring);
	if (p != RingID.end()) {
		return p->second;
	}
	return L"-";
}
wstring MakeSpellsStr(DWORD32 spell)
{
	map<DWORD32, wstring>::iterator p;
	p = SpellID.find(spell);
	if (p != SpellID.end()) {
		return p->second;
	}
	return L"-";
}
wstring MakeConsStr(DWORD32 cons)
{
	map<DWORD32, wstring>::iterator p;
	p = ConsumableID.find(cons);
	if (p != ConsumableID.end()) {
		return p->second;
	}
	return L"-";
}

//BUTTONS
void CreateListBtn(HWND** ListBtn, HWND hWnd, HINSTANCE hInst)
{
	*ListBtn = new HWND[ENTRYCOUNT];
	int cY = Y_BTN_START_POS;

	for (int i = 0; i < ENTRYCOUNT; i++)
	{
		(*ListBtn)[i] = CreateWindow("button", NULL, WS_CHILD | BS_PUSHBUTTON, X_LIST_BTN, cY, XLISTBTN_SZ, YLISTBTN_SZ, hWnd, (HMENU)i, hInst, NULL);
		cY += Y_OFFSET;
	}
}
void ShowListBtn(HWND* ListBtn, vector<Enemy>*stats)
{
	int listSize = stats->size();
	for (int i = 0; i < (listSize >= ENTRYCOUNT ? ENTRYCOUNT : listSize); i++)
	{
		ShowWindow(ListBtn[i], SW_SHOW);
	}
}

void CreateDataBtn(HWND** DataBtn, HWND hWnd, HINSTANCE hInst)
{
	*DataBtn = new HWND[ENTRYCOUNT * 2];
	int cY = Y_BTN_START_POS;

	for (int i = 0; i < ENTRYCOUNT * 2; i += 2) {
		(*DataBtn)[i] = CreateWindow("button", NULL, WS_CHILD | BS_PUSHBUTTON, X_LDATA_BTN, cY, XDATA_SZ, YDATA_SZ, hWnd, (HMENU)(ENTRYCOUNT + i), hInst, NULL);
		(*DataBtn)[i + 1] = CreateWindow("button", NULL, WS_CHILD | BS_PUSHBUTTON, X_RDATA_BTN, cY, XDATA_SZ, YDATA_SZ, hWnd, (HMENU)(ENTRYCOUNT + i + 1), hInst, NULL);
		cY += Y_OFFSET;
	}
}
void ShowDataBtn(HWND* DataBtn, Enemy* target, int* equipPosY, int scrollPosY)
{
	bool DataBtnShow[ENTRYCOUNT * 2];
	for (int i = 0; i < ENTRYCOUNT * 2; i++) DataBtnShow[i] = false;

	DetectDataBtn(DataBtnShow, target, equipPosY, scrollPosY);
	for (int i = 0; i < ENTRYCOUNT * 2; i++)
	{
		if (DataBtnShow[i] == true) ShowWindow(DataBtn[i], SW_SHOW);
		else ShowWindow(DataBtn[i], SW_HIDE);
	}
}
void DetectDataBtn(bool* DataBtn, Enemy* target, int* equipPosY, int scrollPosY)
{
	for (int i = 0; i < EQUIPCOUNT; i++)
	{
		if (target->GetIntPar(pCOUNTS, i) > 1)
		{
			int cY = equipPosY[i] - scrollPosY;
			if (cY < ENTRYCOUNT && cY >= 0)
			{
				DataBtn[cY * 2] = true;
				DataBtn[cY * 2 + 1] = true;
			}
		}
	}
}

void HideBtn(HWND* Buttons, int Counts)
{
	for (int i = 0; i < Counts; i++)
	{
		ShowWindow(Buttons[i], SW_HIDE);
	}
}
void DestroyBtn(HWND* Buttons, int Counts)
{
	for (int i = 0; i < Counts; i++)
	{
		DestroyWindow(Buttons[i]);
	}
	delete[] Buttons;
}

void ChangeEquipCount(WPARAM wParam, Enemy* target, int* equipPosY, int* Counts, int scrollPosY)
{
	int clearID = LOWORD(wParam) - ENTRYCOUNT;
	int direct = clearID % 2;
	int entryPos = (clearID / 2) + scrollPosY;

	for (int i = 0; i < EQUIPCOUNT; i++)
	{
		if (entryPos == equipPosY[i])
		{
			if (direct == 0 && Counts[i] > 0) Counts[i]--;
			if (direct != 0 && Counts[i] < target->GetIntPar(pCOUNTS, i) - 1) Counts[i]++;
		}
	}
}

//Template functions
template wstring readValueFromStream<wstring>(wstringstream& stream);
template DWORD32 readValueFromStream<DWORD32>(wstringstream& stream);
template int readValueFromStream<int>(wstringstream& stream);