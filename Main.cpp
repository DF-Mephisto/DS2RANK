#pragma once

#include "MemReader.h"
#include "resource4.h"
#include "Source.h"
#include "IDs.h"

#pragma comment (lib,"Gdiplus.lib")
#pragma comment(linker,"/manifestdependency:\"type='win32' \
                        name='Microsoft.Windows.Common-Controls' \
                        version='6.0.0.0' processorArchitecture='*'\
			publicKeyToken='6595b64144ccf1df' language='*'\"")

using namespace std;
using namespace Gdiplus;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OtherProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

HINSTANCE hInst;
Trainer ReadRank;

vector<Enemy> statistic;
vector<Enemy> statisticWorld;

HWND OtherInf;
HFONT hfont;

StatisticsType StatType = emptyStat;

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
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hIconSm = (HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);
	wc.lpszClassName = "WindowClass";
	RegisterClassEx(&wc);

	wc.cbWndExtra = DLGWINDOWEXTRA;
	wc.lpfnWndProc = OtherProc;
	wc.lpszClassName = "OtherProc";
	RegisterClassEx(&wc);

	wc.cbWndExtra = 0;
	wc.lpfnWndProc = ChildWindowProc;
	wc.hIconSm = NULL;
	wc.lpszClassName = "ChildWindowClass";
	RegisterClassEx(&wc);

	HWND hWnd = CreateWindow("WindowClass", "DS2RANK v.4.2", WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN, (GetSystemMetrics(SM_CXSCREEN) / 2) - 300, (GetSystemMetrics(SM_CYSCREEN) / 2) - 300, 764, 688, 0, 0, hInstance, 0);
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

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC			hDC;
	HDC			hCompatibleDC;
	RECT			Rect;
	PAINTSTRUCT		PaintStruct;
	POINT			pt;

	static bool		recordStage;
	static HWND		label;
	static HWND		StartBTN, BloodBTN, WorldBTN, AdditionBTN;
	static HBITMAP		hBmp;
	static Image		*procON, *procOFF, *procAct;
	static Image		*BGON, *BGOFF, *BGCUR;
	static wchar_t  	MyDocFolder[MAX_PATH];

	static int*		RCstage; 


	switch (message)
	{
	case WM_CREATE:
	{
					  FillIDs();

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

					  label = CreateWindow("ChildWindowClass", NULL, WS_CHILDWINDOW | WS_VISIBLE | WS_DLGFRAME | WS_VSCROLL | WS_CLIPCHILDREN, 10, 273, 500, 364, hWnd, (HMENU)ID_STATWINDOW, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);					 

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
							   DestroyWindow(label);
							   StatType = bloodStat;
							   label = CreateWindow("ChildWindowClass", NULL, WS_CHILDWINDOW | WS_VISIBLE | WS_DLGFRAME | WS_VSCROLL | WS_CLIPCHILDREN, 10, 273, 500, 364, hWnd, (HMENU)ID_STATWINDOW, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

							   break;
						   }

						   case ID_WORLDSTAT:
						   {
							   DestroyWindow(label);
							   StatType = worldStat;
							   label = CreateWindow("ChildWindowClass", NULL, WS_CHILDWINDOW | WS_VISIBLE | WS_DLGFRAME | WS_VSCROLL | WS_CLIPCHILDREN, 10, 273, 500, 364, hWnd, (HMENU)ID_STATWINDOW, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

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
			if (GET_WHEEL_DELTA_WPARAM(wParam) > 0) SendMessage(label, WM_VSCROLL, SB_LINEUP, 0);
			if (GET_WHEEL_DELTA_WPARAM(wParam) < 0) SendMessage(label, WM_VSCROLL, SB_LINEDOWN, 0);
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

LRESULT CALLBACK ChildWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){

	HDC			hDC;
	HDC			hCompatibleDC;
	PAINTSTRUCT		ps;
	RECT			Rect;
	POINT			pt;
	
	static int		scrollPosY, scrollPosYprev;
	static bool		PlayerWindow;
	static HBITMAP		hBmp;
	static vector<Enemy>*	stats;

	static HWND		ReturnBtn;
	static HWND*		ListBtn;
	static HWND*		DataBtn;
	static Enemy		target;

	static int*		Counts;
	static int*		equipPosY;

	static Image		*Background;

	switch (message){
	case WM_CREATE:
	{
		CreateListBtn(&ListBtn, hWnd, hInst);
		CreateDataBtn(&DataBtn, hWnd, hInst);
		ReturnBtn = CreateWindow("button", "...", WS_CHILD | BS_PUSHBUTTON, X_RET_BTN, Y_BTN_START_POS, XRET_SZ, YRET_SZ, hWnd, (HMENU)ID_RETURNBTN, hInst, NULL);
		
		if (StatType == bloodStat)
		{
			stats = &statistic;
			Background = LoadGdiImage(MAKEINTRESOURCE(IDB_ARENA), "JPG", hInst);
		}
		else if (StatType == worldStat)
		{
			stats = &statisticWorld;
			Background = LoadGdiImage(MAKEINTRESOURCE(IDB_WORLD), "JPG", hInst);
		}
		else if (StatType == emptyStat)
		{
			Background = LoadGdiImage(MAKEINTRESOURCE(IDB_ARENA), "JPG", hInst);
		}

		scrollPosY = (StatType == emptyStat) ? 0 : stats->size() <= ENTRYCOUNT ? 0 : stats->size() - ENTRYCOUNT;
		scrollPosYprev = 0;
		if (StatType == emptyStat) SetScrollRange(hWnd, SB_VERT, 0, 0, TRUE);
		else
		{
			ShowListBtn(ListBtn, stats);
			SetScrollRange(hWnd, SB_VERT, 0, stats->size() <= ENTRYCOUNT ? 0 : stats->size() - ENTRYCOUNT, TRUE);
			SetScrollPos(hWnd, SB_VERT, scrollPosY, TRUE);
		}

		PlayerWindow = false;

		Counts = new int[EQUIPCOUNT];
		equipPosY = new int[EQUIPCOUNT];
		ClearData(Counts, EQUIPCOUNT);
		FillequipPosY(equipPosY);

		return 0;
	}

	case WM_PAINT:
	{
					 hDC = BeginPaint(hWnd, &ps);
					 GetWindowRect(hWnd, &Rect);
					 pt.x = Rect.right;
					 pt.y = Rect.bottom;
					 ScreenToClient(hWnd, &pt);

					 hCompatibleDC = CreateCompatibleDC(hDC);
					 SelectObject(hCompatibleDC, hfont);
					 hBmp = CreateCompatibleBitmap(hDC, pt.x, pt.y);
					 SelectObject(hCompatibleDC, hBmp);
					 PatBlt(hCompatibleDC, 0, 0, pt.x, pt.y, PATCOPY);
					 
					 Graphics graph(hCompatibleDC);
					 graph.SetSmoothingMode(SmoothingModeNone);
					 graph.DrawImage(Background, 0, 0, 500, 364);

					 SetBkMode(hCompatibleDC, TRANSPARENT);
					 SetTextColor(hCompatibleDC, RGB(0xE0, 0xE0, 0xE0));

					 if (StatType != emptyStat)
					 {
						 if (PlayerWindow == false)
						 {
							 ShowListBtn(ListBtn, stats);
							 DrawPlayerList(hCompatibleDC, stats, scrollPosY, StatType);
						 }
						 else
						 {
							 ShowDataBtn(DataBtn, &target, equipPosY, scrollPosY);
							 DrawPlayerData(hCompatibleDC, &target, Counts, scrollPosY);
						 }
					 }

					 BitBlt(hDC, 0, 0, pt.x, pt.y, hCompatibleDC, 0, 0, SRCCOPY);
					 DeleteObject(hBmp);
					 DeleteDC(hCompatibleDC);
					 EndPaint(hWnd, &ps);
					 return 0;
	}

	case WM_VSCROLL:
	{
		int MaxScroll = PlayerWindow ? STATCOUNT - ENTRYCOUNT : stats->size() - ENTRYCOUNT;

		switch (LOWORD(wParam))
		{

		case SB_THUMBTRACK:
		{
			scrollPosY = HIWORD(wParam);
			break;
		}

		case SB_LINEDOWN:
		{
			if (scrollPosY < MaxScroll) {
				scrollPosY++;
			}
			break;
		}

		case SB_LINEUP:
		{
			if (scrollPosY > 0) {
				scrollPosY--;
			}
			break;
		}

		case SB_PAGEDOWN:
		{
			if (scrollPosY < MaxScroll) {
				scrollPosY++;
			}
			break;
		}

		case SB_PAGEUP:
		{
			if (scrollPosY > 0) {
				scrollPosY--;
			}
			break;
		}

		}

		SetScrollRange(hWnd, SB_VERT, 0, MaxScroll, TRUE);
		SetScrollPos(hWnd, SB_VERT, scrollPosY, TRUE);
		InvalidateRect(hWnd, NULL, FALSE);

		return 0;
	}

	case WM_COMMAND:
	{
		int ID = LOWORD(wParam);

		//LIST BUTTONS
		if( ID >= 0 && ID < ENTRYCOUNT)
		{
				HideBtn(ListBtn, ENTRYCOUNT);
				ShowWindow(ReturnBtn, SW_SHOW);

				int playerNumber = scrollPosY + LOWORD(wParam);
				target = (*stats)[playerNumber];
				PlayerWindow = true;

				scrollPosYprev = scrollPosY;
				scrollPosY = 0;
				SetScrollRange(hWnd, SB_VERT, 0, STATCOUNT - ENTRYCOUNT, TRUE);
				SetScrollPos(hWnd, SB_VERT, scrollPosY, TRUE);
				InvalidateRect(hWnd, NULL, FALSE);
		}

		//EQUIP DATA BUTTONS
		if (ID >= ENTRYCOUNT && ID < ENTRYCOUNT * 2 + ENTRYCOUNT )
		{
				ChangeEquipCount(wParam, &target, equipPosY, Counts, scrollPosY);
				InvalidateRect(hWnd, NULL, FALSE);
		}

		//OTHER BUTTONS
		switch(ID)
		{
			case ID_RETURNBTN:
			{
				HideBtn(DataBtn, ENTRYCOUNT * 2);
				ShowWindow(ReturnBtn, SW_HIDE);

				PlayerWindow = false;
				ClearData(Counts, EQUIPCOUNT);

				scrollPosY = scrollPosYprev;
				SetScrollRange(hWnd, SB_VERT, 0, stats->size() <= ENTRYCOUNT ? 0 : stats->size() - ENTRYCOUNT, TRUE);
				SetScrollPos(hWnd, SB_VERT, scrollPosY, TRUE);
				InvalidateRect(hWnd, NULL, FALSE);

				break;
			}
		}

		return 0;
	}

	case WM_DESTROY:
	{
		delete[] Counts;
		delete[] equipPosY;
		delete Background;

		DestroyBtn(ListBtn, ENTRYCOUNT);
		DestroyBtn(DataBtn, ENTRYCOUNT * 2);
		DestroyWindow(ReturnBtn);

		return 0;
	}

	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK OtherProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC 		hDC, hCompDC;
	PAINTSTRUCT 	ps;
	RECT 		rect;
	HBITMAP 	bmpBack;

	static 		Image* Background;
	
	switch (message)
	{
	case WM_CREATE:
	{
		MoveWindow(hWnd, GetSystemMetrics(SM_CXSCREEN) / 2 - 190, GetSystemMetrics(SM_CYSCREEN) / 2 - 65, 400, 130, false);
		Background = LoadGdiImage(MAKEINTRESOURCE(OTHERBACK_PNG), "PNG", hInst);

		return 0;
	}

	case WM_PAINT:
	{
					hDC = BeginPaint(hWnd, &ps);
					GetWindowRect(hWnd, &rect);
					hCompDC = CreateCompatibleDC(hDC);
					bmpBack = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
					SelectObject(hCompDC, bmpBack);
					PatBlt(hCompDC, 0, 0, rect.right, rect.bottom, PATCOPY);

					Graphics graph(hCompDC);
					graph.DrawImage(Background, 0, 0, 400, 600);

					SelectObject(hCompDC, hfont);
					SetTextColor(hCompDC, RGB(0xE0, 0xE0, 0xE0));
					SetBkMode(hCompDC, TRANSPARENT);

					string tempstr;
					tempstr = "You have killed hosts for getting Ring of Thorns: " + to_string(ReadRank.GetRingStat(THORN_offset));
					TextOut(hCompDC, 5, 5, tempstr.c_str(), tempstr.length());

					tempstr = "You have killed invader dark phantoms for getting";
					TextOut(hCompDC, 5, 40, tempstr.c_str(), tempstr.length());

					tempstr = "Lingering Dragoncrest Ring: " + to_string(ReadRank.GetRingStat(DRAGONCREST_offset));
					TextOut(hCompDC, 5, 60, tempstr.c_str(), tempstr.length());
						
					
					BitBlt(hDC, 0, 0, rect.right, rect.bottom, hCompDC, 0, 0, SRCCOPY);

					EndPaint(hWnd, &ps);
					DeleteDC(hCompDC);
					DeleteObject(bmpBack);
					return 0;
	}

	case WM_DESTROY:
	{
		delete Background;
		OtherInf = 0;

		return 0;
	}

	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
