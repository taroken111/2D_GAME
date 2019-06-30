//******************************************
//�w�b�_�C���N���[�h
//******************************************
#include "bigbomb.h"
#include "player.h"
#include "sprite.h"
#include "Texture.h"

//******************************************
//�}�N����`
//******************************************
#define BOMB_MOVEX (7.0f)
#define BOMB_MOVEY (0.0f)

//******************************************
//�O���[�o���ϐ�
//******************************************
static BIG_BOMB g_BigBimb;
static PLAYER *pPlayer;
static Circle BigBomb;
static Circle *pBigBomb;

//******************************************
//������
//******************************************
void BigBomb_Init(void)
{
	g_BigBimb.Enable = false;
	g_BigBimb.Shot = false;
	g_BigBimb.PosX = 0.0f;
	g_BigBimb.PosY = 0.0f;
	g_BigBimb.MoveVec = D3DXVECTOR2(0.0f, 0.0f);
	g_BigBimb.angle = 0.0f;

	pPlayer = Player_GetPlayer();
	pBigBomb = &BigBomb;
}

//******************************************
//�X�V
//******************************************
void BigBomb_Update(void)
{
	if (g_BigBimb.Enable)
	{
		if (g_BigBimb.Shot)
		{
			//���˂���Ă���ꍇ
			g_BigBimb.PosX += g_BigBimb.MoveVec.x;
			g_BigBimb.PosY += g_BigBimb.MoveVec.y;
		}
		else
		{
			//���˂���Ă��Ȃ��Ƃ�
			g_BigBimb.PosX = pPlayer->PosX;
			g_BigBimb.PosY = pPlayer->PosY;
			g_BigBimb.angle = pPlayer->HeadAngle;
		}
	}
}

//******************************************
//�`��
//******************************************
void BigBomb_Draw(void)
{
	if (g_BigBimb.Enable)
	{
		//���˂���Ă���ꍇ�Ƃ���Ă��Ȃ��ꍇ�ɕ�����
		if (g_BigBimb.Shot)
		{
			//���˂���Ă���ꍇ
			Sprite_Draw(TEXTURE_INDEX_BODY_BOMB, g_BigBimb.PosX, g_BigBimb.PosY, 0, 0, Texture_GetWidth(TEXTURE_INDEX_BODY_BOMB), Texture_GetHeight(TEXTURE_INDEX_BODY_BOMB), false, g_BigBimb.PosX + (Texture_GetWidth(TEXTURE_INDEX_BODY_BOMB) / 2), g_BigBimb.PosY + (Texture_GetHeight(TEXTURE_INDEX_BODY_BOMB) / 2), 1.0f, 1.0f, g_BigBimb.angle);
		}
		else
		{
			//���˂���Ă��Ȃ��ꍇ
			Sprite_Draw(TEXTURE_INDEX_BODY_BOMB, g_BigBimb.PosX, g_BigBimb.PosY, 0, 0, Texture_GetWidth(TEXTURE_INDEX_BODY_BOMB), Texture_GetHeight(TEXTURE_INDEX_BODY_BOMB), false, pPlayer->PosX + TANK_HEAD_CENTER_WIDTH, pPlayer->PosY + TANK_HEAD_CENTER_HEIGHT, 1.0f, 1.0f, g_BigBimb.angle);
		}
	}
}

//******************************************
//�쐬(�L����)
//******************************************
void BigBomb_Create(void)
{
	g_BigBimb.Enable = true;
	g_BigBimb.PosX = pPlayer->PosX;
	g_BigBimb.PosY = pPlayer->PosY;
	g_BigBimb.MoveVec.x = BOMB_MOVEX * cos(pPlayer->HeadAngle) - BOMB_MOVEY * sin(pPlayer->HeadAngle);
	g_BigBimb.MoveVec.y = BOMB_MOVEX * sin(pPlayer->HeadAngle) + BOMB_MOVEY * cos(pPlayer->HeadAngle);
	g_BigBimb.angle = pPlayer->HeadAngle;
}

//******************************************
//���������̃t���O�����Ă�
//******************************************
void BigBomb_Shot(void)
{
	g_BigBimb.MoveVec.x = BOMB_MOVEX * cos(pPlayer->HeadAngle) - BOMB_MOVEY * sin(pPlayer->HeadAngle);
	g_BigBimb.MoveVec.y = BOMB_MOVEX * sin(pPlayer->HeadAngle) + BOMB_MOVEY * cos(pPlayer->HeadAngle);
	g_BigBimb.Shot = true;
}

//******************************************
//�L�����擾
//******************************************
bool BigBomb_GetEnable(void)
{
	return g_BigBimb.Enable;
}

//******************************************
//������
//******************************************
void BigBomb_Disable(void)
{
	g_BigBimb.Enable = false;
	g_BigBimb.Shot = false;
}

//******************************************
//���e�̃Q�b�^
//******************************************
BIG_BOMB* BigBomb_GetBigBimb(void)
{
	return &g_BigBimb;
}

//******************************************
//���e�̓����蔻��
//******************************************
Circle* BigBomb_GetCollision(void)
{
	pBigBomb->cx = g_BigBimb.PosX + (Texture_GetWidth(TEXTURE_INDEX_BODY_BOMB) / 2);
	pBigBomb->cy = g_BigBimb.PosY + (Texture_GetHeight(TEXTURE_INDEX_BODY_BOMB) / 2);
	pBigBomb->radius = (Texture_GetWidth(TEXTURE_INDEX_BODY_BOMB) + Texture_GetHeight(TEXTURE_INDEX_BODY_BOMB)) / 2;
	return pBigBomb;
}