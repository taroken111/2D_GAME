//=============================================================================
//
// キーボード入力処理 [input.h]
//
//=============================================================================
#ifndef INPUT_H_
#define INPUT_H_

#include <Windows.h>
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>


bool Keyboard_Init(HINSTANCE hInstance, HWND hWnd);
void Keyboard_Uninit(void);
void Keyboard_Update(void);

// DIK_～
bool Keyboard_IsPress(int nKey);
bool Keyboard_IsTrigger(int nKey);
bool Keyboard_IsRelease(int nKey);

#endif