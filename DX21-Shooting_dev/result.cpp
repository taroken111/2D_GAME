#include "result.h"
#include "scene.h"
#include "sprite.h"
#include "Texture.h"
#include "input.h"
#include "fade.h"
#include "player.h"
#include "number.h"

static PLAYER *pPlayer;

void Result_Init(void)
{
	Fade_Start(false, 90, D3DCOLOR_RGBA(0, 0, 0, 255));
}

void Result_Update(void)
{
	if (Keyboard_IsTrigger(DIK_SPACE))
	{
		Scene_Change(SCENE_INDEX_TITLE);
	}
}

void Result_Draw(void)
{
	pPlayer = Player_GetPlayer();
	Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	if (pPlayer->JudgeWinOrLose)
	{
		Sprite_Draw(TEXTURE_INDEX_GAME_CLEAR, 0, 0);
		Number_Draw(560.0f, 400.0f);
	}
	else
	{
		Sprite_Draw(TEXTURE_INDEX_GAME_OVER, 0, 0);
	}
}

void Result_Uninit(void)
{

}