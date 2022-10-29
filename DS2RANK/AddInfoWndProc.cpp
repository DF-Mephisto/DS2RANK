#include "AddInfoWndProc.h"

LRESULT CALLBACK AddInfoWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hDC, hCompDC;
	PAINTSTRUCT ps;
	RECT rect;
	HBITMAP bmpBack;

	static Image* Background;

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