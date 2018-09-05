/*
*  color_memory.c
*  MSPaint-Extension
*
*  Created by kimbomm on 2018. 09. 04...
*  Copyright 2018 kimbomm. All rights reserved.
*
*/
#pragma once
#include"utils.h"
#include<ShlObj.h>
#pragma comment(lib,"Shell32.lib")
typedef struct ColorBox {
	COLORREF color;
	RECT rect;
}ColorBox;
void FillSolarizedColors16(ColorBox* boxes, int beg);
void FillNordColors16(ColorBox* boxes, int beg);
void FillPastelColors16(ColorBox* boxes, int beg);
void ReadRecentColors(ColorBox* boxes, int beg, int N, FILE* fp);
void WriteRecentColors(ColorBox* boxes, int beg, int N, FILE* fp, COLORREF last);
bool isAlreadyExistColor(COLORREF color, ColorBox* boxes, int n);
bool isDefautColor(COLORREF color);
void DrawColorBoxes(ColorBox* boxes, int beg, int sz, char* name, int pos, HDC hdc, RECT rect_palette);
int ColorHitTest(HWND hwnd, ColorBox* boxes, int n);
COLORREF GetLastColor(HWND rhwnd, HWND ghwnd, HWND bhwnd);
DWORD WINAPI MspexColorMemory(LPVOID param);