#ifndef ITEM_H_
#define ITEM_H_

//**********************************************************
//ヘッダインクルード
//**********************************************************
#include "collision.h"

//**********************************************************
//マクロ定義
//**********************************************************
#define MAX_ITEM (10)

//**********************************************************
//列挙型
//**********************************************************
typedef enum
{
	ITEM_TYPE_MULTIMISSILE,
	ITEM_TYPE_SPEED,
	ITEM_TYPE_COOL,
	ITEM_TYPE_BOMB,
}ITEM_TYPE;

//**********************************************************
//構造体宣言
//**********************************************************
typedef struct
{
	float PosX;
	float PosY;
	bool Enable;
	ITEM_TYPE Type;
}ITEM;

//**********************************************************
//プロトタイプ宣言
//**********************************************************
void Item_Init(void);
void Item_Update(void);                                  //Judgement_Updateで呼び出す
void Item_Draw(void);
//void Item_Uninit(void);

void Item_Create(float x, float y);                      //Judgement_Updateの敵が消滅する際に呼ぶ
void Item_Disable(int index);
Circle* Item_GetCollision(int index);
bool Item_GetEnable(int index);
ITEM* Item_GetItem(int index);

#endif // !ITEM_H_

