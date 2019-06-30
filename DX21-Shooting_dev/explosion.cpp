//*******************************************************************
//ヘッダインクルード
//*******************************************************************
#include "explosion.h"
#include "SpriteAnimation.h"
#include "sprite.h"
#include "DebugText.h"

//*******************************************************************
//グローバル宣言
//*******************************************************************
static EXPLOSION g_Explosion[MAX_EXPLOSION];

//*******************************************************************
//爆発エフェクト有効化
//*******************************************************************
void Explosion_Create(float x, float y)
{
	for (int i = 0;i < MAX_EXPLOSION;i++)
	{
		if (!g_Explosion[i].bEnable)
		{
			g_Explosion[i].PosX = x;
			g_Explosion[i].PosY = y;

			g_Explosion[i].AnimationTextureXY = SpriteAnimation_Create();
			g_Explosion[i].flamecount = 0;
			g_Explosion[i].bEnable = true;
			break;
		}
	}
}

//*******************************************************************
//爆発エフェクト(砲台用)有効化
//*******************************************************************
void Explosion_Create(float x, float y, float cx, float cy, float angle)
{
	for (int i = 0;i < MAX_EXPLOSION;i++)
	{
		if (!g_Explosion[i].bEnable)
		{
			float bufX, bufY;
			bufX = x - cx;
			bufY = y - cy;
			g_Explosion[i].PosX = bufX * cos(angle) - bufY * sin(angle);
			g_Explosion[i].PosY = bufX * sin(angle) + bufY * cos(angle);
			g_Explosion[i].PosX += cx;
			g_Explosion[i].PosY += cy;
			g_Explosion[i].Angle = angle;
			g_Explosion[i].flamecount = 0;
			g_Explosion[i].bEnable = true;
			g_Explosion[i].AngleSet = true;
			g_Explosion[i].AnimationTextureXY = SpriteAnimation_Create();
			break;
		}
	}
}

//*******************************************************************
//爆発エフェクト初期化
//*******************************************************************
void Explosion_Init(void)
{
	for (int i = 0;i < MAX_EXPLOSION;i++)
	{
		g_Explosion[i].bEnable = false;
		g_Explosion[i].flamecount = 0;
		g_Explosion[i].PosX = 0.0f;
		g_Explosion[i].PosY = 0.0f;
		g_Explosion[i].Angle = 0.0f;
		g_Explosion[i].AngleSet = false;
		g_Explosion[i].AnimationTextureXY = 0;
	}
}

//*******************************************************************
//爆発エフェクト更新
//*******************************************************************
void Explosion_Update(void)
{
	for (int i = 0;i < MAX_EXPLOSION;i++)
	{
		if (g_Explosion[i].bEnable)
		{
			SpriteAnimation_Update(g_Explosion[i].flamecount, ANIMATION_INDEX_EXPLOSION, g_Explosion[i].AnimationTextureXY);
			g_Explosion[i].flamecount++;
			if (g_Explosion[i].flamecount == 75)
			{
				Explosion_Uninit(i);
			}
		}
	}
}

//*******************************************************************
//爆発エフェクト描画
//*******************************************************************
void Explosion_Draw(void)
{
	for (int i = 0;i < MAX_EXPLOSION;i++)
	{
		if (g_Explosion[i].bEnable)
		{
			if(g_Explosion[i].AngleSet)
			{
				Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
				SpriteAnimation_Draw(ANIMATION_INDEX_EXPLOSION, TEXTURE_INDEX_EXPLOSION, g_Explosion[i].AnimationTextureXY, g_Explosion[i].PosX, g_Explosion[i].PosY, g_Explosion[i].PosX, g_Explosion[i].PosY, 1.0f, 1.0f, g_Explosion[i].Angle);
			}
			else
			{
				Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
				SpriteAnimation_Draw(ANIMATION_INDEX_EXPLOSION, TEXTURE_INDEX_EXPLOSION, g_Explosion[i].AnimationTextureXY, g_Explosion[i].PosX, g_Explosion[i].PosY);
			}
		}
	}

#ifdef _DEBUG
#define _DEBUG
	LPD3DXFONT pF = GetFont();
	int ExplosionCount = 0;
	for (int i = 0;i < MAX_EXPLOSION;i++)
	{
		if (g_Explosion[i].bEnable)
		{
			ExplosionCount++;
		}
	}
	DebugText(pF, "\n\nExplosion:%d", ExplosionCount);
#endif
}

//*******************************************************************
//爆発エフェクト終了
//*******************************************************************
void Explosion_Uninit(int index)
{
	g_Explosion[index].bEnable = false;
	g_Explosion[index].AngleSet = false;
	SpriteAnimation_Disable(index);
}

//*******************************************************************
//爆発エフェクト有効無効ゲッタ
//*******************************************************************
bool Explosion_GetEnable(int index)
{
	return g_Explosion[index].bEnable;
}

//*******************************************************************
//爆発エフェクトゲッタ
//*******************************************************************
EXPLOSION* Explosion_GetExplosion(int index)
{
	return &g_Explosion[index];
}