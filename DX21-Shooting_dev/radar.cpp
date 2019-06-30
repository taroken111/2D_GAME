#include "radar.h"
#include "sprite.h"
#include "Texture.h"
#include "player.h"
#include "enemy.h"
#include "collision.h"

//*******************************************************************
//マクロ定義
//*******************************************************************
#define MAX_RADAR          (MAX_ENEMY)

#define RADAR_START_POSX   (1024)
#define RADAR_END_POSX     (1280)
#define RADAR_START_POSY   (464)
#define RADAR_END_POSY     (720)
#define RADAR_CENTER_POSX  (1152)
#define RADAR_CENTER_POSY  (592)
#define RADAR_RADIUS       (1280)
#define RADAR_REDUCTION    (0.1)      //探索範囲から見たレーダーの縮小率

#define RADAR_POINT_WIDTH  (20)
#define RADAR_POINT_HEIGHT (20)

//*******************************************************************
//構造体宣言
//*******************************************************************
typedef struct
{
	float Distance_X;
	float Distance_Y;
	float PosX;
	float PosY;
	bool bEnable;
}RADAR;

//*******************************************************************
//グローバル変数
//*******************************************************************
static PLAYER* pPlayer;
static ENEMY* pEnemy;
static RADAR g_Radar[MAX_RADAR];
static Circle g_CircleRadar;
static Point g_PointRadar;

//*******************************************************************
//レーダー初期化
//*******************************************************************
void Radar_Init(void)
{
	for (int i = 0;i < MAX_RADAR;i++)
	{
		g_Radar[i].PosX = 0.0f;
		g_Radar[i].PosY = 0.0f;
		g_Radar[i].Distance_X = 0.0f;
		g_Radar[i].Distance_Y = 0.0f;
		g_Radar[i].bEnable = false;
	}
}

//*******************************************************************
//レーダー更新
//*******************************************************************
void Radar_Update(void)
{
	float PlayerCX, PlayerCY, EnemyCX, EnemyCY;
	pPlayer = Player_GetPlayer();
	PlayerCX = pPlayer->PosX + (pPlayer->width / 2);
	PlayerCY = pPlayer->PosY + (pPlayer->height / 2);
	g_CircleRadar.cx = PlayerCX;
	g_CircleRadar.cy = PlayerCY;
	g_CircleRadar.radius = RADAR_RADIUS;
	for (int i = 0;i < MAX_ENEMY;i++)
	{
		if (Enemy_GetEnable(i))
		{
			pEnemy = Enemy_GetEnemy(i);
			EnemyCX = pEnemy->PosX + (pEnemy->width / 2);
			EnemyCY = pEnemy->PosY + (pEnemy->height / 2);
			g_PointRadar.cx = EnemyCX;
			g_PointRadar.cy = EnemyCY;
			if (Collision_IsHitPointToCircle(&g_CircleRadar, &g_PointRadar))
			{
				Radar_Create(i);
				g_Radar[i].Distance_X = -(PlayerCX - EnemyCX) * RADAR_REDUCTION;
				g_Radar[i].Distance_Y = -(PlayerCY - EnemyCY) * RADAR_REDUCTION;
				g_Radar[i].PosX = RADAR_CENTER_POSX + g_Radar[i].Distance_X;
				g_Radar[i].PosY = RADAR_CENTER_POSY + g_Radar[i].Distance_Y;
			}
		}
	}
}

//*******************************************************************
//レーダー描画
//*******************************************************************
void Radar_Draw(void)
{
	Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	Sprite_Draw(TEXTURE_INDEX_RADAR, 1024, 464);
	for (int i = 0;i < MAX_RADAR;i++)
	{
		if (g_Radar[i].bEnable)
		{
			Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
			Sprite_Draw(TEXTURE_INDEX_ENEMYRADAR, g_Radar[i].PosX, g_Radar[i].PosY);
		}
	}

	for (int i = 0;i < MAX_RADAR;i++)
	{
		g_Radar[i].bEnable = false;
	}
}

//*******************************************************************
//レーダー作成
//*******************************************************************
void Radar_Create(int index)
{
	g_Radar[index].bEnable = true;
}

//*******************************************************************
//レーダーの無効化
//*******************************************************************
void Radar_Disable(int index)
{
	g_Radar[index].bEnable = false;
}