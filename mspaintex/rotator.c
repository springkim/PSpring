/*
*  rotator.c
*  MSPaint-Extension
*
*  Created by kimbomm on 2018. 09. 04...
*  Copyright 2018 kimbomm. All rights reserved.
*
*/
#include"rotator.h"
static char* mspex_r[] = { "71","141","-26","45","72","139","-26","44","73","137","-26","43","74","135","-25","42","75","133","-25","41","77","131","-25","40","78","129","-25","39","79","127","-25","38","80","125","-25","37","81","124","-24","36","82","122","-24","35","83","121","-24","34","84","119","-24","33","85","118","-23","32","86","117","-23","31","87","115","-22","30","87","114","-22","29","88","113","-22","28","89","112","-21","27","90","111","-21","26","91","110","-20","25","91","109","-19","24","92","109","-19","23","93","108","-18","22","93","107","-17","21","94","106","-17","20","95","106","-16","19","95","105","-15","18","96","105","-15","17","96","104","-14","16","97","104","-13","15","97","103","-12","14","97","103","-11","13","98","102","-10","12","98","102","-10","11","98","102","-9","10","99","101","-8","9","99","101","-7","8","99","101","-6","7","99","101","-5","6","100","100","-4","5","100","100","-3","4","100","100","-2","3","100","100","-1","2","100","100","0","1","100","100","0","0","100","100","1","-1","100","100","2","-2","100","100","3","-3","100","100","4","-4","100","100","5","-5","99","101","6","-6","99","101","7","-7","99","101","8","-8","99","101","9","-9","98","102","10","-10","98","102","11","-11","98","102","11","-12","97","103","12","-13","97","103","13","-14","97","104","14","-15","96","104","15","-16","96","105","16","-17","95","105","16","-18","95","106","17","-19","94","106","18","-20","93","107","18","-21","93","108","19","-22","92","109","20","-23","91","109","20","-24","91","110","21","-25","90","111","22","-26","89","112","22","-27","88","113","23","-28","87","114","23","-29","87","115","23","-30","86","117","24","-31","85","118","24","-32","84","119","25","-33","83","121","25","-34","82","122","25","-35","81","124","25","-36","80","125","26","-37","79","127","26","-38","78","129","26","-39","77","131","26","-40","75","133","26","-41","74","135","26","-42","73","137","27","-43","72","139","27","-44","71","141","27","-45","71","141","27","-45" };
void DrawProtector(HDC hdc, HWND hwnd, RECT rect) {
	//HRGN hrgn = CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
	//SelectClipRgn(hdc, hrgn);
	static bool init = true;
	static HBITMAP image = NULL;
	//HBITMAP image=LoadImageA(NULL, "protractor.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (init == true) {
		if (image == NULL) {
			image = GetHeaderResource(GetDC(GetConsoleWindow()), protractor_image_width, protractor_image_height, protractor_image);
		}
		init = false;
	}

	HDC memdc = CreateCompatibleDC(hdc);
	HBITMAP old_bmp = (HBITMAP)SelectObject(memdc, image);
	BITMAP bit;
	GetObject(image, sizeof(BITMAP), &bit);
	StretchBlt(hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, memdc, 0, 0, bit.bmWidth, bit.bmHeight, SRCCOPY);
	BitBlt(hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, memdc, 0, 0, SRCCOPY);

	DeleteDC(memdc);
	//DeleteObject(hrgn);
}
POINT AnglePoint(POINT base, int zeroh, int angle) {
	double r = angle * (acos(-1) / 180.0);
	POINT ret;
	POINT tmp;
	tmp.x = 0;
	tmp.y = -zeroh;
	ret.x = (LONG)(cos(r)*tmp.x - sin(r)*tmp.y);
	ret.y = (LONG)(sin(r)*tmp.x + cos(r)*tmp.y);
	ret.x += base.x;
	ret.y += base.y;
	return ret;
}
DWORD WINAPI MspexRotator(LPVOID param) {
	const int BTN_RESIZE = 0;
	const int BTN_RATIO = 1;
	const int BTN_PIXEL = 2;
	const int BTN_MAINTAIN_RATIO = 3;
	const int BTN_SKEW = 4;
	const int BTN_OK = 5;
	const int BTN_CANCEL = 6;
	const int EDIT_RESIZE_WIDTH = 0;
	const int EDIT_RESIZE_HEIGHT = 1;
	const int EDIT_SKEW_WIDTH = 2;
	const int EDIT_SKEW_HEIGHT = 3;
	ThreadManage* tm = (ThreadManage*)param;
	int hwnd_btns_sz = 0;
	HWND* hwnd_btns = GetHwndsByClass(tm->hwnd, "Button", &hwnd_btns_sz);
	int hwnd_edits_sz = 0;
	HWND* hwnd_edits = GetHwndsByClass(tm->hwnd, "Edit", &hwnd_edits_sz);

	const int RHEIGHT = 150;

	RECT dlg_rect;
	GetWindowRect(tm->hwnd, &dlg_rect);
	SetWindowPos(tm->hwnd, NULL, 0, 0, dlg_rect.right - dlg_rect.left, dlg_rect.bottom - dlg_rect.top + RHEIGHT, SWP_NOMOVE | SWP_SHOWWINDOW);
	dlg_rect.bottom += RHEIGHT;
	//Move buttons to bottom
	for (int i = BTN_OK; i <= BTN_CANCEL; i++) {
		RECT rect;
		GetWindowRect(hwnd_btns[i], &rect);
		SetWindowPos(hwnd_btns[i], NULL, rect.left - dlg_rect.left, rect.top - dlg_rect.top + RHEIGHT - (rect.bottom - rect.top), 0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_SHOWWINDOW);
	}
	RECT dlg_crect;
	GetClientRect(tm->hwnd, &dlg_crect);
	HDC hdc = GetWindowDC(tm->hwnd);
	RECT rect_protractor;
	const int PADDING = 13;
	rect_protractor.left = dlg_crect.left + PADDING;
	rect_protractor.top = (dlg_crect.bottom - dlg_crect.top) - RHEIGHT + PADDING;
	rect_protractor.right = dlg_crect.right - 1;
	rect_protractor.bottom = rect_protractor.top + (rect_protractor.right - rect_protractor.left) / 2;
	RECT rect_protractor_r = { 0 };
	rect_protractor_r.right = rect_protractor.right - rect_protractor.left;
	rect_protractor_r.bottom = rect_protractor.bottom - rect_protractor.top;
	//Rectangle(hdc, rect_protractor.left , rect_protractor.top, rect_protractor.right, rect_protractor.bottom);

	

	HPEN pen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	
	POINT center;
	center.x = (rect_protractor.left + rect_protractor.right) / 2;
	center.y = rect_protractor.bottom;
	POINT center_r;
	center_r.x = (rect_protractor_r.left + rect_protractor_r.right) / 2;
	center_r.y = rect_protractor_r.bottom;
	//DrawProtector(hdc, tm->hwnd, rect_protractor);
	HFONT font= CreateFontA(20, 0, 0, 0, 0, TRUE, 0, 0, 0, 0, 0, 0, 0, "Arial");
	
	SetTextColor(hdc, RGB(33, 33, 33));
	SetBkColor(hdc, RGB(240, 240, 240));
	int angle = 0;
	while (IsWindow(tm->hwnd)) {
		HDC ddc = CreateCompatibleDC(hdc);
		HBITMAP bmp = CreateCompatibleBitmap(hdc, rect_protractor.right - rect_protractor.left, rect_protractor.bottom - rect_protractor.top);
		SelectObject(ddc, bmp);
		PatBlt(ddc, 0, 0, rect_protractor.right - rect_protractor.left, rect_protractor.bottom - rect_protractor.top, WHITENESS);

		SelectObject(ddc, font);
		SelectObject(ddc, pen);
		

		DrawProtector(ddc, tm->hwnd, rect_protractor_r);
		if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) && GetForegroundWindow() == tm->hwnd) {
			//InvalidateRect(tm->hwnd, NULL, 1);
			POINT point;
			GetCursorPos(&point);
			RECT wrect;
			GetWindowRect(tm->hwnd, &wrect);
			point.x -= wrect.left;
			point.y -= wrect.top;
			if (PtInRect(&rect_protractor, point)) {
				double d = (floor((atan2(center.y - point.y, point.x - center.x) * 180 / acos(-1)) + 0.5)) - 90;
				d *= -1;
				angle = (int)d;
				if (angle < -45)angle = -45;
				if (angle > 45)angle = 45;
				SendMessage(hwnd_btns[BTN_RATIO], BM_SETCHECK, BST_CHECKED, (WPARAM)NULL);
				SendMessage(hwnd_btns[BTN_PIXEL], BM_SETCHECK, BST_UNCHECKED, (WPARAM)NULL);
				SendMessage(hwnd_btns[BTN_MAINTAIN_RATIO], BM_SETCHECK, BST_UNCHECKED, (WPARAM)NULL);
				SendMessage(hwnd_edits[EDIT_RESIZE_WIDTH], WM_SETTEXT, (LPARAM)NULL, (LPARAM)mspex_r[(angle + 45) * 4 + 0]);
				SendMessage(hwnd_edits[EDIT_RESIZE_HEIGHT], WM_SETTEXT, (LPARAM)NULL, (LPARAM)mspex_r[(angle + 45) * 4 + 1]);
				SendMessage(hwnd_edits[EDIT_SKEW_WIDTH], WM_SETTEXT, (LPARAM)NULL, (LPARAM)mspex_r[(angle + 45) * 4 + 2]);
				SendMessage(hwnd_edits[EDIT_SKEW_HEIGHT], WM_SETTEXT, (LPARAM)NULL, (LPARAM)mspex_r[(angle + 45) * 4 + 3]);
				
			}
		}
		MoveToEx(ddc, center_r.x, center_r.y, NULL);
		POINT dst = AnglePoint(center_r, rect_protractor_r.bottom - rect_protractor_r.top, angle);
		LineTo(ddc, dst.x, dst.y);
		char num[64] = { 0 };
		sprintf_s(num, 64," %s%d ", angle>=0?"+":"-",abs(angle));
		SIZE sz;
		GetTextExtentPoint32A(ddc, num, strlen(num), &sz);
		TextOutA(hdc, (rect_protractor.left+ rect_protractor.right)/2-sz.cx/2, rect_protractor.top-30, num, strlen(num));

		BitBlt(hdc, rect_protractor.left, rect_protractor.top, rect_protractor.right - rect_protractor.left, rect_protractor.bottom - rect_protractor.top, ddc, 0, 0, SRCCOPY);
		Sleep(50);
		DeleteDC(ddc);
		DeleteObject(bmp);
		
	}
	free(hwnd_btns);
	free(hwnd_edits);
	DeleteObject(pen);
	DeleteObject(font);
	printf("Thread(%zd) terminated\n", tm->hwnd);
	return tm->exitcode = 0;
}

/*
회전         늘이기             기울이기
각도     가로      세로       가로      세로
-45        71        141        -26        45
-44        72        139        -26        44
-43        73        137        -26        43
-42        74        135        -25        42
-41        75        133        -25        41
-40        77        131        -25        40
-39        78        129        -25        39
-38        79        127        -25        38
-37        80        125        -25        37
-36        81        124        -24        36
-35        82        122        -24        35
-34        83        121        -24        34
-33        84        119        -24        33
-32        85        118        -23        32
-31        86        117        -23        31
-30        87        115        -22        30
-29        87        114        -22        29
-28        88        113        -22        28
-27        89        112        -21        27
-26        90        111        -21        26
-25        91        110        -20        25
-24        91        109        -19        24
-23        92        109        -19        23
-22        93        108        -18        22
-21        93        107        -17        21
-20        94        106        -17        20
-19        95        106        -16        19
-18        95        105        -15        18
-17        96        105        -15        17
-16        96        104        -14        16
-15        97        104        -13        15
-14        97        103        -12        14
-13        97        103        -11        13
-12        98        102        -10        12
-11        98        102        -10        11
-10        98        102         -9        10
-9        99         101        -8          9
-8        99         101        -7          8
-7        99         101        -6          7
-6        99         101        -5          6
-5       100        100        -4          5
-4       100        100        -3          4
-3       100        100        -2          3
-2       100        100        -1          2
-1       100        100         0          1
0       100        100         0          0
1       100        100         1         -1
2       100        100         2         -2
3       100        100         3         -3
4       100        100         4         -4
5       100        100         5         -5
6        99        101         6         -6
7        99        101         7         -7
8        99        101         8         -8
9        99        101         9         -9
10        98        102        10        -10
11        98        102        11        -11
12        98        102        11        -12
13        97        103        12        -13
14        97        103        13        -14
15        97        104        14        -15
16        96        104        15        -16
17        96        105        16        -17
18        95        105        16        -18
19        95        106        17        -19
20        94        106        18        -20
21        93        107        18        -21
22        93        108        19        -22
23        92        109        20        -23
24        91        109        20        -24
25        91        110        21        -25
26        90        111        22        -26
27        89        112        22        -27
28        88        113        23        -28
29        87        114        23        -29
30        87        115        23        -30
31        86        117        24        -31
32        85        118        24        -32
33        84        119        25        -33
34        83        121        25        -34
35        82        122        25        -35
36        81        124        25        -36
37        80        125        26        -37
38        79        127        26        -38
39        78        129        26        -39
40        77        131        26        -40
41        75        133        26        -41
42        74        135        26        -42
43        73        137        27        -43
44        72        139        27        -44
45        71        141        27        -45
*/