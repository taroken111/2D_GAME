#include "multimissile.h"
#include "player.h"
#include "sprite.h"
#include "Texture.h"
#include "DebugText.h"

//*******************************************************************
//マクロ定義
//*******************************************************************
#define MULTIMISSILE_MOVE_X (6.0f)
#define MULTIMISSILE_MOVE_Y (0.0f)

//*******************************************************************
//構造体宣言
//*******************************************************************


//*******************************************************************
//グローバル変数
//*******************************************************************
BULLET_MULTIMISSILE g_MultiMissile[MULTIMISSILE_MAX];
static const PLAYER *pPlayer;
Circle MultiMissileCollision;
Circle *pMultiMissileCollision;

//*******************************************************************
//初期化
//*******************************************************************
void MultiMissile_Init(void)
{
	for (int i = 0;i < MULTIMISSILE_MAX;i++)
	{
		g_MultiMissile[i].PosX = 0.0f;
		g_MultiMissile[i].PosY = 0.0f;
		g_MultiMissile[i].vecMoveXY = D3DXVECTOR2(0.0f, 0.0f);
		g_MultiMissile[i].Angle = 0.0f;
		g_MultiMissile[i].Enable = false;
		g_MultiMissile[i].pEnemy = NULL;
	}
	pMultiMissileCollision = &MultiMissileCollision;

	pPlayer = Player_GetPlayer();
}

//*******************************************************************
//更新
//*******************************************************************
void MultiMissile_Update(void)
{
	float BufX = 0.0f;
	float BufY = 0.0f;
	for (int i = 0;i < MULTIMISSILE_MAX;i++)
	{
		if (g_MultiMissile[i].Enable)
		{
			BufX = g_MultiMissile[i].pEnemy->PosX + (g_MultiMissile[i].pEnemy->width / 2) - g_MultiMissile[i].PosX;
			BufY = g_MultiMissile[i].pEnemy->PosY + (g_MultiMissile[i].pEnemy->height / 2) - g_MultiMissile[i].PosY;

			//atan2fがXY座標がともに0の時のエラーを防ぐ
			if ((BufX >= 0.0f && BufX < 1.0f) && (BufY >= 0.0f && BufY < 1.0f))
			{
				BufX = 1.0f;
				BufY = 1.0f;
			}

			//目標が撃破されたら消滅する
			if (!g_MultiMissile[i].pEnemy->Enable)
			{
				MultiMissile_Disable(i);
			}

			g_MultiMissile[i].Angle = atan2f(BufY, BufX);
			g_MultiMissile[i].vecMoveXY.x = MULTIMISSILE_MOVE_X * cos(g_MultiMissile[i].Angle) - MULTIMISSILE_MOVE_Y * sin(g_MultiMissile[i].Angle);
			g_MultiMissile[i].vecMoveXY.y = MULTIMISSILE_MOVE_X * sin(g_MultiMissile[i].Angle) + MULTIMISSILE_MOVE_Y * cos(g_MultiMissile[i].Angle);
			g_MultiMissile[i].PosX += g_MultiMissile[i].vecMoveXY.x;
			g_MultiMissile[i].PosY += g_MultiMissile[i].vecMoveXY.y;
		}
	}
}

//*******************************************************************
//描画
//*******************************************************************
void MultiMissile_Draw(void)
{
	for (int i = 0;i < MULTIMISSILE_MAX;i++)
	{
		if (g_MultiMissile[i].Enable)
		{
			Sprite_Draw(TEXTURE_INDEX_BULLET_MULTIMISSILE, g_MultiMissile[i].PosX, g_MultiMissile[i].PosY, 0, 0, Texture_GetWidth(TEXTURE_INDEX_BULLET_MULTIMISSILE), Texture_GetHeight(TEXTURE_INDEX_BULLET_MULTIMISSILE), false, g_MultiMissile[i].PosX + 8.0f, g_MultiMissile[i].PosY + 8.0f, 1.0f, 1.0f, g_MultiMissile[i].Angle);
			Sprite_Draw(TEXTURE_INDEX_RETICLE_MULTIMISSILE, g_MultiMissile[i].pEnemy->PosX, g_MultiMissile[i].pEnemy->PosY);
		}
	}

#ifdef _DEBUG
#define _DEBUG
	LPD3DXFONT pF = GetFont();
	int Count = 0;
	for (int i = 0;i < MULTIMISSILE_MAX;i++)
	{
		if (g_MultiMissile[i].Enable)
		{
			Count++;
		}
	}
	DebugText(pF, "\n\n\nMultiMissile:%d", Count);
#endif
}

//*******************************************************************
//終了
//*******************************************************************
void MultiMissile_Uninit(void)
{

}

//*******************************************************************
//作成
//*******************************************************************
void MultiMissile_Create(float x, float y)
{
	int cnt = 0;
	int cntOld = 0;
	float posX;
	float posY;
	float distance;
	bool shot = false;
	ENEMY *pEnemy;
	for (int i = 0;i < MULTIMISSILE_MAX;i++)
	{
		if (!g_MultiMissile[i].Enable)
		{
			for (cnt = cntOld;cnt < MAX_ENEMY;cnt++)
			{
				if (Enemy_GetEnable(cnt))
				{
					cntOld = cnt + 1;
					pEnemy = Enemy_GetEnemy(cnt);
					posX = pEnemy->PosX - pPlayer->PosX;
					posY = pEnemy->PosY - pPlayer->PosY;
					distance = (posX * posX) + (posY * posY);
					if (distance < 928000)
					{
						shot = true;
						g_MultiMissile[i].PosX = x;
						g_MultiMissile[i].PosY = y;
						g_MultiMissile[i].pEnemy = pEnemy;
						g_MultiMissile[i].Enable = true;
						break;
					}
				}

				if (shot)
				{
					if (cnt == (MAX_ENEMY - 1))
					{
						cntOld = 0;
						cnt = 0;
					}
				}
			}
		}
	}
}

//*******************************************************************
//無効化
//*******************************************************************
void MultiMissile_Disable(int index)
{
	g_MultiMissile[index].Enable = false;
}

//*******************************************************************
//ゲッタ
//*******************************************************************
BULLET_MULTIMISSILE* MultiMissile_GetMultiMissile(int index)
{
	return &g_MultiMissile[index];
}

//*******************************************************************
//ミサイルの有効無効取得
//*******************************************************************
bool MultiMissile_GetEnable(int index)
{
	return g_MultiMissile[index].Enable;
}

//*******************************************************************
//ミサイルの当たり判定取得
//*******************************************************************
Circle* MultiMissile_GetCollision(int index)
{
	float BufX, BufY;
	BufX = Texture_GetWidth(TEXTURE_INDEX_BULLET_MULTIMISSILE) / 2;
	BufY = Texture_GetHeight(TEXTURE_INDEX_BULLET_MULTIMISSILE) / 2;
	pMultiMissileCollision->cx = BufX * cos(g_MultiMissile[index].Angle) - BufY * sin(g_MultiMissile[index].Angle) + g_MultiMissile[index].PosX;
	pMultiMissileCollision->cy = BufX * sin(g_MultiMissile[index].Angle) + BufY * cos(g_MultiMissile[index].Angle) + g_MultiMissile[index].PosY;
	pMultiMissileCollision->radius = Texture_GetWidth(TEXTURE_INDEX_BULLET_MULTIMISSILE) / 2;

	return pMultiMissileCollision;
}
