//******************************************
//ヘッダインクルード
//******************************************
#include "bigbomb.h"
#include "player.h"
#include "sprite.h"
#include "Texture.h"

//******************************************
//マクロ定義
//******************************************
#define BOMB_MOVEX (7.0f)
#define BOMB_MOVEY (0.0f)

//******************************************
//グローバル変数
//******************************************
static BIG_BOMB g_BigBimb;
static PLAYER *pPlayer;
static Circle BigBomb;
static Circle *pBigBomb;

//******************************************
//初期化
//******************************************
void BigBomb_Init(void)
{
	g_BigBimb.Enable = false;
	g_BigBimb.Shot = false;
	g_BigBimb.PosX = 0.0f;
	g_BigBimb.PosY = 0.0f;
	g_BigBimb.MoveVec = D3DXVECTOR2(0.0f, 0.0f);
	g_BigBimb.angle = 0.0f;

	pPlayer = Player_GetPlayer();
	pBigBomb = &BigBomb;
}

//******************************************
//更新
//******************************************
void BigBomb_Update(void)
{
	if (g_BigBimb.Enable)
	{
		if (g_BigBimb.Shot)
		{
			//発射されている場合
			g_BigBimb.PosX += g_BigBimb.MoveVec.x;
			g_BigBimb.PosY += g_BigBimb.MoveVec.y;
		}
		else
		{
			//発射されていないとき
			g_BigBimb.PosX = pPlayer->PosX;
			g_BigBimb.PosY = pPlayer->PosY;
			g_BigBimb.angle = pPlayer->HeadAngle;
		}
	}
}

//******************************************
//描画
//******************************************
void BigBomb_Draw(void)
{
	if (g_BigBimb.Enable)
	{
		//発射されている場合とされていない場合に分ける
		if (g_BigBimb.Shot)
		{
			//発射されている場合
			Sprite_Draw(TEXTURE_INDEX_BODY_BOMB, g_BigBimb.PosX, g_BigBimb.PosY, 0, 0, Texture_GetWidth(TEXTURE_INDEX_BODY_BOMB), Texture_GetHeight(TEXTURE_INDEX_BODY_BOMB), false, g_BigBimb.PosX + (Texture_GetWidth(TEXTURE_INDEX_BODY_BOMB) / 2), g_BigBimb.PosY + (Texture_GetHeight(TEXTURE_INDEX_BODY_BOMB) / 2), 1.0f, 1.0f, g_BigBimb.angle);
		}
		else
		{
			//発射されていない場合
			Sprite_Draw(TEXTURE_INDEX_BODY_BOMB, g_BigBimb.PosX, g_BigBimb.PosY, 0, 0, Texture_GetWidth(TEXTURE_INDEX_BODY_BOMB), Texture_GetHeight(TEXTURE_INDEX_BODY_BOMB), false, pPlayer->PosX + TANK_HEAD_CENTER_WIDTH, pPlayer->PosY + TANK_HEAD_CENTER_HEIGHT, 1.0f, 1.0f, g_BigBimb.angle);
		}
	}
}

//******************************************
//作成(有効化)
//******************************************
void BigBomb_Create(void)
{
	g_BigBimb.Enable = true;
	g_BigBimb.PosX = pPlayer->PosX;
	g_BigBimb.PosY = pPlayer->PosY;
	g_BigBimb.MoveVec.x = BOMB_MOVEX * cos(pPlayer->HeadAngle) - BOMB_MOVEY * sin(pPlayer->HeadAngle);
	g_BigBimb.MoveVec.y = BOMB_MOVEX * sin(pPlayer->HeadAngle) + BOMB_MOVEY * cos(pPlayer->HeadAngle);
	g_BigBimb.angle = pPlayer->HeadAngle;
}

//******************************************
//撃った時のフラグを建てる
//******************************************
void BigBomb_Shot(void)
{
	g_BigBimb.MoveVec.x = BOMB_MOVEX * cos(pPlayer->HeadAngle) - BOMB_MOVEY * sin(pPlayer->HeadAngle);
	g_BigBimb.MoveVec.y = BOMB_MOVEX * sin(pPlayer->HeadAngle) + BOMB_MOVEY * cos(pPlayer->HeadAngle);
	g_BigBimb.Shot = true;
}

//******************************************
//有効化取得
//******************************************
bool BigBomb_GetEnable(void)
{
	return g_BigBimb.Enable;
}

//******************************************
//無効化
//******************************************
void BigBomb_Disable(void)
{
	g_BigBimb.Enable = false;
	g_BigBimb.Shot = false;
}

//******************************************
//爆弾のゲッタ
//******************************************
BIG_BOMB* BigBomb_GetBigBimb(void)
{
	return &g_BigBimb;
}

//******************************************
//爆弾の当たり判定
//******************************************
Circle* BigBomb_GetCollision(void)
{
	pBigBomb->cx = g_BigBimb.PosX + (Texture_GetWidth(TEXTURE_INDEX_BODY_BOMB) / 2);
	pBigBomb->cy = g_BigBimb.PosY + (Texture_GetHeight(TEXTURE_INDEX_BODY_BOMB) / 2);
	pBigBomb->radius = (Texture_GetWidth(TEXTURE_INDEX_BODY_BOMB) + Texture_GetHeight(TEXTURE_INDEX_BODY_BOMB)) / 2;
	return pBigBomb;
}