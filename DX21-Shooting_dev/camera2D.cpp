#include "camera2D.h"
#include "enemy.h"
#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "map.h"
#include "multimissile.h"
#include "item.h"
#include "bigbomb.h"

//*******************************************************************
//�}�N����`
//*******************************************************************
#define CAMERA_CENTER_POSX (640)
#define CAMERA_CENTER_POSY (360)

//*******************************************************************
//�\���̐錾
//*******************************************************************


//*******************************************************************
//�O���[�o���ϐ�
//*******************************************************************
static PLAYER *pPlayer;
static ENEMY *pEnemy;
static BULLET_STATUS *pBullet;
static EXPLOSION *pExplosion;
static BULLET_MULTIMISSILE *pMultiMissile;
static MAP *pMap;
static Square *pObject;
//static Circle *pObject;
static ITEM *pItem;
static BIG_BOMB *pBomb;


//*******************************************************************
//�J����������
//*******************************************************************
void Camera2D_Init(void)
{

}

//*******************************************************************
//�J�����X�V
//*******************************************************************
void Camera2D_Update(void)
{
	float dx, dy, cx, cy, moveX, moveY;
	bool Limit_Top = false;
	bool Limit_Side = false;

	pPlayer = Player_GetPlayer();
	pMap = Map_GetMap();

	//�v���C���[�ƃJ�����̒��S���W�̍������߂�
	cx = pPlayer->PosX + (pPlayer->width / 2);
	cy = pPlayer->PosY + (pPlayer->height / 2);
	dx = cx - CAMERA_CENTER_POSX;
	dy = cy - CAMERA_CENTER_POSY;
	moveX = -(dx * 0.3);
	moveY = -(dy * 0.3);

	//�J��������ʊO�����Ȃ��悤�ɂ���
	//������
	if (pMap->PosX + moveX > 0 || pMap->PosX + pMap->width + moveX < 1280)
	{
		Limit_Side = true;
	}
	//�c����
	if (pMap->PosY + moveY > 0 || pMap->PosY + pMap->height + moveY < 720)
	{
		Limit_Top = true;
	}

	//GM21�ł�����Ώۂɋߎ����铮�����ŃJ�������ړ�(���ۂ̓I�u�W�F�N�g�������Ȃ�悤�Ɉړ�����)
	if (!Limit_Side)
	{
		pPlayer->PosX += moveX;
		pMap->PosX += moveX;
	}
	if (!Limit_Top)
	{
		pPlayer->PosY += moveY;
		pMap->PosY += moveY;
	}

	for (int i = 0;i < MAX_ENEMY;i++)
	{
		if (Enemy_GetEnable(i))
		{
			pEnemy = Enemy_GetEnemy(i);
			if (!Limit_Side)
			{
				pEnemy->PosX += moveX;
			}
			if (!Limit_Top)
			{
				pEnemy->PosY += moveY;
			}
		}
	}

	for (int i = 0;i < MAX_BULLET;i++)
	{
		if (Bullet_GetEnable(i))
		{
			pBullet = Bullet_GetBullet(i);
			if (!Limit_Side)
			{
				pBullet->posBulletX += moveX;
			}
			if (!Limit_Top)
			{
				pBullet->posBulletY += moveY;
			}
		}
	}

	for (int i = 0;i < MAX_EXPLOSION;i++)
	{
		if (Explosion_GetEnable(i))
		{
			pExplosion = Explosion_GetExplosion(i);
			if (!Limit_Side)
			{
				pExplosion->PosX += moveX;
			}
			if (!Limit_Top)
			{
				pExplosion->PosY += moveY;
			}
		}
	}

	for (int i = 0;i < MULTIMISSILE_MAX;i++)
	{
		if (MultiMissile_GetEnable(i))
		{
			pMultiMissile = MultiMissile_GetMultiMissile(i);
			if (!Limit_Side)
			{
				pMultiMissile->PosX += moveX;
			}
			if (!Limit_Top)
			{
				pMultiMissile->PosY += moveY;
			}
		}
	}

	for (int i = 0;i < MAX_ITEM;i++)
	{
		if (Item_GetEnable(i))
		{
			pItem = Item_GetItem(i);
			if (!Limit_Side)
			{
				pItem->PosX += moveX;
			}
			if (!Limit_Top)
			{
				pItem->PosY += moveY;
			}
		}
	}

	if (BigBomb_GetEnable())
	{
		pBomb = BigBomb_GetBigBimb();
		if (!Limit_Side)
		{
			pBomb->PosX += moveX;
		}
		if (!Limit_Top)
		{
			pBomb->PosY += moveY;
		}
	}

	for (int i = 0;i < MAX_OBJECT;i++)
	{
		pObject = Map_GetObjectCollision(i);
		if (!Limit_Side)
		{
			//pObject->cx += moveX;

			pObject->bottom_left.cx += moveX;
			pObject->bottom_right.cx += moveX;
			pObject->top_left.cx += moveX;
			pObject->top_right.cx += moveX;
		}
		if (!Limit_Top)
		{
			//pObject->cy += moveY;

			pObject->bottom_left.cy += moveY;
			pObject->bottom_right.cy += moveY;
			pObject->top_left.cy += moveY;
			pObject->top_right.cy += moveY;
		}
		
	}
}

//*******************************************************************
//�J�����`��
//*******************************************************************
void Camera2D_Draw(void)
{

}
