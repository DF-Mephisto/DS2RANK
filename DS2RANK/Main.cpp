#pragma once

#include "AddInfoWndProc.h"
#include "PlayerDataWndProc.h"
#include "MemReader.h"
#include "resource.h"
#include "Source.h"
#include "IDs.h"


#pragma comment (lib,"Gdiplus.lib")
#pragma comment(linker,"/manifestdependency:\"type='win32' \
                        name='Microsoft.Windows.Common-Controls' \
                        version='6.0.0.0' processorArchitecture='*'\
						publicKeyToken='6595b64144ccf1df' language='*'\"")

using namespace std;
using namespace Gdiplus;

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

HINSTANCE hInst;
Trainer ReadRank;

vector<Enemy> statistic;
vector<Enemy> statisticWorld;

HWND OtherInf;
HFONT hfont;

StatisticsType StatType;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ULONG_PTR token;
	GdiplusStartupInput GpIn;
	GdiplusStartupOutput GpOut;
	GdiplusStartup(&token, &GpIn, &GpOut);
	
	hInst = hInstance;

	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hIconSm = (HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);
	wc.lpszClassName = "MainWndClass";
	RegisterClassEx(&wc);

	wc.cbWndExtra = DLGWINDOWEXTRA;
	wc.lpfnWndProc = AddInfoWndProc;
	wc.lpszClassName = "AddInfoClass";
	RegisterClassEx(&wc);

	wc.cbWndExtra = 0;
	wc.lpfnWndProc = PlayerDataWndProc;
	wc.hIconSm = NULL;
	wc.lpszClassName = "PlayerDataWndClass";
	RegisterClassEx(&wc);

	HWND hWnd = CreateWindow("MainWndClass", "DS2RANK v.4.3", WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN, (GetSystemMetrics(SM_CXSCREEN) / 2) - 300, (GetSystemMetrics(SM_CYSCREEN) / 2) - 300, 764, 688, 0, 0, hInstance, 0);
	ShowWindow(hWnd, nCmdShow);

	MSG msg;

	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	GdiplusShutdown(token);
	return (int)msg.wParam;
}

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC				hDC;
	HDC				hCompatibleDC;
	RECT			Rect;
	PAINTSTRUCT		PaintStruct;
	POINT			pt;

	static bool		recordStage;
	static HWND		DataWnd;
	static HWND		StartBTN, BloodBTN, WorldBTN, AdditionBTN;
	static HBITMAP	hBmp;
	static Image	*procON, *procOFF, *procAct;
	static Image	*BGON, *BGOFF, *BGCUR;
	static wchar_t  MyDocFolder[MAX_PATH];

	static int*		RCstage; 


	switch (message)
	{
	case WM_CREATE:
	{
					  FillIDs();
					  OtherInf = 0;
					  StatType = emptyStat;

					  RCstage = new int[PLAYERCOUNT];
					  ClearData(RCstage, PLAYERCOUNT);
					  recordStage = false;

					  procON = LoadGdiImage(MAKEINTRESOURCE(IDB_PNG1), "PNG", hInst);					 
					  procOFF = LoadGdiImage(MAKEINTRESOURCE(IDB_PNG2), "PNG", hInst);
					  if (ConnectProc(&ReadRank, false)) procAct = procON;
					  else procAct = procOFF;

					  BGON = LoadGdiImage(MAKEINTRESOURCE(IDB_BGON), "JPG", hInst);
					  BGOFF = LoadGdiImage(MAKEINTRESOURCE(IDB_BGOFF), "JPG", hInst);
					  BGCUR = BGOFF;

					  StartBTN = CreateWindow("button", "Start", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 523, 273, 120, 50, hWnd, (HMENU)ID_START, hInst, NULL);
					  BloodBTN = CreateWindow("button", "BloodBro Statistic", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 523, 370, 176, 50, hWnd, (HMENU)ID_BLOODSTAT, hInst, NULL);
					  WorldBTN = CreateWindow("button", "World Statistic", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 523, 430, 176, 50, hWnd, (HMENU)ID_WORLDSTAT, hInst, NULL);
					  AdditionBTN = CreateWindow("button", ">", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 707, 370, 30, 110, hWnd, (HMENU)ID_ADDITION, hInst, NULL);

					  hfont = GetFont(20, 100, "Arial");

					  SendMessage(StartBTN, WM_SETFONT, (WPARAM)hfont, 0);
					  SendMessage(BloodBTN, WM_SETFONT, (WPARAM)hfont, 0);
					  SendMessage(WorldBTN, WM_SETFONT, (WPARAM)hfont, 0);
					  SendMessage(AdditionBTN, WM_SETFONT, (WPARAM)hfont, 0);

					  DataWnd = CreateWindow("PlayerDataWndClass", NULL, WS_CHILDWINDOW | WS_VISIBLE | WS_DLGFRAME | WS_SYSMENU | WS_VSCROLL | WS_CLIPCHILDREN, 10, 273, 500, 364, hWnd, (HMENU)ID_STATWINDOW, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
					  
					  wcscpy_s(MyDocFolder, GetFolderPath());
					  LoadFile(&statistic, &statisticWorld, MyDocFolder);
					  LoadData(MAKEINTRESOURCE(IDRES_DDS), "RES", hInst, L"\\font.dds");
					  LoadData(MAKEINTRESOURCE(IDRES_VS), "RES", hInst, L"\\font.vs");
					  LoadData(MAKEINTRESOURCE(IDRES_PS), "RES", hInst, L"\\font.ps");
					  LoadData(MAKEINTRESOURCE(IDRES_DATA), "RES", hInst, L"\\fontdata.txt");
					  LoadData(MAKEINTRESOURCE(IDRES_DLL), "RES", hInst, L"\\DS2Overlay.dll");
	
					  return 0;
	}

	case WM_PAINT:
	{
					 GetWindowRect(hWnd, &Rect);
					 pt.x = Rect.right;
					 pt.y = Rect.bottom;
					 ScreenToClient(hWnd, &pt);

					 hDC = BeginPaint(hWnd, &PaintStruct);
					 hCompatibleDC = CreateCompatibleDC(hDC);
					 hBmp = CreateCompatibleBitmap(hDC, pt.x, pt.y);
					 SelectObject(hCompatibleDC, hBmp);
					 PatBlt(hCompatibleDC, 0, 0, pt.x, pt.y, PATCOPY);
					 SelectObject(hCompatibleDC, hfont);	

					 Graphics graph(hCompatibleDC);
					 graph.SetSmoothingMode(SmoothingModeNone);
					 
					 graph.DrawImage(BGCUR, 0, 0, 750, 650);
					 graph.DrawImage(procAct, 720, 253, 20, 20);

					 DrawRank(hCompatibleDC, &ReadRank);
					 
					 BitBlt(hDC, 0, 0, pt.x, pt.y, hCompatibleDC, 0, 0, SRCCOPY);
					 DeleteObject(hBmp);
					 DeleteDC(hCompatibleDC);
					 EndPaint(hWnd, &PaintStruct);
					 return 0;
	}

	case WM_COMMAND:
	{
					   switch (LOWORD(wParam)){
						case ID_START:
						{
							   if (recordStage) {								   
								   recordStage = false;
								   BGCUR = BGOFF;
								   SetWindowText(StartBTN, "Start");
								   KillTimer(hWnd, 1);
								   for (int i = 0; i < 5; i++) RCstage[i] = 0;
							  }
							  else {
									if (ConnectProc(&ReadRank, true))
									  {
										  ReadRank.LoadOverlay();
										  procAct = procON;
										  BGCUR = BGON;
										  recordStage = true;
										  SetWindowText(StartBTN, "Stop");
										  SetTimer(hWnd, 1, 1000, NULL);
									  }
									  else
									  {
										  procAct = procOFF;
									  }
								 }
							   break;
						}

						  case ID_BLOODSTAT:
						  {
							   DestroyWindow(DataWnd);
							   StatType = bloodStat;
							   DataWnd = CreateWindow("PlayerDataWndClass", NULL, WS_CHILDWINDOW | WS_VISIBLE | WS_DLGFRAME | WS_VSCROLL | WS_CLIPCHILDREN, 10, 273, 500, 364, hWnd, (HMENU)ID_STATWINDOW, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

							   break;
						   }

						   case ID_WORLDSTAT:
						   {
							   DestroyWindow(DataWnd);
							   StatType = worldStat;
							   DataWnd = CreateWindow("PlayerDataWndClass", NULL, WS_CHILDWINDOW | WS_VISIBLE | WS_DLGFRAME | WS_VSCROLL | WS_CLIPCHILDREN, 10, 273, 500, 364, hWnd, (HMENU)ID_STATWINDOW, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

							   break;
						   }


						   case ID_ADDITION:
						   {
							   if (!OtherInf) OtherInf = CreateDialog(hInst, "Dlg", hWnd, NULL);
							   break;
						   }

					   }
					   InvalidateRect(hWnd, NULL, FALSE);
					   return 0;
	}

	case WM_TIMER:
	{	
		if (!ReadRank.GetProcessStatus())
		{
			recordStage = false;
			BGCUR = BGOFF;
			procAct = procOFF;
			SetWindowText(StartBTN, "Start");
			KillTimer(hWnd, 1);
			for (int i = 0; i < 5; i++) RCstage[i] = 0;
			InvalidateRect(hWnd, NULL, FALSE);
			return 0;
		}

		Statistic(0x1e8, 0x234, RCstage[0], 0, &statistic, &statisticWorld, &ReadRank);
		Statistic(0x2b8, 0x304, RCstage[1], 1, &statistic, &statisticWorld, &ReadRank);
		Statistic(0x388, 0x3d4, RCstage[2], 2, &statistic, &statisticWorld, &ReadRank);
		Statistic(0x458, 0x4a4, RCstage[3], 3, &statistic, &statisticWorld, &ReadRank);
		Statistic(0x528, 0x574, RCstage[4], 4, &statistic, &statisticWorld, &ReadRank);
		return 0;
	}

	case WM_MOUSEWHEEL:
	{
		if (StatType != emptyStat) {
			if (GET_WHEEL_DELTA_WPARAM(wParam) > 0) SendMessage(DataWnd, WM_VSCROLL, SB_LINEUP, 0);
			if (GET_WHEEL_DELTA_WPARAM(wParam) < 0) SendMessage(DataWnd, WM_VSCROLL, SB_LINEDOWN, 0);
		}
		return 0;
	}

	case WM_DESTROY:
	{
					   delete procON;
					   delete procOFF;
					   delete BGON;
					   delete BGOFF;
					   DeleteObject(hfont);
					   SaveFile(&statistic, &statisticWorld, MyDocFolder);

					   if (recordStage) KillTimer(hWnd, 1);
					   ReadRank.CloseProc();
					   PostQuitMessage(0);
					   return 0;
	}

	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}