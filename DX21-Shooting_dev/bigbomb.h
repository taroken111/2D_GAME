#ifndef BIGBOMB_H_
#define BIGBOMB_H_

//******************************************
//�w�b�_�C���N���[�h
//******************************************
#include "collision.h"
#include <d3dx9.h>

//******************************************
//�\���̐錾
//******************************************
typedef struct
{
	float PosX;
	float PosY;
	float angle;
	bool Enable;
	bool Shot;
	D3DXVECTOR2 MoveVec;
}BIG_BOMB;

//******************************************
//�v���g�^�C�v�錾
//******************************************
void BigBomb_Init(void);
void BigBomb_Update(void);
void BigBomb_Draw(void);

void BigBomb_Create(void);
void BigBomb_Shot(void);
bool BigBomb_GetEnable(void);
void BigBomb_Disable(void);
BIG_BOMB* BigBomb_GetBigBimb(void);
Circle* BigBomb_GetCollision(void);


#endif // !BIGBOMB_H_

