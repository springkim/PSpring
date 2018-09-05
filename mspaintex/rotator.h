/*
*  rotator.h
*  MSPaint-Extension
*
*  Created by kimbomm on 2018. 09. 04...
*  Copyright 2018 kimbomm. All rights reserved.
*
*/
#pragma once
#include<stdio.h>
#include<math.h>
#include"utils.h"
#include"protractor_resource.h"
#pragma comment(lib,"Msimg32.lib")
#pragma comment(lib,"kernel32.lib")

void DrawProtector(HDC hdc, HWND hwnd, RECT rect);
POINT AnglePoint(POINT base, int zeroh, int angle);
DWORD WINAPI MspexRotator(LPVOID param);