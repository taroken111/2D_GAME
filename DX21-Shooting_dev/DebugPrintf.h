#ifndef  _DEBUG_PRINTF_H_
#define _DEBUG_PRINTF_H_
#include <windows.h>
#include <stdio.h>
inline void DebugPrintf(LPCSTR pFormat, ...)
{
#if defined(_DEBUG)|| defined(DEBUG)
	va_list argp;
	char strBuf[256]; //���̐��l��������ƕ����ʂ��ς��
	va_start(argp, pFormat);
	vsprintf_s(strBuf, 256, pFormat, argp); //�������ύX����I
	va_end(argp);
	OutputDebugStringA(strBuf);
#endif // ! _DEBUG
}
#endif // ! _DEBUG_PRINTF_H_
