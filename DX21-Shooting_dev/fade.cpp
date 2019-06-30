//*******************************************************************
//�w�b�_�C���N���[�h
//*******************************************************************
#include "fade.h"
#include "common.h"
#include "mydirect3d.h"

//*******************************************************************
//�\���̐錾
//*******************************************************************
typedef struct FadeVertex_tag
{
	D3DXVECTOR4 position;
	D3DCOLOR color;
}FadeVertex;

#define FVF_FADE_VERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

static bool g_bFade = false;                                  //�t�F�[�h���Ă��邩�ǂ���
static bool g_bOut = false;                                   //true�Ȃ�t�F�[�h�A�E�g
static float g_Alpha = 0.0f;
static float g_addAlpha = 0.0f;                                  //���l�̑�����(1 or 0)
static D3DCOLOR g_color = D3DCOLOR_RGBA(255, 255, 255, 255);
static int g_fadeFrame = 0;
static int g_fadeFrameCount = 0;

//*******************************************************************
//�t�F�[�h������
//*******************************************************************
void Fade_Init(void)
{
	g_bFade = false;
	g_Alpha = 0.0f;
}

//*******************************************************************
//�t�F�[�h�X�V
//*******************************************************************
void Fade_Update(void)
{
	if (g_bFade)
	{
		g_Alpha = min(max(g_Alpha + g_addAlpha, 0.0f), 1.0f);
		g_fadeFrameCount++;

		if (g_fadeFrameCount > g_fadeFrame)
		{
			g_fadeFrameCount = g_fadeFrame;
			g_bFade = false;
		}
	}
}

//*******************************************************************
//�t�F�[�h�`��
//*******************************************************************
void Fade_Draw(void)
{
	if (g_Alpha <= 0.0000001f)
	{
		//�t�F�[�h���Ă��Ȃ��Ƃ��̓t�F�[�h�p�̃|���S����`�悵�Ȃ�
		return;
	}
	D3DXCOLOR color(g_color);   //D3DXCOLOR�^�F���܂ł�D3DCOLOR���̒l��float�ň�����悤�ɂȂ�(�������A���𓾂Ă���g��)
	color.a = g_Alpha;
	FadeVertex v[] = {
		{ D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f), color },
		{ D3DXVECTOR4(SCREEN_WIDTH, 0.0f, 1.0f, 1.0f), color },
		{ D3DXVECTOR4(0.0f, SCREEN_HEIGHT, 1.0f, 1.0f), color },
		{ D3DXVECTOR4(SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f, 1.0f), color },
	};

	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	pDevice->SetFVF(FVF_FADE_VERTEX);
	pDevice->SetTexture(0, NULL);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(FadeVertex));
}

//*******************************************************************
//�t�F�[�h�J�n
//*******************************************************************
void Fade_Start(bool bOut, int frame, D3DCOLOR color)
{
	g_bFade = true;
	g_bOut = bOut;
	g_color = color;
	g_fadeFrame = frame;
	g_fadeFrameCount = 0;

	g_addAlpha = 1.0f / g_fadeFrame;

	if (bOut)
	{
		g_Alpha = 0.0f;
	}
	else
	{
		g_Alpha = 1.0f;
		g_addAlpha = -g_addAlpha;
	}
}

//*******************************************************************
//�t�F�[�h�̗L�������`�F�b�N
//*******************************************************************
bool Fade_IsFade(void)
{
	return g_bFade;
}