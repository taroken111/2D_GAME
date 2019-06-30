#include <d3dx9.h>
#include "effect.h"
#include "mydirect3d.h"
#include "Texture.h"

//*******************************************************************
//�}�N����`
//*******************************************************************
#define EFFECT_COUNT_MAX     (2048)     //�G�t�F�N�g�̍ő吔

//*******************************************************************
//�\���̐錾
//*******************************************************************
typedef struct EffectVertex2D_tag
{
	D3DXVECTOR4 position;                                                                            //�|���S���̍��W
	D3DCOLOR color;                                                                                  //�|���S���̐F�ݒ�
	D3DXVECTOR2 texcoord;                                                                            //�e�N�X�`�����W�ݒ�
}EffectVertex;                                                                                     //�|���S���̏��
#define FVF_EFFECT (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)                                  //RHW��1���ĂȂ��Ɠ񎟌������Ȃ�

typedef struct EFFECT_tag
{
	float x, y;
	D3DCOLOR color;
	float alpha;
	float radius;
	int life;
	int birth;
}EFFECT;

//*******************************************************************
//�O���[�o���ϐ�
//*******************************************************************
static EFFECT g_Effect[EFFECT_COUNT_MAX];
static int g_EffectFrameCount = 0;
static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;   //�K�v�Ȓ��_��
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;     //���_�̑g�ݍ��킹��

void Effect_Init(void)
{
	for (int i = 0;i < EFFECT_COUNT_MAX;i++)
	{
		g_Effect[i].life = -1;

	}

	g_EffectFrameCount = 0;

	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	pDevice->CreateVertexBuffer(
		sizeof(EffectVertex) * 4 * EFFECT_COUNT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_EFFECT,
		D3DPOOL_MANAGED,
		&g_pVertexBuffer,
		NULL
	);

	pDevice->CreateIndexBuffer(
		sizeof(WORD) * 6 * EFFECT_COUNT_MAX,      //���܂�ɑ�ʂ̃G�t�F�N�g�̏ꍇ�̓_�u��WORD�^���g���AD3DFMT_INDEX16��16��32�ɕύX
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIndexBuffer,
		NULL
	);
}

void Effect_Update(void)
{
	for (int i = 0;i < EFFECT_COUNT_MAX;i++)
	{
		if (g_Effect[i].life >= 0)
		{
			int age = g_EffectFrameCount - g_Effect[i].birth;
			if (age > g_Effect[i].life)
			{
				g_Effect[i].life = -1;
				continue;
			}
			g_Effect[i].alpha = 1.0f - (float)age / g_Effect[i].life;
			D3DXCOLOR c = g_Effect[i].color;
			c.a = g_Effect[i].alpha;
			g_Effect[i].color = c;
			g_Effect[i].radius *= 0.98f;
		}
	}
	g_EffectFrameCount++;
}

void Effect_Draw(void)
{
	EffectVertex *pV;                            //�����ɕ`��f�[�^���u���ׂāv��������
	g_pVertexBuffer->Lock(0, 0, (void**)&pV, 0);
	
	LPWORD pIndex;
	g_pIndexBuffer->Lock(0, 0, (void**)&pIndex, 0);

	//�E�����玞�v���ŕ`��
	WORD n = 0;                                   //�G�t�F�N�g�̗L�����̐�
	for (int i = 0;i < EFFECT_COUNT_MAX;i++)
	{
		if (g_Effect[i].life >= 0)
		{
			for (int j = 0;j < 4;j++)            //j�͒��_�̂���
			{
				pV[n * 4 + j].position.x = cos(D3DXToRadian(45 + j * 90)) * g_Effect[i].radius + g_Effect[i].x;
				pV[n * 4 + j].position.y = sin(D3DXToRadian(45 + j * 90)) * g_Effect[i].radius + g_Effect[i].y;
				pV[n * 4 + j].position.z = 1.0f;
				pV[n * 4 + j].position.w = 1.0f;
				pV[n * 4 + j].color = g_Effect[i].color;
			}

			//���ӁF����E�����玞�v���ɒl�����Ă���̂Ńe�N�X�`�����W�ɒ���!!
			pV[n * 4 + 0].texcoord = D3DXVECTOR2(1.0f, 1.0f);
			pV[n * 4 + 1].texcoord = D3DXVECTOR2(0.0f, 1.0f);
			pV[n * 4 + 2].texcoord = D3DXVECTOR2(0.0f, 0.0f);
			pV[n * 4 + 3].texcoord = D3DXVECTOR2(1.0f, 0.0f);

			//���ӁF�G�t�F�N�g�̒��_�z��ԍ��ƃC���f�b�N�X�̑���ԍ��𓝈ꂷ��
			pIndex[n * 6 + 0] = 0 + n * 4;
			pIndex[n * 6 + 1] = 1 + n * 4;
			pIndex[n * 6 + 2] = 2 + n * 4;
			pIndex[n * 6 + 3] = 2 + n * 4;
			pIndex[n * 6 + 4] = 3 + n * 4;
			pIndex[n * 6 + 5] = 0 + n * 4;

			n++;
		}
	}

	g_pVertexBuffer->Unlock();
	g_pIndexBuffer->Unlock();

	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(EffectVertex));     //�g�p���钸�_�o�b�t�@�̎w��
	pDevice->SetIndices(g_pIndexBuffer);
	pDevice->SetFVF(FVF_EFFECT);
	pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_EFFECT));

	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);                                           //���u�����f�B���O�����Z�����ɕύX

	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0, 0,
		n * 4,                      //���A�L���ȃG�t�F�N�g�̒��_��
		0,
		n * 2                      //���A�L���ȃG�t�F�N�g�̒��_�̑g�ݍ��킹��
	);


	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);                                   //���u�����f�B���O�����Ƃɖ߂�
}

void Effect_Uninit(void)
{
	if (g_pIndexBuffer)
	{
		g_pIndexBuffer->Release();
		g_pIndexBuffer = NULL;
	}

	if (g_pVertexBuffer)
	{
		g_pVertexBuffer->Release();
		g_pVertexBuffer = NULL;
	}
}

void Effect_Create(float x, float y, int life, float radius, D3DCOLOR color)    //��O�����F�G�t�F�N�g�̎���, ��l�����F�|���S���̑Ίp���̔���
{
	for (int i = 0;i, EFFECT_COUNT_MAX;i++)
	{
		if (g_Effect[i].life < 0)
		{
			g_Effect[i].alpha = 1.0f;
			g_Effect[i].birth = g_EffectFrameCount;
			g_Effect[i].color = color;
			g_Effect[i].life = life;
			g_Effect[i].radius = radius;
			g_Effect[i].x = x;
			g_Effect[i].y = y;
			break;
		}
	}
}
