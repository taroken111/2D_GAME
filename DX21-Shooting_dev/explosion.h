#ifndef EXPLOSION_H_
#define EXPLOSION_H_

//*******************************************************************
//マクロ定義
//*******************************************************************
#define MAX_EXPLOSION   (30)

//*******************************************************************
//構造体宣言
//*******************************************************************
typedef struct
{
	float PosX;
	float PosY;
	float Angle;
	int flamecount;
	bool bEnable;
	bool AngleSet;
	int AnimationTextureXY;
}EXPLOSION;

//*******************************************************************
//プロトタイプ宣言
//*******************************************************************
void Explosion_Create(float x, float y);
void Explosion_Create(float x, float y, float cx, float cy, float angle);
void Explosion_Init(void);
void Explosion_Update(void);
void Explosion_Draw(void);
void Explosion_Uninit(int index);
bool Explosion_GetEnable(int index);
EXPLOSION* Explosion_GetExplosion(int index);

#endif
