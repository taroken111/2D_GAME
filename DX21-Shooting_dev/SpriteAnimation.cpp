#include "SpriteAnimation.h"
#include "Texture.h"
#include "sprite.h"
#include "common.h"

//************************************************************************************************************************************************************************
//マクロ定義
//************************************************************************************************************************************************************************
#define MAX_ANIMATION (30)

//************************************************************************************************************************************************************************
//グローバル変数
//************************************************************************************************************************************************************************
ANIMATION_DATA g_AnimationData[] = {
	{128.0f, 128.0f, 5 , false, DIRECT_NONE , 18, 5}, //EXPLOSION
};

int FILE_COUNT = sizeof(g_AnimationData) / sizeof(g_AnimationData[0]);

ANIMATION_TEXTURE_XY g_AnimationTXY[MAX_ANIMATION];

//************************************************************************************************************************************************************************
//アニメーション初期化
//************************************************************************************************************************************************************************
void SpriteAnimation_Init(void)
{
	for (int i = 0;i < MAX_ANIMATION;i++)
	{
		g_AnimationTXY[i].tx = 0.0f;
		g_AnimationTXY[i].ty = 0.0f;
		g_AnimationTXY[i].bEnable = false;
	}
}

//************************************************************************************************************************************************************************
//アニメーションの更新処理
//************************************************************************************************************************************************************************
void SpriteAnimation_Update(int flame_count, int animation_index, int animation_textureXY)
{
	//----------------------------------
	//切り取りカ所の設定
	//----------------------------------
	int PrintPattern = 0;
	PrintPattern = (flame_count / g_AnimationData[animation_index].speed) % g_AnimationData[animation_index].pattern_MAX;
	g_AnimationTXY[animation_textureXY].tx = g_AnimationData[animation_index].tw * (PrintPattern % g_AnimationData[animation_index].pattern_X);
	g_AnimationTXY[animation_textureXY].ty = g_AnimationData[animation_index].th * (PrintPattern / g_AnimationData[animation_index].pattern_X + 1);

	//----------------------------------
	//アニメーションの移動と移動範囲制限
	//----------------------------------
	//if (g_AnimationStatus[status_index].moveX != 0.0f)
	//{
	//	g_AnimationStatus[status_index].dx += g_AnimationStatus[status_index].moveX;
	//}
	//if (g_AnimationStatus[status_index].dx + g_AnimationData[animation_index].tw > SCREEN_WIDTH)
	//{
	//	g_AnimationStatus[status_index].dx = SCREEN_WIDTH - g_AnimationData[animation_index].tw;
	//	g_AnimationStatus[status_index].moveX *= -1.0f;
	//	g_AnimationData[animation_index].direct = DIRECT_LEFT;
	//}
	//if (g_AnimationStatus[status_index].dx < 0)
	//{
	//	g_AnimationStatus[status_index].dx = 0;
	//	g_AnimationStatus[status_index].moveX *= -1.0f;
	//	g_AnimationData[animation_index].direct = DIRECT_RIGHT;
	//}

	//----------------------------------
	//アニメーションの向き
	//----------------------------------
	//if (g_AnimationData[animation_index].direct == DIRECT_RIGHT)
	//{
	//	g_AnimationData[animation_index].flip = true;
	//}
	//else
	//{
	//	g_AnimationData[animation_index].flip = false;
	//}
}

//************************************************************************************************************************************************************************
//アニメーションの描画処理
//************************************************************************************************************************************************************************
void SpriteAnimation_Draw(int animation_index, int texture_index, int animation_textureXY, float dx, float dy)
{

	Sprite_Draw(texture_index, dx, dy, g_AnimationTXY[animation_textureXY].tx, g_AnimationTXY[animation_textureXY].ty, g_AnimationData[animation_index].tw, g_AnimationData[animation_index].th, g_AnimationData[animation_index].flip);
}

void SpriteAnimation_Draw(int animation_index, int texture_index, int animation_textureXY, float dx, float dy, float cx, float cy, float sx, float sy, float angle)
{
	Sprite_Draw(texture_index, dx, dy, g_AnimationTXY[animation_textureXY].tx, g_AnimationTXY[animation_textureXY].ty, g_AnimationData[animation_index].tw, g_AnimationData[animation_index].th, g_AnimationData[animation_index].flip, cx, cy, sx, sy, angle);
}

//************************************************************************************************************************************************************************
//アニメーションの作成
//************************************************************************************************************************************************************************
int SpriteAnimation_Create(void)
{
	for (int i = 0;i < MAX_ANIMATION;i++)
	{
		if (!g_AnimationTXY[i].bEnable)
		{
			g_AnimationTXY[i].tx = 0;
			g_AnimationTXY[i].ty = 0;
			g_AnimationTXY[i].bEnable = true;
			return i;
		}
	}
}

//************************************************************************************************************************************************************************
//アニメーションの無効化
//************************************************************************************************************************************************************************
void SpriteAnimation_Disable(int index)
{
	g_AnimationTXY[index].bEnable = false;
}

//************************************************************************************************************************************************************************
//アニメーションの１パターン当たりの横幅取得
//************************************************************************************************************************************************************************
float SpriteAnimation_GetTextureWidth(int index)
{
	return g_AnimationData[index].tw;
}

//************************************************************************************************************************************************************************
//アニメーションの１パターン当たりの高さ取得
//************************************************************************************************************************************************************************
float SpriteAnimation_GetTextureHeight(int index)
{
	return g_AnimationData[index].th;
}