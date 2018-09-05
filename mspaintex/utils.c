/*
*  utils.c
*  MSPaint-Extension
*
*  Created by kimbomm on 2018. 09. 04...
*  Copyright 2018 kimbomm. All rights reserved.
*
*/
#include"utils.h"
HWND* WINAPI GetInnerHWnds(HWND hwnd,DWORD style,int* n) {
	HWND* hwnds = NULL;
	size_t hwnds_sz = 0;
	HWND tmp = NULL;
	HWND lhwnd = NULL;
	HWND c = 0;
	*n = 0;
	do {
		lhwnd = tmp;
		tmp = FindWindowExA(hwnd, tmp, NULL, NULL);
		DWORD dwStyle = (DWORD)GetWindowLong(lhwnd, GWL_STYLE);
	
		if ((style) == dwStyle) {
			hwnds = realloc(hwnds, ++hwnds_sz*sizeof(HWND));
			(*n)++;
			hwnds[hwnds_sz - 1] = lhwnd;
		}
	} while (tmp);
	return hwnds;
}
HWND* WINAPI GetHwndsByClass(HWND hwnd, char* _class, int* n) {
	HWND* hwnds = NULL;
	size_t hwnds_sz = 0;
	HWND tmp = NULL;
	HWND lhwnd = NULL;
	HWND c = 0;
	*n = 0;
	do {
		lhwnd = tmp;
		tmp = FindWindowExA(hwnd, tmp, NULL, NULL);
		char str[MAX_PATH] = { 0 };
		RealGetWindowClassA(lhwnd, str, MAX_PATH);
		if(strcmp(str,_class)==0){
			hwnds = realloc(hwnds, ++hwnds_sz * sizeof(HWND));
			(*n)++;
			hwnds[hwnds_sz - 1] = lhwnd;
		}
	} while (tmp);
	return hwnds;
}
HWND WINAPI GetInnerHWndsByID(HWND hwnd, DWORD id) {
	HWND* hwnds = NULL;
	size_t hwnds_sz = 0;
	HWND tmp = NULL;
	HWND lhwnd = NULL;
	do {
		lhwnd = tmp;
		tmp = FindWindowExA(hwnd, tmp, NULL, NULL);
		int ctrl_id=GetDlgCtrlID(lhwnd);

		if (id== ctrl_id) {
			return lhwnd;
		}
	} while (tmp);
	return NULL;
}
static ThreadManage* mspex_head = NULL;
ThreadManage* MspexCreateThread(DWORD(WINAPI*f)(LPVOID), HWND hwnd) {
	DWORD tid = 0;
	ThreadManage* n = mspex_head;
	while (n) {
		if (n->hwnd == hwnd) {
			return NULL;
		}
		n = n->next;
	}
	ThreadManage* tm = (ThreadManage*)malloc(sizeof(ThreadManage));
	tm->hwnd = hwnd;
	tm->exitcode = -1;
	tm->next = NULL;
	tm->tid = CreateThread(NULL, 0, f, tm, 0, &tid);
#ifdef _DEBUG
	printf("%s : hwnd(%zd) created\n", __FUNCTION__, hwnd);
#endif
	return tm;
}

void MspexUpdateThread(ThreadManage* tm) {
	if (tm) {
		tm->next = mspex_head;
		mspex_head = tm;
	}
	ThreadManage** phead = &mspex_head;
	while (*phead) {
		ThreadManage* tmp = *phead;
		bool b = (**phead).exitcode != -1;
		if (b) 
			*phead = (**phead).next;
		if(*phead)
			phead = &(**phead).next;
		if (b) {
			CloseHandle(tmp->tid);
			free(tmp);
		}
	}

}