//***************************************************************************************************
//作成日 6月4日
//テーマ:「関数分け(Texture.h)」
//作成者:藤井 賢太郎
//***************************************************************************************************
#ifndef TEXTURE_H_
#define TEXTURE_H_
#include <d3dx9.h>

#define TEXTURE_FILENAME_MAX (64)                                                  //テクスチャのファイル名の最大文字数

enum TEX_INDEX
{
	TEXTURE_INDEX_BACK,
	TEXTURE_INDEX_ENEMY,
	TEXTURE_INDEX_EXPLOSION,
	TEXTURE_INDEX_NUMBER,
	TEXTURE_INDEX_TITLE,
	TEXTURE_INDEX_RESULT,
	TEXTURE_INDEX_RADAR,
	TEXTURE_INDEX_ENEMYRADAR,
	TEXTURE_INDEX_TANK_BODY,
	TEXTURE_INDEX_TANK_HEAD,
	TEXTURE_INDEX_BULLET_NORMAL,
	TEXTURE_INDEX_EFFECT,
	TEXTURE_INDEX_MAP,
	TEXTURE_INDEX_BODY_MULTIMISSILE,
	TEXTURE_INDEX_BULLET_MULTIMISSILE,
	TEXTURE_INDEX_FIRE_MULTIMISSILE,
	TEXTURE_INDEX_RETICLE_MULTIMISSILE,
	TEXTURE_INDEX_ITEM_MULTIMISSILE,
	TEXTURE_INDEX_HP_PLAYER,
	TEXTURE_INDEX_HP_GAUGE,
	TEXTURE_INDEX_HG_1,
	TEXTURE_INDEX_HG_2,
	TEXTURE_INDEX_HG_3,
	TEXTURE_INDEX_BULLET_FIRE,
	TEXTURE_INDEX_ITEM_NORMAL,
	TEXTURE_INDEX_ITEM_SPEED,
	TEXTURE_INDEX_ITEM_COOL,
	TEXTURE_INDEX_HP_ITEM,
	TEXTURE_INDEX_GAME_OVER,
	TEXTURE_INDEX_GAME_CLEAR,
	TEXTURE_INDEX_BODY_BOMB,
	TEXTURE_INDEX_ITEM_BOMB,
	TEXTURE_INDEX_HP_NAME,
	TEXTURE_INDEX_SB_NAME,
	TEXTURE_INDEX_LIFE,
};
//******************
//プロトタイプ宣言
//******************
int Texture_Load(void);                                                            //戻り値：読み込めなかったテクスチャの数
void Texture_Release(void);
LPDIRECT3DTEXTURE9 Texture_GetTexture(int index);
int Texture_GetWidth(int index);
int Texture_GetHeight(int index);
#endif
