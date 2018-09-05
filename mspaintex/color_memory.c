/*
*  color_memory.c
*  MSPaint-Extension
*
*  Created by kimbomm on 2018. 09. 04...
*  Copyright 2018 kimbomm. All rights reserved.
*
*/
#include"color_memory.h"

void FillSolarizedColors16(ColorBox* boxes,int beg) {
	static COLORREF colors[] = {
		RGB(0,43,54),
		RGB(7,54,66),
		RGB(88,110,117),
		RGB(101,123,131),
		RGB(131,148,150),
		RGB(147,161,161),
		RGB(238,232,213),
		RGB(253,246,227),
		RGB(181,137,0),
		RGB(203,75,22),
		RGB(220,50,47),
		RGB(211,54,130),
		RGB(108,113,196),
		RGB(38,139,210),
		RGB(42,161,152),
		RGB(133,153,0)
	};
	for (int i = beg; i < beg + 16; i++) {
		boxes[i].color = colors[i - beg];
	}
}
void FillNordColors16(ColorBox* boxes, int beg) {
	static COLORREF colors[] = {
		RGB(46,52,64),
		RGB(59,66,82),
		RGB(67,76,94),
		RGB(76,86,106),
		RGB(216,222,233),
		RGB(229,233,240),
		RGB(236,239,244),
		RGB(143,188,187),
		RGB(136,192,208),
		RGB(129,161,193),
		RGB(94,129,172),
		RGB(191,97,106),
		RGB(208,135,112),
		RGB(235,203,139),
		RGB(163,190,140),
		RGB(180,142,173),
	};
	for (int i = beg; i < beg + 16; i++) {
		boxes[i].color = colors[i - beg];
	}
}
void FillPastelColors16(ColorBox* boxes, int beg) {
	static COLORREF colors[] = {
		RGB(198,172,201),
		RGB(236,180,191),
		RGB(253,214,181),
		RGB(253,243,184),
		RGB(167,228,174),
		RGB(191,255,230),
		RGB(134,153,209),
		RGB(219,169,206),
		RGB(170,231,232),
		RGB(239,230,235),
		RGB(71,209,213),
		RGB(232,210,255),
		RGB(193,209,253),
		RGB(217,224,252),
		RGB(255,224,241),
		RGB(199,199,199)
	};
	for (int i = beg; i < beg + 16; i++) {
		boxes[i].color = colors[i - beg];
	}
}
void ReadRecentColors(ColorBox* boxes, int beg, int N,FILE* fp) {
	for (int i = beg; i < beg + N; i++) {
		int r=255, g=255, b=255;
		if(fp!=NULL)
			fscanf_s(fp,"%d%d%d", &r, &g, &b);
		boxes[i].color = RGB(r, g, b);
	}
	if(fp!=NULL)
		fclose(fp);
}
void WriteRecentColors(ColorBox* boxes, int beg, int N, FILE* fp,COLORREF last) {
	rewind(fp);
	fprintf_s(fp, "%d\t%d\t%d\n", GetRValue(last),GetGValue(last),GetBValue(last));
	for (int i = beg; i < beg + N-1; i++) {
		int r = GetRValue(boxes[i].color);
		int g = GetGValue(boxes[i].color);
		int b = GetBValue(boxes[i].color);
		fprintf_s(fp, "%d\t%d\t%d\n", r, g, b);
	}
	fclose(fp);
}
bool isAlreadyExistColor(COLORREF color, ColorBox* boxes, int n) {
	for (int i = 0; i < n; i++) {
		if (color == boxes[i].color) {
			return true;
		}
	}
	return false;
}
bool isDefautColor(COLORREF color) {
	static COLORREF colors[] = {
		RGB(255,128,128),
		RGB(255,255,128),
		RGB(128,255,128),
		RGB(0,255,128),
		RGB(128,255,255),
		RGB(0,128,255),
		RGB(255,128,192),
		RGB(255,128,255),
		RGB(255,0,0),
		RGB(255,255,0),
		RGB(128,255,0),
		RGB(0,255,64),
		RGB(0,255,255),
		RGB(0,128,192),
		RGB(128,128,192),
		RGB(255,0,255),
		RGB(128,64,64),
		RGB(255,128,64),
		RGB(0,255,0),
		RGB(0,128,128),
		RGB(0,64,128),
		RGB(128,128,255),
		RGB(128,0,64),
		RGB(255,0,128),
		RGB(128,0,0),
		RGB(255,128,0),
		RGB(0,128,0),
		RGB(0,128,64),
		RGB(0,0,255),
		RGB(0,0,160),
		RGB(128,0,128),
		RGB(128,0,255),
		RGB(64,0,0),
		RGB(128,64,0),
		RGB(0,64,0),
		RGB(0,64,64),
		RGB(0,0,128),
		RGB(0,0,64),
		RGB(64,0,64),
		RGB(64,0,128),
		RGB(0,0,0),
		RGB(128,128,0),
		RGB(128,128,64),
		RGB(128,128,128),
		RGB(64,128,128),
		RGB(192,192,192),
		RGB(64,0,64),
		RGB(255,255,255)
	};
	int n = sizeof(colors) / sizeof(COLORREF);
	for (int i = 0; i < n; i++) {
		if (colors[i] == color) {
			return true;
		}
	}
	return false;
}
void DrawColorBoxes(ColorBox* boxes, int beg, int sz, char* name,int pos,HDC hdc,RECT rect_palette) {
	HPEN pen105 = CreatePen(PS_SOLID, 1, RGB(105, 105, 105));
	HPEN pen160 = CreatePen(PS_SOLID, 1, RGB(160, 160, 160));
	HPEN pen227 = CreatePen(PS_SOLID, 1, RGB(227, 227, 227));
	TextOutA(hdc, rect_palette.left, rect_palette.top + pos-22, name, (int)strlen(name));
	for (int i = 0; i < sz / 8; i++) {
		for (int j = 0; j < 8; j++) {
			int idx = i * 8 + j + beg;
			const int W = 30;
			const int H = 25;
			const int W2 = 21;
			const int H2 = 18;
			RECT rect, rect2;
			rect2.left = rect.left = rect_palette.left + W * j;
			rect2.top = rect.top = rect_palette.top + H * i + pos;
			rect.right = rect.left + W;
			rect.bottom = rect.top + H;
			rect2.right = rect.left + W2;
			rect2.bottom = rect.top + H2;
			boxes[idx].rect = rect;
			HBRUSH brush = CreateSolidBrush(boxes[idx].color);
			SelectObject(hdc, brush);
			SelectObject(hdc, pen105);
			Rectangle(hdc, rect2.left, rect2.top, rect2.right, rect2.bottom);
			SelectObject(hdc, pen160);
			MoveToEx(hdc, rect2.left - 1, rect2.bottom - 1, NULL);
			LineTo(hdc, rect2.left - 1, rect2.top - 1);
			LineTo(hdc, rect2.right, rect2.top - 1);
			SelectObject(hdc, pen227);
			MoveToEx(hdc, rect2.right - 1, rect2.top, NULL);
			LineTo(hdc, rect2.right - 1, rect2.bottom - 1);
			LineTo(hdc, rect2.left - 1, rect2.bottom - 1);

			DeleteObject(brush);
		}
	}
	DeleteObject(pen105);
	DeleteObject(pen160);
	DeleteObject(pen227);
}
int ColorHitTest(HWND hwnd,ColorBox* boxes,int n) {
	if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) && GetForegroundWindow()==hwnd) {
		//InvalidateRect(tm->hwnd, NULL, 1);
		POINT point;
		GetCursorPos(&point);
		RECT wrect;
		GetWindowRect(hwnd, &wrect);
		point.x -= wrect.left;
		point.y -= wrect.top;
		for (int i = 0; i < n; i++) {
			if (PtInRect(&boxes[i].rect, point)) {
				return i;
			}
		}
	}
	return -1;
}
COLORREF GetLastColor(HWND rhwnd, HWND ghwnd, HWND bhwnd) {
	char rstr[64] = { 0 };
	SendMessage(rhwnd, WM_GETTEXT, (WPARAM)64, (LPARAM)rstr);
	int r = strlen(rstr) != 0 ? atoi(rstr) : 0;

	char gstr[64] = { 0 };
	SendMessage(ghwnd, WM_GETTEXT, (WPARAM)64, (LPARAM)gstr);
	int g = strlen(gstr) != 0 ? atoi(gstr) : 0;

	char bstr[64] = { 0 };
	SendMessage(bhwnd, WM_GETTEXT, (WPARAM)64, (LPARAM)bstr);
	int b = strlen(bstr) != 0 ? atoi(bstr) : 0;

	return RGB(r, g, b);
}
DWORD WINAPI MspexColorMemory(LPVOID param) {
	ThreadManage* tm = (ThreadManage*)param;

	const int RRIGHT = 256;
	RECT dlg_rect;
	GetWindowRect(tm->hwnd, &dlg_rect);
	SetWindowPos(tm->hwnd, NULL, 0, 0, dlg_rect.right - dlg_rect.left+RRIGHT, dlg_rect.bottom - dlg_rect.top, SWP_NOMOVE | SWP_SHOWWINDOW);
	dlg_rect.right += RRIGHT;
	RECT dlg_crect;
	GetClientRect(tm->hwnd, &dlg_crect);

	HDC hdc = GetWindowDC(tm->hwnd);
#define COLOR_MEMORY_NUMBER 64+16
	ColorBox boxes[COLOR_MEMORY_NUMBER] = { 0 };
	FillSolarizedColors16(boxes, 0);
	FillNordColors16(boxes, 16);
	FillPastelColors16(boxes, 32);
	char path[MAX_PATH] = { 0 };
	SHGetSpecialFolderPathA(NULL, path, CSIDL_PROFILE, FALSE);
	strcat_s(path,MAX_PATH, "\\mspex_recent.txt");

	FILE* fp = NULL;
	fopen_s(&fp, path, "r");
	ReadRecentColors(boxes, 48, 32, fp);

	const int EDIT_H=0;
	const int EDIT_S=1;
	const int EDIT_V=2;
	const int EDIT_R=3;
	const int EDIT_G=4;
	const int EDIT_B=5;
	int hwnd_edits_sz = 0;
	HWND* hwnd_edits = GetHwndsByClass(tm->hwnd, "Edit", &hwnd_edits_sz);


	RECT rect_palette = dlg_crect;
	rect_palette.left = rect_palette.right - 245 + 10;
	rect_palette.top += 62;
	HFONT font = CreateFontA(16, 0, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0, "Segoe UI");
	SelectObject(hdc, font);

	SetTextColor(hdc, RGB(33, 33, 33));
	SetBkColor(hdc, RGB(240, 240, 240));
	
	COLORREF last_color = RGB(0, 0, 0);
	//Rectangle(hdc, rect_palette.left, rect_palette.top, rect_palette.right, rect_palette.bottom);
	while (IsWindow(tm->hwnd)) {
			

		DrawColorBoxes(boxes, 0, 16, "Solarized", 0, hdc, rect_palette);
		DrawColorBoxes(boxes,16, 16, "Nord", 71, hdc, rect_palette);
		DrawColorBoxes(boxes, 32, 16, "Pastel", 71*2, hdc, rect_palette);
		DrawColorBoxes(boxes, 48, 32, "Recent", 71*3, hdc, rect_palette);
		int sel = ColorHitTest(tm->hwnd, boxes, COLOR_MEMORY_NUMBER);
		if (sel >= 0) {
			char Rvalue[64];
			sprintf_s(Rvalue, 64,"%d", GetRValue(boxes[sel].color));
			SendMessage(hwnd_edits[EDIT_R], WM_SETTEXT, (WPARAM)NULL, (LPARAM)Rvalue);

			char Gvalue[64];
			sprintf_s(Gvalue,64, "%d", GetGValue(boxes[sel].color));
			SendMessage(hwnd_edits[EDIT_G], WM_SETTEXT, (WPARAM)NULL, (LPARAM)Gvalue);

			char Bvalue[64];
			sprintf_s(Bvalue,64, "%d", GetBValue(boxes[sel].color));
			SendMessage(hwnd_edits[EDIT_B], WM_SETTEXT, (WPARAM)NULL, (LPARAM)Bvalue);
		}
		last_color=GetLastColor(hwnd_edits[EDIT_R], hwnd_edits[EDIT_G], hwnd_edits[EDIT_B]);
		Sleep(100);;

			
	}
	
	if (!isAlreadyExistColor(last_color,boxes, COLOR_MEMORY_NUMBER)) {
		fp = NULL;
		while (fopen_s(&fp, path, "w") != 0 || fp == NULL)Sleep(0);
		WriteRecentColors(boxes, 48, 32, fp,last_color);
	}
	DeleteObject(font);
	free(hwnd_edits);
	printf("Thread(%zd) terminated\n", tm->hwnd);
	return tm->exitcode = 0;
}