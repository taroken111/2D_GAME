#include "map.h"
#include "Texture.h"
#include "sprite.h"
#include "collision.h"

//*******************************************************************
//マクロ定義
//*******************************************************************
#define MAP_WIDTH    (2560)
#define MAP_HEIGHT   (1440)

//*******************************************************************
//グローバル変数
//*******************************************************************
static MAP g_Map;
static Square Object[MAX_OBJECT];
static const Square g_Object[MAX_OBJECT] = {
	//{ 280.0f, 600.0f, 40.0f },
	//{ 280.0f, 680.0f, 40.0f },
	//{ 280.0f, 760.0f, 40.0f },
	//{ 280.0f, 840.0f, 40.0f },

	//{ 1200.0f, 280.0f, 40.0f },
	//{ 1280.0f, 280.0f, 40.0f },
	//{ 1360.0f, 280.0f, 40.0f },
	//{ 1440.0f, 280.0f, 40.0f },

	//{ 2280.0f, 600.0f, 40.0f },
	//{ 2280.0f, 680.0f, 40.0f },
	//{ 2280.0f, 760.0f, 40.0f },
	//{ 2280.0f, 840.0f, 40.0f },

	//{ 1200.0f, 1180.0f, 40.0f },
	//{ 1280.0f, 1180.0f, 40.0f },
	//{ 1360.0f, 1180.0f, 40.0f },
	//{ 1440.0f, 1180.0f, 40.0f },

	//{ 1280.0f, 680.0f, 40.0f },
	//{ 1280.0f, 760.0f, 40.0f },
	//{ 1360.0f, 680.0f, 40.0f },
	//{ 1360.0f, 760.0f, 40.0f }

	{{240.0f, 560.0f}, {336.0f, 560.0f}, {240.0f, 880.0f}, {336.0f, 880.0f}},
	{{1120.0f, 240.0f}, {1440.0f, 240.0f}, {1120.0f, 336.0f}, {1440.0f, 336.0f}},
	{{2224.0f, 560.0f}, {2320.0f, 560.0f}, {2224.0f, 880.0f}, {2320.0f, 880.0f}},
	{{1120.0f, 1104.0f}, {1440.0f, 1104.0f}, {1120.0f, 1200.0f}, {1440.0f, 1200.0f}},
	{{1200.0f, 640.0f}, {1360.0f, 640.0f}, {1200.0f, 800.0f}, {1360.0f, 800.0f}}
};

//*******************************************************************
//マップの初期化
//*******************************************************************
void Map_Init(void)
{
	g_Map.PosX = 0.0f;
	g_Map.PosY = 0.0f;
	g_Map.width = MAP_WIDTH;
	g_Map.height = MAP_HEIGHT;

	for (int i = 0;i < MAX_OBJECT;i++)
	{
		Object[i].top_left.cx = g_Object[i].top_left.cx;
		Object[i].top_left.cy = g_Object[i].top_left.cy;

		Object[i].top_right.cx = g_Object[i].top_right.cx;
		Object[i].top_right.cy = g_Object[i].top_right.cy;

		Object[i].bottom_left.cx = g_Object[i].bottom_left.cx;
		Object[i].bottom_left.cy = g_Object[i].bottom_left.cy;

		Object[i].bottom_right.cx = g_Object[i].bottom_right.cx;
		Object[i].bottom_right.cy = g_Object[i].bottom_right.cy;

		//Object[i].cx = g_Object[i].cx;
		//Object[i].cy = g_Object[i].cy;
		//Object[i].radius = g_Object[i].radius;
	}

}

//*******************************************************************
//マップの描画
//*******************************************************************
void Map_Draw(void)
{
	Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	Sprite_Draw(TEXTURE_INDEX_MAP, g_Map.PosX, g_Map.PosY);
}
//*******************************************************************
//マップの他ファイル用ゲッタ
//*******************************************************************
MAP* Map_GetMap(void)
{
	return &g_Map;
}

//*******************************************************************
//マップの障害物用当たり判定ゲッタ
//*******************************************************************
Square* Map_GetObjectCollision(int index)
{
	return &Object[index];
}