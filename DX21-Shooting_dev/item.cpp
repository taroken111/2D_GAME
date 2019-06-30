//**********************************************************
//�w�b�_�C���N���[�h
//**********************************************************
#include "item.h"
#include "player.h"
#include "collision.h"
#include "sprite.h"
#include "Texture.h"
#include <stdlib.h>
#include <time.h>
#include "bigbomb.h"

//**********************************************************
//�O���[�o���ϐ�
//**********************************************************
static ITEM g_Item[MAX_ITEM];
static Circle ItemCollision;
static Circle *pItemCollision;
static PLAYER *pPlayer;
static Circle *pPlayerCollision;


//**********************************************************
//������
//**********************************************************
void Item_Init(void)
{
	for (int i = 0;i < MAX_ITEM;i++)
	{
		g_Item[i].Enable = false;
		g_Item[i].PosX = 0.0f;
		g_Item[i].PosY = 0.0f;
		g_Item[i].Type = ITEM_TYPE_MULTIMISSILE;
	}

	pItemCollision = &ItemCollision;
	pPlayer = Player_GetPlayer();
}

//**********************************************************
//�X�V
//**********************************************************
void Item_Update(void)
{
	pPlayerCollision = Player_GetCollision();
	for (int i = 0;i < MAX_ITEM;i++)
	{
		if (g_Item[i].Enable)
		{
			pItemCollision = Item_GetCollision(i);
			if (Collision_IsHitCircle(pPlayerCollision, pItemCollision))
			{
				Item_Disable(i);

				//�}���`�~�T�C���̃A�C�e�����E�����ꍇ
				if (g_Item[i].Type == ITEM_TYPE_MULTIMISSILE)
				{
					pPlayer->BulletType_Normal = false;
					pPlayer->BulletType_Bomb = false;
					pPlayer->BulletType_MultiMissile = true;
					pPlayer->PlayerScore += 50 * pPlayer->SpeedBooster;
				}

				//���e�̃A�C�e�����E�����ꍇ
				//if (g_Item[i].Type == ITEM_TYPE_BOMB)
				//{
				//	pPlayer->BulletType_Normal = false;
				//	pPlayer->BulletType_MultiMissile = false;
				//	pPlayer->BulletType_Bomb = true;
				//	BigBomb_Create();

				//}

				//�X�s�[�h�A�b�v�̃A�C�e�����E�����ꍇ
				if (g_Item[i].Type == ITEM_TYPE_SPEED)
				{
					if (pPlayer->HowManySpeedUp < 5)
					{
						pPlayer->SpeedBooster += 0.5f;
						pPlayer->HowManySpeedUp++;
						pPlayer->PlayerScore += 50 * pPlayer->SpeedBooster;
					}
				}

				//�N�[���_�E���̃A�C�e�����E�����ꍇ
				if (g_Item[i].Type == ITEM_TYPE_COOL)
				{
					if (pPlayer->HowManyCoolDown < 5)
					{
						pPlayer->CoolTime -= 36.0f;
						pPlayer->HowManyCoolDown++;
						pPlayer->PlayerScore += 50 * pPlayer->SpeedBooster;
					}
				}
			}
		}
	}
}

//**********************************************************
//�`��
//**********************************************************
void Item_Draw(void)
{
	for (int i = 0;i < MAX_ITEM;i++)
	{
		if (g_Item[i].Enable)
		{
			if (g_Item[i].Type == ITEM_TYPE_MULTIMISSILE)
			{
				Sprite_Draw(TEXTURE_INDEX_ITEM_MULTIMISSILE, g_Item[i].PosX, g_Item[i].PosY);
			}

			if (g_Item[i].Type == ITEM_TYPE_SPEED)
			{
				Sprite_Draw(TEXTURE_INDEX_ITEM_SPEED, g_Item[i].PosX, g_Item[i].PosY);
			}

			if (g_Item[i].Type == ITEM_TYPE_COOL)
			{
				Sprite_Draw(TEXTURE_INDEX_ITEM_COOL, g_Item[i].PosX, g_Item[i].PosY);
			}

			if (g_Item[i].Type == ITEM_TYPE_BOMB)
			{
				Sprite_Draw(TEXTURE_INDEX_ITEM_BOMB, g_Item[i].PosX, g_Item[i].PosY);
			}
		}
	}
}

//**********************************************************
//�쐬
//**********************************************************
void Item_Create(float x, float y)
{
	for (int i = 0;i < MAX_ITEM;i++)
	{
		if (!g_Item[i].Enable)
		{
			//���W���͂ƗL��������
			g_Item[i].PosX = x;
			g_Item[i].PosY = y;
			g_Item[i].Enable = true;

			//�A�C�e���K�`���p��������
			srand((unsigned)time(NULL));
			int buf = rand() % 100;

			//========================
			//�������������̏ꍇ����
			//========================

			//20%�̊m���Ń}���`�~�T�C��
			if (buf >= 0 && buf < 20)
			{
				g_Item[i].Type = ITEM_TYPE_MULTIMISSILE;
			}

			//10%�̊m���Ŕ��e
			//if (buf >= 20 && buf < 30)
			//{
			//	g_Item[i].Type = ITEM_TYPE_BOMB;
			//}

			//30%�̊m���ŃX�s�[�h�A�b�v
			if (buf >= 20 && buf < 50)
			{
				g_Item[i].Type = ITEM_TYPE_SPEED;
			}

			//40%�̊m���ŃN�[���_�E��
			if (buf >= 50 && buf < 100)
			{
				g_Item[i].Type = ITEM_TYPE_COOL;
			}

			break;
		}
	}
}

//**********************************************************
//������
//**********************************************************
void Item_Disable(int index)
{
	g_Item[index].Enable = false;
}

//**********************************************************
//�����蔻��擾
//**********************************************************
Circle* Item_GetCollision(int index)
{
	pItemCollision->cx = g_Item[index].PosX + 16.0f;
	pItemCollision->cy = g_Item[index].PosY + 16.0f;
	pItemCollision->radius = 16.0f;
	return pItemCollision;
}

//**********************************************************
//�L�������擾
//**********************************************************
bool Item_GetEnable(int index)
{
	return g_Item[index].Enable;
}

//**********************************************************
//�Q�b�^
//**********************************************************
ITEM* Item_GetItem(int index)
{
	return &g_Item[index];
}