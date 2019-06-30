#ifndef ITEM_H_
#define ITEM_H_

//**********************************************************
//�w�b�_�C���N���[�h
//**********************************************************
#include "collision.h"

//**********************************************************
//�}�N����`
//**********************************************************
#define MAX_ITEM (10)

//**********************************************************
//�񋓌^
//**********************************************************
typedef enum
{
	ITEM_TYPE_MULTIMISSILE,
	ITEM_TYPE_SPEED,
	ITEM_TYPE_COOL,
	ITEM_TYPE_BOMB,
}ITEM_TYPE;

//**********************************************************
//�\���̐錾
//**********************************************************
typedef struct
{
	float PosX;
	float PosY;
	bool Enable;
	ITEM_TYPE Type;
}ITEM;

//**********************************************************
//�v���g�^�C�v�錾
//**********************************************************
void Item_Init(void);
void Item_Update(void);                                  //Judgement_Update�ŌĂяo��
void Item_Draw(void);
//void Item_Uninit(void);

void Item_Create(float x, float y);                      //Judgement_Update�̓G�����ł���ۂɌĂ�
void Item_Disable(int index);
Circle* Item_GetCollision(int index);
bool Item_GetEnable(int index);
ITEM* Item_GetItem(int index);

#endif // !ITEM_H_

