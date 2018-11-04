/* 
*  main.c
*  MSPaint-Extension
*
*  Created by kimbomm on 2018. 09. 04...
*  Copyright 2018 kimbomm. All rights reserved.
*
*/
#include<stdio.h>
#include<Windows.h>
#include<shellapi.h>
#include<time.h>
#include"utils.h"
#include"rotator.h"
#include"color_memory.h"
#include"resource.h"
#define ID_TRAY_APP_ICON                5000
#define ID_TRAY_EXIT_CONTEXT_MENU_ITEM  3000
#define WM_TRAYICON ( WM_USER + 1 )
UINT WM_TASKBARCREATED = 0;

HWND g_hwnd;
HMENU g_menu;
double g_inittime;

NOTIFYICONDATA g_notifyIconData;
#pragma region helper funcs
void Minimize() {
	Shell_NotifyIcon(NIM_ADD, &g_notifyIconData);
	ShowWindow(g_hwnd, SW_HIDE);
}
void Restore() {
	Shell_NotifyIcon(NIM_DELETE, &g_notifyIconData);
	ShowWindow(g_hwnd, SW_SHOW);
}
void InitNotifyIconData() {
	ZeroMemory(&g_notifyIconData, sizeof(NOTIFYICONDATA));
	g_notifyIconData.cbSize = sizeof(NOTIFYICONDATA);
	g_notifyIconData.hWnd = g_hwnd;
	g_notifyIconData.uID = ID_TRAY_APP_ICON;
	g_notifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;      
	g_notifyIconData.uCallbackMessage = WM_TRAYICON; 
	g_notifyIconData.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
	strcpy_s(g_notifyIconData.szTip,128, TEXT("PSpring ^_^"));
}
#pragma endregion




bool isModalOfMsPaint(HWND hwnd,char** nullend_modalname) {
	char modal_name[MAX_PATH] = { 0 };
	GetWindowTextA(hwnd, modal_name, MAX_PATH);
	bool b = false;
	for (char** p = nullend_modalname; *p != NULL; p++) {
		b |= strcmp(*p, modal_name) == 0;
	}
	if (b) {
		HWND parent = GetParent(hwnd);
		char parent_class_name[MAX_PATH] = { 0 };
		GetClassNameA(parent, parent_class_name, MAX_PATH);
		if (!strcmp(parent_class_name, "MSPaintApp")) {
			return true;
		}
	}
	return false;
}
int main(){
	HANDLE hEvent;

	hEvent = CreateEventA(NULL, FALSE, TRUE, "mspaintex");

	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		return 1;
	}
#ifndef _DEBUG
	ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif
	char* resize_and_skew[] = {"크기 조정 및 기울이기","Resize and Skew",NULL};
	char* edit_colors[] = { "색 편집","Edit Colors",NULL };
	HWND hwnd;
	while (1) {
		hwnd = GetForegroundWindow();
		ThreadManage* tm = NULL;
		if (isModalOfMsPaint(hwnd, resize_and_skew)) {
			tm = MspexCreateThread(MspexRotator, hwnd);
		}
		else if (isModalOfMsPaint(hwnd, edit_colors)) {
			tm = MspexCreateThread(MspexColorMemory, hwnd);
		}
		
		Sleep(100);
		MspexUpdateThread(tm);
	}


	return 0;
}

DWORD WINAPI MsPaintExManage(LPVOID param) {
	char* resize_and_skew[] = { "크기 조정 및 기울이기","Resize and Skew",NULL };
	char* edit_colors[] = { "색 편집","Edit Colors",NULL };
	HWND hwnd;
	while (1) {
		hwnd = GetForegroundWindow();
		ThreadManage* tm = NULL;
		if (isModalOfMsPaint(hwnd, resize_and_skew)) {
			tm = MspexCreateThread(MspexRotator, hwnd);
		} else if (isModalOfMsPaint(hwnd, edit_colors)) {
			tm = MspexCreateThread(MspexColorMemory, hwnd);
		}
		MspexUpdateThread(tm);
		Sleep(300);
	}
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR args, int iCmdShow) {
	g_inittime = clock();
	TCHAR className[] = TEXT("PSpring");
	WM_TASKBARCREATED = RegisterWindowMessageA("TaskbarCreated");

	WNDCLASSEX wnd = { 0 };
	wnd.hInstance = hInstance;
	wnd.lpszClassName = className;
	wnd.lpfnWndProc = WndProc;
	wnd.style = CS_HREDRAW | CS_VREDRAW;
	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
	wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	wnd.hbrBackground = (HBRUSH)COLOR_APPWORKSPACE;

	if (!RegisterClassEx(&wnd)) {
		FatalAppExit(0, TEXT("Couldn't register window class!"));
	}

	//g_hwnd = CreateWindowEx(0, className,TEXT("PSpring"),WS_OVERLAPPEDWINDOW,CW_USEDEFAULT, CW_USEDEFAULT,840,623,NULL, NULL,hInstance, NULL);
	POINT zeropt = { 0 };
	HMONITOR hmonPrimary = MonitorFromPoint(zeropt, MONITOR_DEFAULTTOPRIMARY);
	MONITORINFO monitorinfo = { 0 };
	monitorinfo.cbSize = sizeof(monitorinfo);
	GetMonitorInfo(hmonPrimary, &monitorinfo);
	const RECT rcWork = monitorinfo.rcWork;
	SIZE sz;
	sz.cx = 840;
	sz.cy = 623;
	g_hwnd=CreateWindowExA(WS_EX_TOPMOST, className, TEXT("PSpring"), WS_POPUP, (rcWork.right-rcWork.left-sz.cx)/2,(rcWork.bottom-rcWork.top-sz.cy)/2,sz.cx,sz.cy, NULL, NULL, hInstance, 0);
	SetWindowLong(g_hwnd, GWL_EXSTYLE, GetWindowLong(g_hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(g_hwnd, RGB(244, 247, 252), 0, LWA_COLORKEY);
	InitNotifyIconData();
	ShowWindow(g_hwnd, iCmdShow);
	SetTimer(g_hwnd, 0x22222222, 2500, NULL);

	HANDLE hEvent;

	hEvent = CreateEventA(NULL, FALSE, TRUE, "mspaintex");

	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		return 1;
	}
	
	int tid;
	CreateThread(NULL, 0, MsPaintExManage, NULL, 0, &tid);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (!IsWindowVisible(g_hwnd)) {
		Shell_NotifyIcon(NIM_DELETE, &g_notifyIconData);
	}
	return msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static bool hide = false;
	if ((message == WM_TASKBARCREATED && !IsWindowVisible(g_hwnd))) {
		Minimize();
		return 0;
	}
	switch (message) {
		case WM_TIMER: {
			hide = true;
			Minimize();
		}break;
		case WM_PAINT: {
			HBITMAP png = (HBITMAP)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDB_BITMAP1), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
			PAINTSTRUCT ps;
			HDC hdc;
			BITMAP bitmap;
			HDC hdcMem;
			HGDIOBJ oldBitmap;
			hdc = BeginPaint(hwnd, &ps);
			hdcMem = CreateCompatibleDC(hdc);
			oldBitmap = SelectObject(hdcMem, png);
			GetObject(png, sizeof(bitmap), &bitmap);
			TransparentBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, RGB(255, 255, 255));
			SelectObject(hdcMem, oldBitmap);
			DeleteDC(hdcMem);
			EndPaint(hwnd, &ps);
			DeleteObject(png);
			break;
		}break;
		case WM_CREATE:
			g_menu = CreatePopupMenu();
			AppendMenu(g_menu, MF_STRING, ID_TRAY_EXIT_CONTEXT_MENU_ITEM, TEXT("Exit"));
			break;
		case WM_SYSCOMMAND:
			switch (wParam & 0xfff0) 
			{
				case SC_MINIMIZE:
				case SC_CLOSE: 
					Minimize();
					return 0;
					break;
			}
			break;
		case WM_TRAYICON:
		{
			if (lParam == WM_RBUTTONDOWN) {
				POINT curPoint;
				GetCursorPos(&curPoint);
				SetForegroundWindow(hwnd);
				UINT clicked = TrackPopupMenu(g_menu,TPM_RETURNCMD | TPM_NONOTIFY, curPoint.x,curPoint.y,0,hwnd,NULL);
				if (clicked == ID_TRAY_EXIT_CONTEXT_MENU_ITEM) {
					PostQuitMessage(0);
				}
			}
		}
		break;
		case WM_NCHITTEST:
		{
			UINT uHitTest = DefWindowProc(hwnd, WM_NCHITTEST, wParam, lParam);
			if (uHitTest == HTCLIENT)
				return HTCAPTION;
			else
				return uHitTest;
		}
		case WM_CLOSE:
			Minimize();
			return 0;
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}