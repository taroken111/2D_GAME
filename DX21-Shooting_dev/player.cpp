#include "player.h"
#include "Texture.h"
#include "sprite.h"
#include "input.h"
#include "SpriteAnimation.h"
#include <d3dx9.h>
#include "common.h"
#include "bullet.h"
#include "explosion.h"
#include "effect.h"
#include "sound.h"
#include "multimissile.h"
#include "collision.h"
#include "bigbomb.h"
#include "menu.h"
#include "map.h"

//*******************************************************************
//マクロ定義
//*******************************************************************
#define PI (3.141592653f)
#define ROTATION_SPEED (3.0f)
#define REMAIN_BULLET_POSX (16.0f)
#define REMAIN_BULLET_POSY (656.0f)
#define COOL_TIME          (180)
#define HP_CITY_MAX        (5000.0f)
#define HP_PLAYER_MAX      (1000.0f)
#define GAIN_CITY_HP       (0.5f)                  //拠点のHP減少率
#define MAX_SCORE_BOOST    (10.0f)

//*******************************************************************
//グローバル変数
//*******************************************************************
static PLAYER g_Player;
static Circle PlayerCollision;
static Circle *pPlayerCollision;
static Square_Rotate *pPlayerSquareRotate;
static Square_Rotate PlayerSquareRotate;

//残弾数管理用
int CoolTime;
float alpha;

//めんどくさい
bool right;
bool left;
bool LRnone;


void Player_Init(void)
{
	g_Player.PosX = 100.0f;
	g_Player.PosY = 100.0f;
	g_Player.Deg = 0.0f;
	g_Player.HeadDeg = 0.0f;
	g_Player.Angle = 0.0f;
	g_Player.HeadAngle = 0.0f;
	g_Player.width = Texture_GetWidth(TEXTURE_INDEX_TANK_BODY);
	g_Player.height = Texture_GetHeight(TEXTURE_INDEX_TANK_BODY);
	g_Player.Speed = 2.0f;
	g_Player.HP_Player = 1000.0f;
	g_Player.HP_City = 5000.0f;

	g_Player.HowManyCoolDown = 0;
	g_Player.HowManySpeedUp = 0;
	g_Player.SpeedBooster = 1.0f;

	g_Player.Score_Boost = 1.0f;
	g_Player.PlayerScore = 0;
	g_Player.ReSpawnCount = 3;

	g_Player.vecSpeed = D3DXVECTOR2(0.0f, 0.0f);

	g_Player.EndGame = false;
	g_Player.JudgeWinOrLose = false;
	g_Player.OpenMenu = false;

	g_Player.BulletType_Bomb = false;
	g_Player.BulletType_MultiMissile = false;
	g_Player.BulletType_Normal = true;

	g_Player.CoolTime = 180;
	CoolTime = g_Player.CoolTime;
	alpha = 1.0f;
	right = false;
	left = false;
	LRnone = false;

	pPlayerCollision = &PlayerCollision;
	pPlayerSquareRotate = &PlayerSquareRotate;
}

void Player_Update(void)
{
	//===============================================
	//本体コントロール
	//===============================================
	if (Keyboard_IsPress(DIK_D))
	{
		g_Player.Deg += ROTATION_SPEED;
		g_Player.HeadDeg += ROTATION_SPEED;
	}

	if (Keyboard_IsPress(DIK_S))
	{
		g_Player.vecSpeed += D3DXVECTOR2(-g_Player.SpeedBooster, 0.0f);
	}

	if (Keyboard_IsPress(DIK_A))
	{
		g_Player.Deg -= ROTATION_SPEED;
		g_Player.HeadDeg -= ROTATION_SPEED;
	}

	if (Keyboard_IsPress(DIK_W))
	{
		g_Player.vecSpeed += D3DXVECTOR2(g_Player.SpeedBooster, 0.0f);
	}

	//===============================================
	//主砲コントロール
	//===============================================
	if (Keyboard_IsPress(DIK_LEFT))
	{
		g_Player.HeadDeg -= ROTATION_SPEED;
		right = true;
		left = false;
		LRnone = false;
	}

	if (Keyboard_IsPress(DIK_RIGHT))
	{
		g_Player.HeadDeg += ROTATION_SPEED;
		right = false;
		left = true;
		LRnone = false;
	}

	if (!Keyboard_IsPress(DIK_RIGHT) && !Keyboard_IsPress(DIK_LEFT))
	{
		right = false;
		left = false;
		LRnone = true;
	}

	//===============================================
	//角度、ラジアン変換
	//===============================================
	g_Player.Angle = (g_Player.Deg * PI) / 180;
	g_Player.HeadAngle = (g_Player.HeadDeg * PI) / 180;

	//===============================================
	//その他、キー操作
	//===============================================

	//弾発射
	if (Keyboard_IsTrigger(DIK_SPACE))
	{
		if (CoolTime >= g_Player.CoolTime)
		{
			CoolTime -= 1;
			alpha = 0.0f;

			//弾が通常弾である場合
			if (g_Player.BulletType_Normal)
			{
				Bullet_Create(g_Player.PosX + g_Player.width, g_Player.PosY + (g_Player.height / 2) - 8, g_Player.HeadAngle, TEXTURE_INDEX_BULLET_NORMAL, TYPE_PLAYER);
				Explosion_Create(g_Player.PosX + g_Player.width - 64, g_Player.PosY + (g_Player.height / 2) - 64, g_Player.PosX + TANK_HEAD_CENTER_WIDTH, g_Player.PosY + TANK_HEAD_CENTER_HEIGHT, g_Player.HeadAngle);
				PlaySound(SOUND_LABEL_SE_EXPLOSION);
			}

			//弾がマルチミサイルである場合
			if (g_Player.BulletType_MultiMissile)
			{
				MultiMissile_Create(g_Player.PosX, g_Player.PosY);
				g_Player.BulletType_MultiMissile = false;
				g_Player.BulletType_Normal = true;
			}

			//弾が爆弾である場合
			if (g_Player.BulletType_Bomb)
			{
				BigBomb_Shot();
				g_Player.BulletType_MultiMissile = false;
				g_Player.BulletType_Normal = true;
			}
		}
	}

	//メニューを開く(Eキー)
	//if (Keyboard_IsTrigger(DIK_M))
	//{
	//	g_Player.OpenMenu = true;
	//	Menu_Loop();
	//}

	//===============================================
	//移動量制限
	//===============================================
	if (!Keyboard_IsPress(DIK_W) && !Keyboard_IsPress(DIK_A) && !Keyboard_IsPress(DIK_S) && !Keyboard_IsPress(DIK_D))
	{
		g_Player.vecSpeed *= 0.8f;
	}

	if (Keyboard_IsPress(DIK_W) || Keyboard_IsPress(DIK_A) || Keyboard_IsPress(DIK_S) || Keyboard_IsPress(DIK_D))
	{
		g_Player.vecSpeed *= 0.8f;
	}

	//===============================================
	//速度ベクトル変換
	//===============================================
	float px = g_Player.vecSpeed.x * cos(g_Player.Angle) - g_Player.vecSpeed.y * sin(g_Player.Angle);
	float py = g_Player.vecSpeed.x * sin(g_Player.Angle) + g_Player.vecSpeed.y * cos(g_Player.Angle);

	//D3DXVec2Normalize(&vecSpeed, &vecSpeed);
	//vecSpeed *= g_PlayerSpeed;

	g_Player.PosX += px;
	g_Player.PosY += py;

	//===============================================
	//移動制限
	//===============================================
	//オブジェクトとの衝突
	for (int i = 0;i < MAX_OBJECT;i++)
	{
		if (Collision_IsHitStage(Map_GetObjectCollision(i), Player_GetSquareRotate()))
		{
			g_Player.PosX -= px;
			g_Player.PosY -= py;
			if (right)
			{
				g_Player.Deg += ROTATION_SPEED;
				g_Player.Angle = (g_Player.Deg * PI) / 180;
			}

			if(left)
			{
				g_Player.Deg -= ROTATION_SPEED;
				g_Player.Angle = (g_Player.Deg * PI) / 180;
			}
		}

		//if (Collision_IsHitCircle(Player_GetCollision(), Map_GetObjectCollision(i)))
		//{
		//	g_Player.PosX -= px;
		//	g_Player.PosY -= py;
		//}
	}

	//画面外判定
	if (g_Player.PosX < 0)
	{
		g_Player.PosX = 0.0f;
	}

	if (g_Player.PosX + g_Player.width > SCREEN_WIDTH)
	{
		g_Player.PosX = SCREEN_WIDTH - g_Player.width;
	}

	if (g_Player.PosY < 0)
	{
		g_Player.PosY = 0.0f;
	}

	if (g_Player.PosY + g_Player.height > SCREEN_HEIGHT)
	{
		g_Player.PosY = SCREEN_HEIGHT - g_Player.height;
	}

	//TEST:「エフェクト」
	//Effect_Create(g_Player.PosX + (g_Player.width / 2), g_Player.PosY + (g_Player.height / 2), 60, 128.0f, D3DCOLOR_RGBA(128, 64, 64, 255));

	//===============================================
	//主砲発射制限
	//===============================================
	if (CoolTime != g_Player.CoolTime)
	{
		CoolTime--;
		alpha += 0.0055;
		alpha = min(alpha, 1.0f);
	}

	if (CoolTime <= 0)
	{
		CoolTime = g_Player.CoolTime;
	}

	//拠点のHP減少
	g_Player.HP_City -= GAIN_CITY_HP;

	//スコア上昇率制御
	if (g_Player.Score_Boost > 10.0f)
	{
		g_Player.Score_Boost = 10.0f;
	}
}

void Player_Draw(void)
{
	Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	Sprite_Draw(TEXTURE_INDEX_TANK_BODY, g_Player.PosX, g_Player.PosY, 0, 0, g_Player.width, g_Player.height, false, g_Player.PosX + TANK_HEAD_CENTER_WIDTH, g_Player.PosY + TANK_HEAD_CENTER_HEIGHT, 1.0f, 1.0f, g_Player.Angle);
	Sprite_Draw(TEXTURE_INDEX_TANK_HEAD, g_Player.PosX, g_Player.PosY, 0, 0, g_Player.width, g_Player.height, false, g_Player.PosX + TANK_HEAD_CENTER_WIDTH, g_Player.PosY + TANK_HEAD_CENTER_HEIGHT, 1.0f, 1.0f, g_Player.HeadAngle);

	int Alpha;
	Alpha = 255 * alpha;
	Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, Alpha));
	if (g_Player.BulletType_Normal)
	{
		Sprite_Draw(TEXTURE_INDEX_ITEM_NORMAL, REMAIN_BULLET_POSX, REMAIN_BULLET_POSY);
	}
	if (g_Player.BulletType_MultiMissile)
	{
		Sprite_DrawWH(TEXTURE_INDEX_ITEM_MULTIMISSILE, REMAIN_BULLET_POSX, REMAIN_BULLET_POSY, 64, 64);
	}
	Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	//--------------------------------------------------------------------------------------------------------------------------------------------
	//====================================
	//UI関係はここに記述
	//====================================
	
	//HPバー関係
	Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	Sprite_Draw(TEXTURE_INDEX_HP_GAUGE, 144, 16);
	Sprite_Draw(TEXTURE_INDEX_HP_PLAYER, 144, 16, 0, 0, Texture_GetWidth(TEXTURE_INDEX_HP_PLAYER) * (g_Player.HP_Player / HP_PLAYER_MAX), Texture_GetHeight(TEXTURE_INDEX_HP_PLAYER), false);
	Sprite_Draw(TEXTURE_INDEX_HP_NAME, 16, 16);
	Sprite_Draw(TEXTURE_INDEX_SB_NAME, 16, 48);
	Sprite_Draw(TEXTURE_INDEX_HG_1, 144, 48);
	Sprite_Draw(TEXTURE_INDEX_HG_2, 144, 48, 0, 0, Texture_GetWidth(TEXTURE_INDEX_HG_2) * (g_Player.Score_Boost / MAX_SCORE_BOOST), Texture_GetHeight(TEXTURE_INDEX_HG_2), false);
	Sprite_Draw(TEXTURE_INDEX_HP_NAME, 16, 16);
	Sprite_Draw(TEXTURE_INDEX_HG_3, 144, 48);
	Sprite_DrawWH(TEXTURE_INDEX_ITEM_SPEED, 96, 688, 32, 32);
	Sprite_DrawWH(TEXTURE_INDEX_ITEM_COOL, 144, 688, 32, 32);
	Sprite_DrawWH(TEXTURE_INDEX_HP_ITEM, 96, 528 + (32 * (5 - g_Player.HowManySpeedUp)), 32, 160 - (32 * (5 - g_Player.HowManySpeedUp)));
	Sprite_DrawWH(TEXTURE_INDEX_HP_ITEM, 144, 528 + (32 * (5 - g_Player.HowManyCoolDown)), 32, 160 - (32 * (5 - g_Player.HowManyCoolDown)));

	for (int i = 0;i < g_Player.ReSpawnCount;i++)
	{
		Sprite_Draw(TEXTURE_INDEX_LIFE, 16 + (32 * i), 80);
	}



	//--------------------------------------------------------------------------------------------------------------------------------------------

	//====================================
	//マルチミサイルのボディ
	//====================================
	if (g_Player.BulletType_MultiMissile)
	{
		Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
		float width = Texture_GetWidth(TEXTURE_INDEX_BODY_MULTIMISSILE);
		float height = Texture_GetHeight(TEXTURE_INDEX_BODY_MULTIMISSILE);
		Sprite_Draw(TEXTURE_INDEX_BODY_MULTIMISSILE, g_Player.PosX, g_Player.PosY, 0, 0, width, height, false, g_Player.PosX + TANK_HEAD_CENTER_WIDTH, g_Player.PosY + TANK_HEAD_CENTER_HEIGHT, 1.0f, 1.0f, g_Player.HeadAngle);
	}
}

PLAYER* Player_GetPlayer(void)
{
	return &g_Player;
}

Circle* Player_GetCollision(void)
{
	float bufX, bufY;
	pPlayerCollision->radius = Texture_GetHeight(TEXTURE_INDEX_TANK_BODY) / 2;
	bufX = Texture_GetWidth(TEXTURE_INDEX_TANK_BODY) / 2;
	bufY = pPlayerCollision->radius;
	pPlayerCollision->cx = g_Player.PosX + bufX;
	pPlayerCollision->cy = g_Player.PosY + bufY;
	return pPlayerCollision;
}

Square_Rotate* Player_GetSquareRotate(void)
{
	pPlayerSquareRotate->SquarePos.top_left.cx = g_Player.PosX + 3;
	pPlayerSquareRotate->SquarePos.top_left.cy = g_Player.PosY + 33;

	pPlayerSquareRotate->SquarePos.top_right.cx = g_Player.PosX + 3 + 178;
	pPlayerSquareRotate->SquarePos.top_right.cy = g_Player.PosY + 33;

	pPlayerSquareRotate->SquarePos.bottom_left.cx = g_Player.PosX + 3;
	pPlayerSquareRotate->SquarePos.bottom_left.cy = g_Player.PosY + 33 + 90;

	pPlayerSquareRotate->SquarePos.bottom_right.cx = g_Player.PosX + 3 + 178;
	pPlayerSquareRotate->SquarePos.bottom_right.cy = g_Player.PosY + 33 + 90;

	pPlayerSquareRotate->cx = g_Player.PosX + TANK_HEAD_CENTER_WIDTH;
	pPlayerSquareRotate->cy = g_Player.PosY + TANK_HEAD_CENTER_HEIGHT;

	pPlayerSquareRotate->rotate = g_Player.Angle;

	return pPlayerSquareRotate;
}