//===================================================
//ヘッダインクルード
//===================================================
#include "judgement.h"
#include "collision.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"
#include "number.h"
#include "DebugPrintf.h"
#include "scene.h"
#include "player.h"
#include "SpriteAnimation.h"
#include "multimissile.h"
#include "item.h"
#include "Map.h"

#define BOOST_UP    (0.2f)

//===================================================
//グローバル変数
//===================================================
//const Circle Enemy, Player;
const Circle *pEnemy, *pPlayer, *pMultiMissile, *pBullet;
const Square *pObject;
const ENEMY *pEnemy_Status;
static PLAYER *pPlayerData;
static const Circle *pBossCollision;
static bool g_NextScene = false;

//===================================================
//更新処理
//===================================================
void Judgement_Init(void)
{
	//pEnemy = &Enemy;
	//pPlayer = &Player;
	pPlayerData = Player_GetPlayer();
}

//===================================================
//更新処理
//===================================================
void Judgement_Update(void)
{

	//アイテムとプレイヤーの当たり判定
	Item_Update();

	//弾と壁の当たり判定
	for (int i = 0;i < MAX_BULLET;i++)
	{
		if (Bullet_GetEnable(i))
		{
			pBullet = Bullet_GetCircleCollision(i);
			for (int j = 0;j < MAX_OBJECT;j++)
			{
				pObject = Map_GetObjectCollision(j);
				Point PosXY;
				PosXY.cx = pBullet->cx;
				PosXY.cy = pBullet->cy;
				if (Collision_IsHitPointToSquare(pObject, &PosXY))
				{
					float ExplosionWidth, ExplosionHeight;
					ExplosionWidth = SpriteAnimation_GetTextureWidth(ANIMATION_INDEX_EXPLOSION);
					ExplosionHeight = SpriteAnimation_GetTextureHeight(ANIMATION_INDEX_EXPLOSION);
					Explosion_Create(pEnemy->cx - (ExplosionWidth / 2), pEnemy->cy - (ExplosionHeight / 2));
					Bullet_Destroy(i);
				}
			}
		}
	}

	//プレイヤーの弾と敵との当たり判定

	for (int i = 0;i < MAX_BULLET;i++)
	{
		//プレイヤーの弾が有効かどうか
		if (Bullet_GetEnable(i) && (Bullet_GetType(i) == TYPE_PLAYER))
		{
			for (int loop = 0;loop < MAX_ENEMY;loop++)
			{
				//敵が有効かどうか
				if (Enemy_GetEnable(loop))
				{
					//プレイヤーの弾のコリジョンと敵のコリジョンとの当たり判定
					pPlayer = Bullet_GetCircleCollision(i);
					pEnemy = Enemy_GetCollision(loop);
					if (Collision_IsHitCircle(pPlayer, pEnemy))
					{
						//当たっていたら、プレイヤーの弾と敵を無効にする
						float ExplosionWidth, ExplosionHeight;
						ExplosionWidth = SpriteAnimation_GetTextureWidth(ANIMATION_INDEX_EXPLOSION);
						ExplosionHeight = SpriteAnimation_GetTextureHeight(ANIMATION_INDEX_EXPLOSION);
						Explosion_Create(pEnemy->cx - (ExplosionWidth / 2), pEnemy->cy - (ExplosionHeight / 2));
						Bullet_Destroy(i);
						Enemy_Disable(loop);
						Item_Create(pEnemy->cx, pEnemy->cy);
						pPlayerData->PlayerScore += 100 * pPlayerData->Score_Boost;
						pPlayerData->Score_Boost += BOOST_UP;
					}
				}
			}

		}
	}

	//敵の弾とプレイヤーとの当たり判定

	for (int i = 0;i < MAX_BULLET;i++)
	{
		//敵の弾が有効かどうか
		if (Bullet_GetEnable(i) && (Bullet_GetType(i) == TYPE_ENEMY))
		{
			//プレイヤーの弾のコリジョンと敵のコリジョンとの当たり判定
			pPlayer = Player_GetCollision();
			pEnemy = Bullet_GetCircleCollision(i);
			if (Collision_IsHitCircle(pPlayer, pEnemy))
			{
				//当たっていたら、プレイヤーの弾と敵を無効にする
				float ExplosionWidth, ExplosionHeight;
				ExplosionWidth = SpriteAnimation_GetTextureWidth(ANIMATION_INDEX_EXPLOSION);
				ExplosionHeight = SpriteAnimation_GetTextureHeight(ANIMATION_INDEX_EXPLOSION);
				Explosion_Create(pEnemy->cx - (ExplosionWidth / 2), pEnemy->cy - (ExplosionHeight / 2));
				Bullet_Destroy(i);
				pPlayerData->HP_Player -= 200;
				if (pPlayerData->HP_Player <= 0)
				{
					Explosion_Create(pPlayer->cx - (ExplosionWidth / 2), pPlayer->cy - (ExplosionHeight / 2));
					pPlayerData->HP_Player += 1000;
					pPlayerData->ReSpawnCount--;
					pPlayerData->Score_Boost = 1.0f;
					if (pPlayerData->ReSpawnCount < 0)
					{
						pPlayerData->EndGame = true;
						pPlayerData->JudgeWinOrLose = true;
					}
				}
			}
		}
	}

	//マルチミサイルの当たり判定

	//弾と壁の当たり判定
	for (int i = 0;i < MULTIMISSILE_MAX;i++)
	{
		if (MultiMissile_GetEnable(i))
		{
			pMultiMissile = MultiMissile_GetCollision(i);
			for (int j = 0;j < MAX_OBJECT;j++)
			{
				pObject = Map_GetObjectCollision(j);
				Point PosXY;
				PosXY.cx = pMultiMissile->cx;
				PosXY.cy = pMultiMissile->cy;
				if (Collision_IsHitPointToSquare(pObject, &PosXY))
				{
					float ExplosionWidth, ExplosionHeight;
					ExplosionWidth = SpriteAnimation_GetTextureWidth(ANIMATION_INDEX_EXPLOSION);
					ExplosionHeight = SpriteAnimation_GetTextureHeight(ANIMATION_INDEX_EXPLOSION);
					Explosion_Create(pEnemy->cx - (ExplosionWidth / 2), pEnemy->cy - (ExplosionHeight / 2));
					MultiMissile_Disable(i);
				}
			}
		}
	}

	for (int i = 0;i < MULTIMISSILE_MAX;i++)
	{
		if (MultiMissile_GetEnable(i))
		{
			for (int loop = 0;loop < MAX_ENEMY;loop++)
			{
				if (Enemy_GetEnable(loop))
				{
					pMultiMissile = MultiMissile_GetCollision(i);
					pEnemy = Enemy_GetCollision(loop);
					if (Collision_IsHitCircle(pMultiMissile, pEnemy))
					{
						//当たっていたら、プレイヤーの弾と敵を無効にする
						float ExplosionWidth, ExplosionHeight;
						ExplosionWidth = SpriteAnimation_GetTextureWidth(ANIMATION_INDEX_EXPLOSION);
						ExplosionHeight = SpriteAnimation_GetTextureHeight(ANIMATION_INDEX_EXPLOSION);
						Explosion_Create(pEnemy->cx - (ExplosionWidth / 2), pEnemy->cy - (ExplosionHeight / 2));
						MultiMissile_Disable(i);
						Enemy_Disable(loop);
						Item_Create(pEnemy->cx, pEnemy->cy);
						pPlayerData->PlayerScore += 100 * pPlayerData->Score_Boost;
						pPlayerData->Score_Boost += BOOST_UP;
					}
				}
			}
		}
	}

	//Score更新
	Number_Update(pPlayerData->PlayerScore);


	//==============================================
	//終了条件
	//==============================================

	//プレイヤーの拠点HPが０になったらもしくはボスの討伐
	if (pPlayerData->EndGame)
	{
		g_NextScene = true;
	}
}

bool Judgement_GetEnable(void)
{
	return g_NextScene;
}

void Judgement_SetDisable(void)
{
	g_NextScene = false;
}