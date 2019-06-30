#ifndef FADE_H_
#define FADE_H_

#include <d3dx9.h>

void Fade_Init(void);
void Fade_Update(void);
void Fade_Draw(void);

void Fade_Start(bool bOut, int frame, D3DCOLOR color);           //第一引数：trueだったらフェードアウト
bool Fade_IsFade(void);

#endif
