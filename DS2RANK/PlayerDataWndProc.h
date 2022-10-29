#pragma once
#include <Windows.h>
#include "Source.h"
#include "resource.h"

using namespace std;

extern HINSTANCE hInst;
extern vector<Enemy> statistic;
extern vector<Enemy> statisticWorld;
extern StatisticsType StatType;
extern HFONT hfont;

LRESULT CALLBACK PlayerDataWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);