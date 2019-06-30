#ifndef MULTIMISSILE_H_
#define MULTIMISSILE_H_

//*******************************************************************
//ヘッダインクルード
//*******************************************************************
#include <d3dx9.h>
#include "enemy.h"
#include "collision.h"

//*******************************************************************
//マクロ定義
//*******************************************************************
#define MULTIMISSILE_MAX (8)

//*******************************************************************
//構造体宣言
//*******************************************************************
typedef struct
{
	float PosX;
	float PosY;
	D3DXVECTOR2 vecMoveXY;
	float Angle;
	bool Enable;
	ENEMY* pEnemy;
}BULLET_MULTIMISSILE;

//*******************************************************************
//プロトタイプ宣言
//*******************************************************************
void MultiMissile_Init(void);
void MultiMissile_Update(void);
void MultiMissile_Draw(void);
void MultiMissile_Uninit(void);

void MultiMissile_Create(float x, float y);
void MultiMissile_Disable(int index);
BULLET_MULTIMISSILE* MultiMissile_GetMultiMissile(int index);
bool MultiMissile_GetEnable(int index);
Circle* MultiMissile_GetCollision(int index);


#endif // !MULTIMISSILE_H_

