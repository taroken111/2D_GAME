#ifndef EFFECT_H_
#define EFFECT_H_

#include <d3dx9.h>

void Effect_Init(void);
void Effect_Update(void);
void Effect_Draw(void);
void Effect_Uninit(void);

void Effect_Create(float x, float y, int life, float radius, D3DCOLOR color);    //第三引数：エフェクトの寿命, 第四引数：ポリゴンの対角線の半分

#endif
