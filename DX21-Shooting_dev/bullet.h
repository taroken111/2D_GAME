#ifndef BULLET_H_
#define BULLET_H_

#include "collision.h"
#include "Texture.h"
#include <d3dx9.h>

//*******************************************************************
//�}�N����`
//*******************************************************************
#define MAX_BULLET      (100)

//*******************************************************************
//�񋓌^
//*******************************************************************
enum BULLET_TYPE
{
	TYPE_PLAYER,
	TYPE_ENEMY,
};

//*******************************************************************
//�\���̐錾
//*******************************************************************
typedef struct
{
	float posBulletX;
	float posBulletY;
	float moveX;
	float moveY;
	float width;
	float height;
	float Angle;
	bool bBulletEnable;
	int flame_count;
	BULLET_TYPE type;
	TEX_INDEX texIndex;
}BULLET_STATUS;

//*******************************************************************
//�v���g�^�C�v�錾
//*******************************************************************
void Bullet_Init(void);
void Bullet_Uninit(void);
void Bullet_Update(void);
void Bullet_Draw(void);

void Bullet_Create(float x, float y, float angle, TEX_INDEX index, BULLET_TYPE type);   //�e�̎�ނ�����Ƃ��͈�����ǉ�

void Bullet_Destroy(int index);
bool Bullet_GetEnable(int index);
const Circle* Bullet_GetCircleCollision(int index);
BULLET_STATUS* Bullet_GetBullet(int index);
BULLET_TYPE Bullet_GetType(int index);

#endif