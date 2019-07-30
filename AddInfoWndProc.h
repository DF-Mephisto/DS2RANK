#pragma once

#include <Windows.h>
#include <gdiplus.h>

#include "Source.h"
#include "resource.h"

using namespace std;
using namespace Gdiplus;

extern HINSTANCE hInst;
extern HFONT hfont;
extern Trainer ReadRank;
extern HWND OtherInf;

LRESULT CALLBACK AddInfoWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
