//**********************************************************
//ヘッダインクルード
//**********************************************************
#include "item.h"
#include "player.h"
#include "collision.h"
#include "sprite.h"
#include "Texture.h"
#include <stdlib.h>
#include <time.h>
#include "bigbomb.h"

//**********************************************************
//グローバル変数
//**********************************************************
static ITEM g_Item[MAX_ITEM];
static Circle ItemCollision;
static Circle *pItemCollision;
static PLAYER *pPlayer;
static Circle *pPlayerCollision;


//**********************************************************
//初期化
//**********************************************************
void Item_Init(void)
{
	for (int i = 0;i < MAX_ITEM;i++)
	{
		g_Item[i].Enable = false;
		g_Item[i].PosX = 0.0f;
		g_Item[i].PosY = 0.0f;
		g_Item[i].Type = ITEM_TYPE_MULTIMISSILE;
	}

	pItemCollision = &ItemCollision;
	pPlayer = Player_GetPlayer();
}

//**********************************************************
//更新
//**********************************************************
void Item_Update(void)
{
	pPlayerCollision = Player_GetCollision();
	for (int i = 0;i < MAX_ITEM;i++)
	{
		if (g_Item[i].Enable)
		{
			pItemCollision = Item_GetCollision(i);
			if (Collision_IsHitCircle(pPlayerCollision, pItemCollision))
			{
				Item_Disable(i);

				//マルチミサイルのアイテムを拾った場合
				if (g_Item[i].Type == ITEM_TYPE_MULTIMISSILE)
				{
					pPlayer->BulletType_Normal = false;
					pPlayer->BulletType_Bomb = false;
					pPlayer->BulletType_MultiMissile = true;
					pPlayer->PlayerScore += 50 * pPlayer->SpeedBooster;
				}

				//爆弾のアイテムを拾った場合
				//if (g_Item[i].Type == ITEM_TYPE_BOMB)
				//{
				//	pPlayer->BulletType_Normal = false;
				//	pPlayer->BulletType_MultiMissile = false;
				//	pPlayer->BulletType_Bomb = true;
				//	BigBomb_Create();

				//}

				//スピードアップのアイテムを拾った場合
				if (g_Item[i].Type == ITEM_TYPE_SPEED)
				{
					if (pPlayer->HowManySpeedUp < 5)
					{
						pPlayer->SpeedBooster += 0.5f;
						pPlayer->HowManySpeedUp++;
						pPlayer->PlayerScore += 50 * pPlayer->SpeedBooster;
					}
				}

				//クールダウンのアイテムを拾った場合
				if (g_Item[i].Type == ITEM_TYPE_COOL)
				{
					if (pPlayer->HowManyCoolDown < 5)
					{
						pPlayer->CoolTime -= 36.0f;
						pPlayer->HowManyCoolDown++;
						pPlayer->PlayerScore += 50 * pPlayer->SpeedBooster;
					}
				}
			}
		}
	}
}

//**********************************************************
//描画
//**********************************************************
void Item_Draw(void)
{
	for (int i = 0;i < MAX_ITEM;i++)
	{
		if (g_Item[i].Enable)
		{
			if (g_Item[i].Type == ITEM_TYPE_MULTIMISSILE)
			{
				Sprite_Draw(TEXTURE_INDEX_ITEM_MULTIMISSILE, g_Item[i].PosX, g_Item[i].PosY);
			}

			if (g_Item[i].Type == ITEM_TYPE_SPEED)
			{
				Sprite_Draw(TEXTURE_INDEX_ITEM_SPEED, g_Item[i].PosX, g_Item[i].PosY);
			}

			if (g_Item[i].Type == ITEM_TYPE_COOL)
			{
				Sprite_Draw(TEXTURE_INDEX_ITEM_COOL, g_Item[i].PosX, g_Item[i].PosY);
			}

			if (g_Item[i].Type == ITEM_TYPE_BOMB)
			{
				Sprite_Draw(TEXTURE_INDEX_ITEM_BOMB, g_Item[i].PosX, g_Item[i].PosY);
			}
		}
	}
}

//**********************************************************
//作成
//**********************************************************
void Item_Create(float x, float y)
{
	for (int i = 0;i < MAX_ITEM;i++)
	{
		if (!g_Item[i].Enable)
		{
			//座標入力と有効化処理
			g_Item[i].PosX = x;
			g_Item[i].PosY = y;
			g_Item[i].Enable = true;

			//アイテムガチャ用乱数生成
			srand((unsigned)time(NULL));
			int buf = rand() % 100;

			//========================
			//発生した乱数の場合分け
			//========================

			//20%の確率でマルチミサイル
			if (buf >= 0 && buf < 20)
			{
				g_Item[i].Type = ITEM_TYPE_MULTIMISSILE;
			}

			//10%の確率で爆弾
			//if (buf >= 20 && buf < 30)
			//{
			//	g_Item[i].Type = ITEM_TYPE_BOMB;
			//}

			//30%の確率でスピードアップ
			if (buf >= 20 && buf < 50)
			{
				g_Item[i].Type = ITEM_TYPE_SPEED;
			}

			//40%の確率でクールダウン
			if (buf >= 50 && buf < 100)
			{
				g_Item[i].Type = ITEM_TYPE_COOL;
			}

			break;
		}
	}
}

//**********************************************************
//無効化
//**********************************************************
void Item_Disable(int index)
{
	g_Item[index].Enable = false;
}

//**********************************************************
//当たり判定取得
//**********************************************************
Circle* Item_GetCollision(int index)
{
	pItemCollision->cx = g_Item[index].PosX + 16.0f;
	pItemCollision->cy = g_Item[index].PosY + 16.0f;
	pItemCollision->radius = 16.0f;
	return pItemCollision;
}

//**********************************************************
//有効無効取得
//**********************************************************
bool Item_GetEnable(int index)
{
	return g_Item[index].Enable;
}

//**********************************************************
//ゲッタ
//**********************************************************
ITEM* Item_GetItem(int index)
{
	return &g_Item[index];
}