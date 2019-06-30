#ifndef  _DEBUG_TEXT_H_
#define _DEBUG_TEXT_H_
#include <windows.h>
#include <stdio.h>
#include <d3dx9.h>
#include "common.h"

LPD3DXFONT GetFont(void);   //mainで内容を記述

inline void DebugText(LPD3DXFONT pF, LPCSTR pText, ...)
{
#if defined(_DEBUG)|| defined(DEBUG)
	va_list argp;
	char strBuf[256]; //この数値をいじると文字量が変わる
	va_start(argp, pText);
	vsprintf_s(strBuf, 256, pText, argp); //ここも変更する！
	va_end(argp);
	OutputDebugStringA(strBuf);
	RECT rect;
	rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	pF->DrawText(NULL, strBuf, -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0, 255, 0, 255));

#endif // ! _DEBUG
}
#endif // ! _DEBUG_TEXT_H_
