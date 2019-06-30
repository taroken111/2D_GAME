#ifndef ENEMY_H_
#define ENEMY_H_

#include "collision.h"
#include "Texture.h"

//*******************************************************************
//�}�N����`
//*******************************************************************
#define MAX_ENEMY      (10)

//*******************************************************************
//�\���̐錾
//*******************************************************************
typedef struct
{
	float PosX;
	float PosY;
	float width;
	float height;
	bool Enable;
	int flame_count;

	float fMoveY;
	float fMoveX;
	float Dec;
	float Angle;

	Circle CircleCollision;
	TEX_INDEX EnemyType;
}ENEMY;

//*******************************************************************
//�v���g�^�C�v�錾
//*******************************************************************
void Enemy_Init(void);
void Enemy_Uninit(void);
void Enemy_Update(void);
void Enemy_Draw(void);

void Enemy_Create(float x, float y, TEX_INDEX index);   //�e�̎�ނ�����Ƃ��͈�����ǉ�
void Enemy_Disable(int num);
bool Enemy_GetEnable(int index);
Circle* Enemy_GetCollision(int index);
ENEMY* Enemy_GetEnemy(int index);

void Enemy_Moving(int index);

#endif // !ENEMY_H_

