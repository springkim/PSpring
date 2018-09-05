/*
*  utils.c
*  MSPaint-Extension
*
*  Created by kimbomm on 2018. 09. 04...
*  Copyright 2018 kimbomm. All rights reserved.
*
*/
#pragma once
#include<Windows.h>
#include<stdbool.h>
#include<stdio.h>
typedef struct ThreadManage ThreadManage;
struct ThreadManage {
	HWND hwnd;
	HANDLE tid;
	DWORD exitcode;
	ThreadManage* next;
};
#define MSPEX_FORM	(WS_CHILDWINDOW | WS_VISIBLE | WS_GROUP | BS_GROUPBOX)

HWND* WINAPI GetInnerHWnds(HWND hwnd, DWORD style, int* n);
HWND WINAPI GetInnerHWndsByID(HWND hwnd, DWORD id);
HWND* WINAPI GetHwndsByClass(HWND hwnd, char* _class, int* n);
ThreadManage* MspexCreateThread(DWORD(WINAPI*f)(LPVOID), HWND hwnd);
void MspexUpdateThread(ThreadManage* tm);