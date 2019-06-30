#include "title.h"
#include "scene.h"
#include "input.h"
#include "sprite.h"
#include "Texture.h"
#include "fade.h"

static bool bNextScene = false;

void Title_Init(void)
{

}

void Title_Update(void)
{
	if (Keyboard_IsTrigger(DIK_SPACE))
	{
		Fade_Start(true, 90, D3DCOLOR_RGBA(0, 0, 0, 255));
		bNextScene = true;
	}

	if (bNextScene)
	{
		if (!Fade_IsFade())
		{
			Scene_Change(SCENE_INDEX_GAME);
			bNextScene = false;
		}
	}
}

void Title_Draw(void)
{
	Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	Sprite_Draw(TEXTURE_INDEX_TITLE, 0, 0);
}

void Title_Uninit(void)
{

}