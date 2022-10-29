#include "PlayerDataWndProc.h"

LRESULT CALLBACK PlayerDataWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	HDC						hDC;
	HDC						hCompatibleDC;
	PAINTSTRUCT				ps;
	RECT					Rect;
	POINT					pt;

	static int				scrollPosY, scrollPosYprev;
	static bool				PlayerWindow;
	static HBITMAP			hBmp;
	static vector<Enemy>*	stats;

	static HWND				ReturnBtn;
	static HWND*			ListBtn;
	static HWND*			DataBtn;
	static Enemy			target;

	static int*				Counts;
	static int*				equipPosY;

	static Image			*Background;

	switch (message) {
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
		if (ID >= 0 && ID < ENTRYCOUNT)
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
		if (ID >= ENTRYCOUNT && ID < ENTRYCOUNT * 2 + ENTRYCOUNT)
		{
			ChangeEquipCount(wParam, &target, equipPosY, Counts, scrollPosY);
			InvalidateRect(hWnd, NULL, FALSE);
		}

		//OTHER BUTTONS
		switch (ID)
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