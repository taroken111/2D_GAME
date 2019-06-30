#include "scene.h"
#include "game.h"
#include "title.h"
#include "result.h"

static SCENE_INDEX g_SceneIndex = SCENE_INDEX_TITLE;
static SCENE_INDEX g_SceneNextIndex = g_SceneIndex;

typedef void (*Func) (void);   //関数ポイインタの型

//上から順にENUMで宣言した順番に作る
static const Func SCENE_FUNCTION[][4] =
{
	{Title_Init, Title_Update, Title_Draw, Title_Uninit},
	{Game_Init, Game_Update, Game_Draw, Game_Uninit},
	{Result_Init, Result_Update, Result_Draw, Result_Uninit},
};

void Scene_Init(SCENE_INDEX index)
{
	g_SceneIndex = g_SceneNextIndex = index;

	SCENE_FUNCTION[g_SceneIndex][FUNCTION_INDEX_INIT]();
}

void Scene_Update(void)
{
	SCENE_FUNCTION[g_SceneIndex][FUNCTION_INDEX_UPDATE]();
}

void Scene_Draw(void)
{
	SCENE_FUNCTION[g_SceneIndex][FUNCTION_INDEX_DRAW]();
}

void Scene_Check(void)   //毎フレーム終わりに呼ぶ関数
{
	if (g_SceneIndex != g_SceneNextIndex)
	{
		Scene_Uninit();
		Scene_Init(g_SceneNextIndex);
	}
}

void Scene_Uninit(void)
{
	SCENE_FUNCTION[g_SceneIndex][FUNCTION_INDEX_UNINIT]();
}

void Scene_Change(SCENE_INDEX index)
{
	g_SceneNextIndex = index;
}