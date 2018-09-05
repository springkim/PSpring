/* 
*  main.c
*  MSPaint-Extension
*
*  Created by kimbomm on 2018. 09. 04...
*  Copyright 2018 kimbomm. All rights reserved.
*
*/
#include<stdio.h>
#include <Windows.h>
#include"utils.h"
#include"rotator.h"
#include"color_memory.h"


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