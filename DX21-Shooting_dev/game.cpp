#include "game.h"
#include "player.h"
#include "bullet.h"
#include "judgement.h"
#include "enemy.h"
#include "explosion.h"
#include "number.h"
#include "camera2D.h"
#include "radar.h"
#include "map.h"
#include "fade.h"
#include "scene.h"
#include "effect.h"
#include "multimissile.h"
#include "item.h"
#include "bigbomb.h"

bool g_NextScene = false;

void Game_Init(void)
{
	BigBomb_Init();
	Item_Init();
	MultiMissile_Init();
	Map_Init();
	Player_Init();
	Enemy_Init();
	Bullet_Init();
	Judgement_Init();
	Explosion_Init();
	Radar_Init();
	Effect_Init();
	Fade_Start(false, 90, D3DCOLOR_RGBA(0, 0, 0, 255));
}

void Game_Uninit(void)
{
	Effect_Uninit();
	Bullet_Uninit();
}

void Game_Update(void)
{
	Camera2D_Update();
	Player_Update();
	Enemy_Update();
	Bullet_Update();
	BigBomb_Update();
	MultiMissile_Update();
	Judgement_Update();
	Explosion_Update();
	Radar_Update();
	Effect_Update();

	if (Judgement_GetEnable())
	{
		if (!g_NextScene)
		{
			Fade_Start(true, 90, D3DCOLOR_RGBA(0, 0, 0, 255));
			g_NextScene = true;
		}
		else
		{
			if (!Fade_IsFade())
			{
				Scene_Change(SCENE_INDEX_RESULT);
				Judgement_SetDisable();
				g_NextScene = false;
			}
		}
	}
}

void Game_Draw(void)
{
	Map_Draw();
	Enemy_Draw();
	Effect_Draw();    //描画するタイミングに気を付ける
	Player_Draw();
	MultiMissile_Draw();
	BigBomb_Draw();
	Bullet_Draw();
	Item_Draw();
	Explosion_Draw();
	Number_Draw(1056, 0);
	Radar_Draw();
}