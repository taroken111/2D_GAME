#ifndef SPRITEANIMATION_H_
#define SPRITEANIMATION_H_
#include "Texture.h"

//************************************************************************************************************************************************************************
//マクロ定義
//************************************************************************************************************************************************************************
#define DIRECT_NONE (0)
#define DIRECT_LEFT (1)
#define DIRECT_RIGHT (2)

//************************************************************************************************************************************************************************
//列挙型
//************************************************************************************************************************************************************************
enum
{
	ANIMATION_INDEX_EXPLOSION,
};

//************************************************************************************************************************************************************************
//構造体宣言
//************************************************************************************************************************************************************************
typedef struct
{
	float tw;
	float th;
	int speed;
	bool flip;
	unsigned short direct;
	unsigned short pattern_MAX;
	unsigned short pattern_X;
}ANIMATION_DATA;

typedef struct
{
	float tx;
	float ty;
	bool bEnable;
}ANIMATION_TEXTURE_XY;

//************************************************************************************************************************************************************************
//プロトタイプ宣言
//************************************************************************************************************************************************************************
void SpriteAnimation_Init(void);
void SpriteAnimation_Update(int flame_count, int animation_index, int animation_textureXY);
void SpriteAnimation_Draw(int animation_index, int texture_index, int animation_textureXY, float dx, float dy);
void SpriteAnimation_Draw(int animation_index, int texture_index, int animation_textureXY, float dx, float dy, float cx, float cy, float sx, float sy, float angle);
int SpriteAnimation_Create(void);
void SpriteAnimation_Disable(int index);
float SpriteAnimation_GetTextureWidth(int index);
float SpriteAnimation_GetTextureHeight(int index);

#endif
