#include "bullet.h"
#include "sprite.h"
#include "SpriteAnimation.h"
#include "Texture.h"
#include "common.h"
#include "DebugText.h"
#include "player.h"

//*******************************************************************
//マクロ定義
//*******************************************************************
#define BULLET_SPEED_X  (6.0f)
#define BULLET_SPEED_Y  (0.0f)

//*******************************************************************
//グローバル変数
//*******************************************************************
static BULLET_STATUS g_Bullet[MAX_BULLET];
static const PLAYER *pPlayer;
static Circle g_BulletCollision;
static Circle* pBulletCollision;
static D3DXVECTOR2 g_BulletVecSpeed = D3DXVECTOR2(70.0f, 0.0f);

//*******************************************************************
//初期化
//*******************************************************************
void Bullet_Init(void)
{
	for (int i = 0;i < MAX_BULLET;i++)
	{
		g_Bullet[i].bBulletEnable = false;
		g_Bullet[i].flame_count = 0;
		g_Bullet[i].posBulletX = 0.0f;
		g_Bullet[i].posBulletY = 0.0f;
		g_Bullet[i].moveX = 0.0f;
		g_Bullet[i].moveY = 0.0f;
		g_Bullet[i].height = 0.0f;
		g_Bullet[i].width = 0.0f;
		g_Bullet[i].Angle = 0.0f;
		g_Bullet[i].type = TYPE_PLAYER;
		g_Bullet[i].texIndex = TEXTURE_INDEX_BULLET_NORMAL;
	}

	pBulletCollision = &g_BulletCollision;

	pPlayer = Player_GetPlayer();
}

//*******************************************************************
//終了
//*******************************************************************
void Bullet_Uninit(void)
{

}

//*******************************************************************
//更新
//*******************************************************************
void Bullet_Update(void)
{
	for (int i = 0;i < MAX_BULLET;i++)
	{
		if (g_Bullet[i].bBulletEnable)
		{
			g_Bullet[i].flame_count;
			g_Bullet[i].posBulletX += g_Bullet[i].moveX;
			g_Bullet[i].posBulletY += g_Bullet[i].moveY;

			//弾の画面外処理
			if (g_Bullet[i].posBulletX < 0 || g_Bullet[i].posBulletX > SCREEN_WIDTH)
			{
				//とりあえず今回は横移動しかしない弾なので横判定のみ
				Bullet_Destroy(i);
			}
		}
	}
}

//*******************************************************************
//描画
//*******************************************************************
void Bullet_Draw(void)
{
	for (int i = 0;i < MAX_BULLET;i++)
	{
		if (g_Bullet[i].bBulletEnable)
		{
			Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
			Sprite_Draw(g_Bullet[i].texIndex, g_Bullet[i].posBulletX, g_Bullet[i].posBulletY, 0, 0, g_Bullet[i].width, g_Bullet[i].height, false, g_Bullet[i].posBulletX, g_Bullet[i].posBulletY, 1.0f, 1.0f, g_Bullet[i].Angle);
		}
	}

#ifdef _DEBUG
#define _DEBUG
	LPD3DXFONT pF = GetFont();
	int BulletCount = 0;
	for (int i = 0;i < MAX_BULLET;i++)
	{
		if (g_Bullet[i].bBulletEnable)
		{
			BulletCount++;
		}
	}
	DebugText(pF, "\nBullet:%d", BulletCount);
#endif
}

//*******************************************************************
//弾　生成
//*******************************************************************
void Bullet_Create(float x, float y, float angle, TEX_INDEX index, BULLET_TYPE type)
{
	for (int i = 0;i < MAX_BULLET;i++)
	{
		if (!g_Bullet[i].bBulletEnable)
		{

			//テクスチャの設定
			g_Bullet[i].texIndex = index;

			//誰が撃った弾なのか
			g_Bullet[i].type = type;

			//弾の当たり判定
			g_Bullet[i].width = Texture_GetWidth(index);
			g_Bullet[i].height = Texture_GetHeight(index);

			//弾の移動量
			if (g_Bullet[i].type == TYPE_PLAYER)
			{
				float bufX, bufY;
				//回転のために座標をプレイヤーの中心座標分ずらす
				bufX = x - (pPlayer->PosX + TANK_HEAD_CENTER_WIDTH);
				bufY = y - (pPlayer->PosY + TANK_HEAD_CENTER_HEIGHT);

				//回転
				g_Bullet[i].posBulletX = bufX * cos(angle) - bufY * sin(angle);
				g_Bullet[i].posBulletY = bufX * sin(angle) + bufY * cos(angle);

				//ずらした分の座標をもとに戻す
				g_Bullet[i].posBulletX += pPlayer->PosX + TANK_HEAD_CENTER_WIDTH;
				g_Bullet[i].posBulletY += pPlayer->PosY + TANK_HEAD_CENTER_HEIGHT;

				g_Bullet[i].moveX = g_BulletVecSpeed.x * cos(angle) - g_BulletVecSpeed.y * sin(angle);
				g_Bullet[i].moveY = g_BulletVecSpeed.x * sin(angle) + g_BulletVecSpeed.y * cos(angle);
			}
			else
			{
				g_Bullet[i].posBulletX = x;
				g_Bullet[i].posBulletY = y;

				g_Bullet[i].moveX = BULLET_SPEED_X * cos(angle) - BULLET_SPEED_Y * sin(angle);
				g_Bullet[i].moveY = BULLET_SPEED_X * sin(angle) + BULLET_SPEED_Y * cos(angle);
			}

			//回転角の同期
			g_Bullet[i].Angle = angle;

			//弾を有効にする
			g_Bullet[i].bBulletEnable = true;

			break;
		}
	}
}

//*******************************************************************
//弾　消去
//*******************************************************************
void Bullet_Destroy(int index)
{
	g_Bullet[index].bBulletEnable = false;
	g_Bullet[index].flame_count = 0;
}

//*******************************************************************
//弾の当たり判定取得
//*******************************************************************
const Circle* Bullet_GetCircleCollision(int index)
{
	pBulletCollision->cx = g_Bullet[index].posBulletX + (g_Bullet[index].width / 2);
	pBulletCollision->cy = g_Bullet[index].posBulletY + (g_Bullet[index].height / 2);
	pBulletCollision->radius = (g_Bullet[index].width / 2);
	return pBulletCollision;
}

//*******************************************************************
//弾の有効無効用ゲッタ
//*******************************************************************
bool Bullet_GetEnable(int index)
{
	return g_Bullet[index].bBulletEnable;
}

//*******************************************************************
//弾１個のゲッタ
//*******************************************************************
BULLET_STATUS* Bullet_GetBullet(int index)
{
	return &g_Bullet[index];
}

//*******************************************************************
//誰が撃った弾なのか
//*******************************************************************
BULLET_TYPE Bullet_GetType(int index)
{
	return g_Bullet[index].type;
}