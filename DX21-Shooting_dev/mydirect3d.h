//インクルードガード
#ifndef MYDIRECT3D_H_
#define MYDIRECT3D_H_
#include <d3dx9.h>

//プロトタイプ宣言
LPDIRECT3DDEVICE9 MyDirect3D_GetDevice(void);
LPD3DXFONT MyDirect3D_GetFont(void);
bool MyDirect3D_Init(HWND hWnd);
void MyDirect3D_Uninit(void);

#endif
