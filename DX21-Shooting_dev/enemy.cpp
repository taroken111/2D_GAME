#include "bullet.h"
#include "sprite.h"
#include "SpriteAnimation.h"
#include "Texture.h"
#include "common.h"
#include "enemy.h"
#include "DebugText.h"
#include "radar.h"
#include "player.h"
#include <stdlib.h>
#include <time.h>

//*******************************************************************
//�}�N����`
//*******************************************************************
#define PI (3.14)
#define MOVE_Y (4.0f)
#define MOVE_X (4.0f)

//*******************************************************************
//�O���[�o���ϐ�
//*******************************************************************
static ENEMY g_EnemyStatus[MAX_ENEMY];
static PLAYER *pPlayer;
static Circle *pEnemyCircleCollision;
static int g_EnemyChanceCounter;

//*******************************************************************
//������
//*******************************************************************
void Enemy_Init(void)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		g_EnemyStatus[i].flame_count = 0;
		g_EnemyStatus[i].PosX = 0.0f;
		g_EnemyStatus[i].PosY = 0.0f;
		g_EnemyStatus[i].height = 0.0f;
		g_EnemyStatus[i].width = 0.0f;
		g_EnemyStatus[i].Enable = false;

		g_EnemyStatus[i].fMoveY = 0.0f;
		g_EnemyStatus[i].fMoveX = 0.0f;
		g_EnemyStatus[i].Dec = 0.0f;
		g_EnemyStatus[i].Angle = 0.0f;

		g_EnemyStatus[i].EnemyType = TEXTURE_INDEX_ENEMY;

	}
	g_EnemyChanceCounter = 0;
	Enemy_Create(1000, 200, TEXTURE_INDEX_ENEMY);
	Enemy_Create(100, 1000, TEXTURE_INDEX_ENEMY);
	Enemy_Create(690, 790, TEXTURE_INDEX_ENEMY);
	Enemy_Create(2480, 720, TEXTURE_INDEX_ENEMY);
}

//*******************************************************************
//�I��
//*******************************************************************
void Enemy_Uninit(void)
{

}

//*******************************************************************
//�X�V
//*******************************************************************
void Enemy_Update(void)
{
	for (int i = 0;i < MAX_ENEMY;i++)
	{
		if (g_EnemyStatus[i].Enable)
		{
			g_EnemyStatus[i].flame_count++;

			//�ړ��ʂ̍X�V
			Enemy_Moving(i);
			g_EnemyStatus[i].Dec += 5;

			g_EnemyStatus[i].CircleCollision.cx = g_EnemyStatus[i].PosX + 64.0f;
			g_EnemyStatus[i].CircleCollision.cy = g_EnemyStatus[i].PosY + 64.0f;
			g_EnemyStatus[i].CircleCollision.radius = 32.0f * 1.5;

			//�e�̉�ʊO����
			float height = Texture_GetHeight(TEXTURE_INDEX_ENEMY);
			float width = Texture_GetWidth(TEXTURE_INDEX_ENEMY);

			if ((g_EnemyStatus[i].PosX < -1280))
			{
				//�Ƃ肠��������͉��ړ��������Ȃ��e�Ȃ̂ŉ�����̂�
				Enemy_Disable(i);
			}

			//========================================================
			//�G�̍U������
			//========================================================
			//�U�������邩�̔��f
			if (g_EnemyStatus[i].flame_count == 120)
			{
				//�v���C���[�̒��S�����߂�
				pPlayer = Player_GetPlayer();
				float BufX, BufY, cx, cy, angle;
				BufX = Texture_GetWidth(TEXTURE_INDEX_TANK_BODY) / 2;
				BufY = Texture_GetHeight(TEXTURE_INDEX_TANK_BODY) / 2;
				//cx = BufX * cos(pPlayer->Angle) - BufY * sin(pPlayer->Angle) + pPlayer->PosX - g_EnemyStatus[i].posEnemyX;
				//cy = BufX * sin(pPlayer->Angle) + BufY * cos(pPlayer->Angle) + pPlayer->PosY - g_EnemyStatus[i].posEnemyY;
				cx = pPlayer->PosX + BufX - g_EnemyStatus[i].PosX;
				cy = pPlayer->PosY + BufY - g_EnemyStatus[i].PosY;
				angle = atan2f(cy, cx);
				//�e�쐬
				Bullet_Create(g_EnemyStatus[i].PosX, g_EnemyStatus[i].PosY, angle, TEXTURE_INDEX_BULLET_FIRE, TYPE_ENEMY);

				g_EnemyStatus[i].flame_count = 0;
			}
		}
	}

	if (g_EnemyChanceCounter == 120)
	{
		float BufX;
		srand((unsigned)time(NULL));
		BufX = rand() % 720;
		Enemy_Create(1200, BufX, TEXTURE_INDEX_ENEMY);
		g_EnemyChanceCounter = 0;
	}
	g_EnemyChanceCounter++;
}

//*******************************************************************
//�`��
//*******************************************************************
void Enemy_Draw(void)
{
	for (int i = 0;i < MAX_ENEMY;i++)
	{
		if (g_EnemyStatus[i].Enable)
		{
			Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
			Sprite_Draw(TEXTURE_INDEX_ENEMY, g_EnemyStatus[i].PosX, g_EnemyStatus[i].PosY);
		}
	}

#ifdef _DEBUG
#define _DEBUG
	LPD3DXFONT pF = GetFont();
	int EnemyCount = 0;
	for (int i = 0;i < MAX_ENEMY;i++)
	{
		if (g_EnemyStatus[i].Enable)
		{
			EnemyCount++;
		}
	}
	DebugText(pF, "Enemy:%d", EnemyCount);
#endif
}

//*******************************************************************
//�e�@����
//*******************************************************************
void Enemy_Create(float x, float y, TEX_INDEX index)
{
	for (int i = 0;i < MAX_ENEMY;i++)
	{
		if (!g_EnemyStatus[i].Enable)
		{
			//�����e��������������
			//������X���W�AY���W�����ꂼ��ɓ����
			g_EnemyStatus[i].PosX = x;
			g_EnemyStatus[i].PosY = y;

			//�G��L���ɂ���
			g_EnemyStatus[i].Enable = true;

			//Y�����̈ړ��ʂ̐ݒ�
			g_EnemyStatus[i].fMoveY = MOVE_Y;
			g_EnemyStatus[i].fMoveX = -MOVE_X;

			//���ƍ����̎擾
			g_EnemyStatus[i].height = Texture_GetHeight(index);
			g_EnemyStatus[i].width = Texture_GetWidth(index);

			//�G�̃^�C�v
			g_EnemyStatus[i].EnemyType = index;

			break;
		}
	}
}

//*******************************************************************
//�e�@����
//*******************************************************************
void Enemy_Disable(int num)
{
	g_EnemyStatus[num].Enable = false;
	g_EnemyStatus[num].flame_count = 0;
	g_EnemyStatus[num].Angle = 0.0f;
	g_EnemyStatus[num].Dec = 0;
	Radar_Disable(num);
}

//*******************************************************************
//�G�l�~�[�̗L�������Q�b�^
//*******************************************************************
bool Enemy_GetEnable(int index)
{
	return g_EnemyStatus[index].Enable;
}

//*******************************************************************
//�G�l�~�[�̓����蔻��Q�b�^
//*******************************************************************
Circle* Enemy_GetCollision(int index)
{
	//pEnemyCircleCollision = &g_EnemyStatus[index].CircleCollision;
	//return pEnemyCircleCollision;
	return &g_EnemyStatus[index].CircleCollision;
}

//*******************************************************************
//�G�l�~�[�P�̂̍\���̃|�C���^�Q�b�^
//*******************************************************************
ENEMY* Enemy_GetEnemy(int index)
{
	return &g_EnemyStatus[index];
}

//*******************************************************************
//�G�l�~�[�̈ړ�����
//*******************************************************************
void Enemy_Moving(int index)
{
	g_EnemyStatus[index].Angle = (g_EnemyStatus[index].Dec / 180) * PI;
	g_EnemyStatus[index].fMoveY = MOVE_Y * sin(g_EnemyStatus[index].Angle);
	g_EnemyStatus[index].PosY += g_EnemyStatus[index].fMoveY;
	g_EnemyStatus[index].PosX += g_EnemyStatus[index].fMoveX;
}