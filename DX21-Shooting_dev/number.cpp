//**********************************************************
//�w�b�_�C���N���[�h
//**********************************************************
#include "number.h"
#include "sprite.h"
#include "Texture.h"
#include "DebugPrintf.h"

//**********************************************************
//�}�N����`
//**********************************************************
#define MAX_NUMBER          (7)
#define NUMBER_TW           (32.0f)
#define NUMBER_TH           (32.0f)

//**********************************************************
//�\���̐錾
//**********************************************************
typedef struct
{
	float tx;
	float ty;
}NUMBER;
static NUMBER g_Number[MAX_NUMBER];

//**********************************************************
//�X�R�A�X�V
//**********************************************************
void Number_Update(int n)
{
	int buf;
	int pow_ten = 10;
	for (int i = 0;i < MAX_NUMBER;i++)
	{
		buf = n % 10;
		n /= 10;

		g_Number[i].tx = NUMBER_TW * buf;
		g_Number[i].ty = 0;
	}
}

//**********************************************************
//�X�R�A�\��
//**********************************************************
void Number_Draw(float x, float y)
{
	////==============================================
	////
	////�X�R�A�v�Z(�v�@�C���I�I)
	////
	////==============================================
	for (int i = MAX_NUMBER - 1;i >= 0 ;i--)
	{
		Sprite_SetColor(D3DCOLOR_RGBA(0, 0, 0, 255));
		Sprite_Draw(TEXTURE_INDEX_NUMBER, x + (6 - i)*NUMBER_TW, y, g_Number[i].tx, g_Number[i].ty, NUMBER_TW, NUMBER_TH, false);
	}
}