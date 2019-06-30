#ifndef SPRITE_H_
#define SPRITE_H_
#include <d3dx9.h>

void Sprite_SetColor(D3DCOLOR color);
void Sprite_Draw(int texture_index, float dx, float dy);
void Sprite_DrawWH(int texture_index, float dx, float dy, float width, float height);
void Sprite_Draw(int texture_index, float dx, float dy, float tx, float ty, float tw, float th, bool flip);
void Sprite_Draw(int texture_index, float dx, float dy, float tx, float ty, float tw, float th, bool flip, float cx, float cy, float sx, float sy, float rotation);
void Sprite_Draw(int texture_index, float dx, float dy, float sx, float sy);

void Sprite_DrawBlackToTransparent(int texture_index, float dx, float dy);

#endif