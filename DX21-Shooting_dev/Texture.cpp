//***************************************************************************************************
//作成日 6月4日
//テーマ:「関数分け(Texture.cpp)」
//作成者:藤井 賢太郎
//***************************************************************************************************
#include "Texture.h"
#include "mydirect3d.h"
#include "DebugPrintf.h"

//***********************************
//構造体宣言
//***********************************
typedef struct TextureFile_tag                                                                     //TextureFile_tagはタグ名。なくてもよい
{
	const char filename[TEXTURE_FILENAME_MAX];                                                     //一度登録したら変えることがない変数にconstをつける
	int width;
	int height;
}TextureFile;                                                                                      //テクスチャのデータベース1レコードあたりの構造体

//***********************************
//グローバル宣言
//***********************************
static TextureFile g_TextureFiles[] = {
	{ "asset/texture/background.jpg", 2560, 1440},
	{ "asset/texture/enemy.png", 128, 128},
	{ "asset/texture/explosion.png", 640, 512},
	{ "asset/texture/number.tga", 320, 32},
	{ "asset/texture/TITLE.png", 1280, 720},
	{ "asset/texture/RESULT.jpg", 1280, 720},
	{ "asset/texture/radar.png", 256, 256},
	{ "asset/texture/enemyradar.png", 20, 20},
	{ "asset/texture/Tank_Body.png", 256, 160},
	{ "asset/texture/Tank_Head.png", 256, 160},
	{ "asset/texture/Bullet_Normal.png", 16, 16},
	{ "asset/texture/effect000.jpg", 80, 80},
	{ "asset/texture/Map.png", 2560, 1440},
	{ "asset/texture/Body_MultiMissile.png", 128, 160},
	{ "asset/texture/Bullet_MultiMissile.png", 16, 16},
	{ "asset/texture/Fire_MultiMissile.png", 16, 16},
	{ "asset/texture/Reticle_MultiMissile.png", 64, 64},
	{ "asset/texture/item_multimissile.png", 32, 32},
	{ "asset/texture/HP_player.png", 1000, 32},
	{ "asset/texture/HP_gauge.png", 1000, 32},
	{ "asset/texture/HG_1.png", 500, 16},
	{ "asset/texture/HG_2.png", 500, 16},
	{ "asset/texture/HG_3.png", 500, 16},
	{ "asset/texture/fireball.png", 32, 32},
	{ "asset/texture/item_normal.png", 64, 64},
	{ "asset/texture/item_speedup.png", 32, 32},
	{ "asset/texture/item_cooldown.png", 32, 32},
	{ "asset/texture/HP_item.jpg", 256, 256},
	{ "asset/texture/GAME_OVER.png", 1280, 720},
	{ "asset/texture/GAME_CLEAR.png", 1280, 720},
	{ "asset/texture/Bomb_Body.png",170, 106},
	{ "asset/texture/item_bigbomb.png", 32, 32},
	{ "asset/texture/HP_name.png", 128, 32},
	{ "asset/texture/SB_name.png", 128, 32},
	{ "asset/texture/life.png", 32, 32},
};
static const int TEXTURE_FILE_COUNT = sizeof(g_TextureFiles) / sizeof(g_TextureFiles[0]);          //構造体配列の全体を構造体一個あたりのデータで割ると何個テクスチャが読み込まれたか算出できる。
                                                                                                   //ただし、配列のみ。ポインタで管理している場合はできない
static LPDIRECT3DTEXTURE9 g_pTexture[TEXTURE_FILE_COUNT];

//***********************************
//テクスチャ読み込み
//***********************************
int Texture_Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice)
	{
		DebugPrintf("\n\n\nDevice Return NULL!!\n\n\n");
		return TEXTURE_FILE_COUNT;
	}

	HRESULT hr;
	int failed_count = 0;

	for (int i = 0;i < TEXTURE_FILE_COUNT;i++)
	{
		hr = D3DXCreateTextureFromFile(pDevice, g_TextureFiles[i].filename, &g_pTexture[i]);
		if (FAILED(hr))
		{
			DebugPrintf("\n\n\nTexture[%d] Failed!!\n\n\n", i);
			failed_count++;
		}
	}

	return failed_count;
}

//***********************************
//テクスチャRelease
//***********************************
void Texture_Release(void)
{
	for (int i = 0;i < TEXTURE_FILE_COUNT;i++)
	{
		if (g_pTexture[i] != NULL)
		{
			g_pTexture[i]->Release();
			g_pTexture[i] = NULL;
		}
	}
}

//***********************************
//指定したテクスチャをデバイスに送る
//***********************************
LPDIRECT3DTEXTURE9 Texture_GetTexture(int index)
{
	return g_pTexture[index];
}

//***********************************
//テクスチャ幅取得
//***********************************
int Texture_GetWidth(int index)
{
	return g_TextureFiles[index].width;
}

//***********************************
//テクスチャ高さ取得
//***********************************
int Texture_GetHeight(int index)
{
	return g_TextureFiles[index].height;
}
