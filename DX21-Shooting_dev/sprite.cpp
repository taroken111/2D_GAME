#include "sprite.h"
#include "mydirect3d.h"
#include "Texture.h"
#include "DebugPrintf.h"
#include <d3dx9.h>
#include <math.h>

//**************************************************************
//�\����
//**************************************************************
typedef struct Vertex2D_tag
{
	D3DXVECTOR4 position;                                                                            //�|���S���̍��W
	D3DCOLOR color;                                                                                  //�|���S���̐F�ݒ�
	D3DXVECTOR2 texcoord;                                                                            //�e�N�X�`�����W�ݒ�
}Vertex2D;                                                                                           //�|���S���̏��

#define FVF_VERTEX2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)                                                //RHW��1���ĂȂ��Ɠ񎟌������Ȃ�

//**************************************************************
//�O���[�o���ϐ�
//**************************************************************
D3DCOLOR g_Color;


//**************************************************************
//�|���S���`��
//**************************************************************
void Sprite_Draw(int texture_index, float dx, float dy)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice)
	{
		return;
	}

	int width  = Texture_GetWidth(texture_index);
	int height = Texture_GetHeight(texture_index);

	Vertex2D v[] = {
		{ D3DXVECTOR4(dx         - 0.5f, dy          - 0.5f, 1.0f, 1.0f), g_Color, D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR4(dx + width - 0.5f, dy          - 0.5f, 1.0f, 1.0f), g_Color, D3DXVECTOR2(1.0f, 0.0f) },
		{ D3DXVECTOR4(dx         - 0.5f, dy + height - 0.5f, 1.0f, 1.0f), g_Color, D3DXVECTOR2(0.0f, 1.0f) },
		{ D3DXVECTOR4(dx + width - 0.5f, dy + height - 0.5f, 1.0f, 1.0f), g_Color, D3DXVECTOR2(1.0f, 1.0f) }
	};
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);                                                    //���u�����f�B���O�̗L����
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);                                               //�|���S�����̃��u�����f�B���O�̐ݒ�
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);                                           //��ʑ��̃��u�����f�B���O�̐ݒ�

	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);                                        //�|���S���ƃe�N�X�`���̃u�����h
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	
	pDevice->SetTexture(0, Texture_GetTexture(texture_index));                                                            //GetTexture�̈�����ς��邱�Ƃŉ摜���u�������
	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));
}

//**************************************************************
//�|���S���`��(���A�����̕ύX�\)
//**************************************************************
void Sprite_DrawWH(int texture_index, float dx, float dy, float width, float height)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice)
	{
		return;
	}

	Vertex2D v[] = {
		{ D3DXVECTOR4(dx - 0.5f, dy - 0.5f, 1.0f, 1.0f), g_Color, D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR4(dx + width - 0.5f, dy - 0.5f, 1.0f, 1.0f), g_Color, D3DXVECTOR2(1.0f, 0.0f) },
		{ D3DXVECTOR4(dx - 0.5f, dy + height - 0.5f, 1.0f, 1.0f), g_Color, D3DXVECTOR2(0.0f, 1.0f) },
		{ D3DXVECTOR4(dx + width - 0.5f, dy + height - 0.5f, 1.0f, 1.0f), g_Color, D3DXVECTOR2(1.0f, 1.0f) }
	};
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);                                                    //���u�����f�B���O�̗L����
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);                                               //�|���S�����̃��u�����f�B���O�̐ݒ�
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);                                           //��ʑ��̃��u�����f�B���O�̐ݒ�

	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);                                        //�|���S���ƃe�N�X�`���̃u�����h
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	pDevice->SetTexture(0, Texture_GetTexture(texture_index));                                                            //GetTexture�̈�����ς��邱�Ƃŉ摜���u�������
	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));
}

//**************************************************************
//�e�N�X�`���̐؂���p�֐�
//**************************************************************
void Sprite_Draw(int texture_index, float dx, float dy, float tx, float ty, float tw, float th, bool flip)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice)
	{
		return;
	}

	float buf = 0.0f;

	int width = Texture_GetWidth(texture_index);
	int height = Texture_GetHeight(texture_index);

	float u0 = (float)(tx) / width;
	float v0 = (float)(ty) / height;
	float u1 = (float)(tx + tw) / width;
	float v1 = (float)(ty + th) / height;

	if (flip)
	{
		buf = u0;
		u0 = u1;
		u1 = buf;
	}

	Vertex2D v[] = {
		{ D3DXVECTOR4(dx         - 0.5f, dy          - 0.5f, 1.0f, 1.0f), g_Color, D3DXVECTOR2(u0, v0) },
		{ D3DXVECTOR4(dx    + tw - 0.5f, dy          - 0.5f, 1.0f, 1.0f), g_Color, D3DXVECTOR2(u1, v0) },
		{ D3DXVECTOR4(dx         - 0.5f, dy     + th - 0.5f, 1.0f, 1.0f), g_Color, D3DXVECTOR2(u0, v1) },
		{ D3DXVECTOR4(dx    + tw - 0.5f, dy     + th - 0.5f, 1.0f, 1.0f), g_Color, D3DXVECTOR2(u1, v1) }
	};
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);                                                    //���u�����f�B���O�̗L����
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);                                               //�|���S�����̃��u�����f�B���O�̐ݒ�
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);                                           //��ʑ��̃��u�����f�B���O�̐ݒ�

	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);                                        //�|���S���ƃe�N�X�`���̃u�����h
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	pDevice->SetTexture(0, Texture_GetTexture(texture_index));                                                            //GetTexture�̈�����ς��邱�Ƃŉ摜���u�������
	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));
}

//**************************************************************
//�e�N�X�`���̐؂��� + ��] + �g��E�k���p�֐�
//**************************************************************
void Sprite_Draw(int texture_index, float dx, float dy, float tx, float ty, float tw, float th, bool flip, float cx, float cy, float sx, float sy, float rotation)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice)
	{
		return;
	}

	float buf = 0.0f;

	int width = Texture_GetWidth(texture_index);
	int height = Texture_GetHeight(texture_index);

	float u0 = (float)(tx) / width;
	float v0 = (float)(ty) / height;
	float u1 = (float)(tx + tw) / width;
	float v1 = (float)(ty + th) / height;
	//===================================================================
	//��]�̒��S���W����ʍ���ɂ��낦��
	//===================================================================
	float rx0 = dx - cx;
	float ry0 = dy - cy;
	float rx1 = dx + tw - cx;
	float ry1 = dy - cy;
	float rx2 = dx - cx;
	float ry2 = dy + th - cy;
	float rx3 = dx + tw - cx;
	float ry3 = dy + th - cy;

	//===================================================================
	//��]�v�Z
	//===================================================================
	float px0 = rx0 * sx * cos(rotation) - ry0 * sy * sin(rotation);
	float py0 = rx0 * sx * sin(rotation) + ry0 * sy * cos(rotation);
	float px1 = rx1 * sx * cos(rotation) - ry1 * sy * sin(rotation);
	float py1 = rx1 * sx * sin(rotation) + ry1 * sy * cos(rotation);
	float px2 = rx2 * sx * cos(rotation) - ry2 * sy * sin(rotation);
	float py2 = rx2 * sx * sin(rotation) + ry2 * sy * cos(rotation);
	float px3 = rx3 * sx * cos(rotation) - ry3 * sy * sin(rotation);
	float py3 = rx3 * sx * sin(rotation) + ry3 * sy * cos(rotation);

	//===================================================================
	//����Ɉړ��������������Ƃɖ߂�
	//===================================================================
	px0 += cx;
	py0 += cy;
	px1 += cx;
	py1 += cy;
	px2 += cx;
	py2 += cy;
	px3 += cx;
	py3 += cy;


	if (flip)
	{
		buf = u0;
		u0 = u1;
		u1 = buf;
	}

	Vertex2D v[] = {
		{ D3DXVECTOR4(px0 - 0.5f, py0 - 0.5f, 1.0f, 1.0f), g_Color, D3DXVECTOR2(u0, v0) },
		{ D3DXVECTOR4(px1 - 0.5f, py1 - 0.5f, 1.0f, 1.0f), g_Color, D3DXVECTOR2(u1, v0) },
		{ D3DXVECTOR4(px2 - 0.5f, py2 - 0.5f, 1.0f, 1.0f), g_Color, D3DXVECTOR2(u0, v1) },
		{ D3DXVECTOR4(px3 - 0.5f, py3 - 0.5f, 1.0f, 1.0f), g_Color, D3DXVECTOR2(u1, v1) }
	};
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);                                                    //���u�����f�B���O�̗L����
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);                                               //�|���S�����̃��u�����f�B���O�̐ݒ�
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);                                           //��ʑ��̃��u�����f�B���O�̐ݒ�

	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);                                        //�|���S���ƃe�N�X�`���̃u�����h
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	pDevice->SetTexture(0, Texture_GetTexture(texture_index));                                                            //GetTexture�̈�����ς��邱�Ƃŉ摜���u�������
	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));
}

//**************************************************************
//�g��E�k���p�֐�
//**************************************************************
void Sprite_Draw(int texture_index, float dx, float dy, float sx, float sy)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice)
	{
		return;
	}

	int width = Texture_GetWidth(texture_index);
	int height = Texture_GetHeight(texture_index);

	Vertex2D v[] = {
		{ D3DXVECTOR4(dx           * sx - 0.5f, dy            * sy - 0.5f, 1.0f, 1.0f), g_Color, D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR4((dx + width) * sx - 0.5f, dy            * sy - 0.5f, 1.0f, 1.0f), g_Color, D3DXVECTOR2(1.0f, 0.0f) },
		{ D3DXVECTOR4(dx           * sx - 0.5f, (dy + height) * sy - 0.5f, 1.0f, 1.0f), g_Color, D3DXVECTOR2(0.0f, 1.0f) },
		{ D3DXVECTOR4((dx + width) * sx - 0.5f, (dy + height) * sy - 0.5f, 1.0f, 1.0f), g_Color, D3DXVECTOR2(1.0f, 1.0f) }
	};
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);                                                    //���u�����f�B���O�̗L����
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);                                               //�|���S�����̃��u�����f�B���O�̐ݒ�
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);                                           //��ʑ��̃��u�����f�B���O�̐ݒ�

	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);                                        //�|���S���ƃe�N�X�`���̃u�����h
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	pDevice->SetTexture(0, Texture_GetTexture(texture_index));                                                            //GetTexture�̈�����ς��邱�Ƃŉ摜���u�������
	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));
}

//**************************************************************
//�|���S���J���[�ݒ�
//**************************************************************
void Sprite_SetColor(D3DCOLOR color)
{
	g_Color = color;
}

//**************************************************************
//�u�����f�B���O�ݒ�(���𓧉߂���)
//**************************************************************
void Sprite_DrawBlackToTransparent(int texture_index, float dx, float dy)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice)
	{
		return;
	}

	int width = Texture_GetWidth(texture_index);
	int height = Texture_GetHeight(texture_index);

	Vertex2D v[] = {
		{ D3DXVECTOR4(dx - 0.5f, dy - 0.5f, 1.0f, 1.0f), g_Color, D3DXVECTOR2(0.0f, 0.0f) },
		{ D3DXVECTOR4(dx + width - 0.5f, dy - 0.5f, 1.0f, 1.0f), g_Color, D3DXVECTOR2(1.0f, 0.0f) },
		{ D3DXVECTOR4(dx - 0.5f, dy + height - 0.5f, 1.0f, 1.0f), g_Color, D3DXVECTOR2(0.0f, 1.0f) },
		{ D3DXVECTOR4(dx + width - 0.5f, dy + height - 0.5f, 1.0f, 1.0f), g_Color, D3DXVECTOR2(1.0f, 1.0f) }
	};
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);                                                    //���u�����f�B���O�̗L����
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);                                               //�|���S�����̃��u�����f�B���O�̐ݒ�
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);                                                   //���u�����f�B���O�����Z�����ɕύX

	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);                                        //�|���S���ƃe�N�X�`���̃u�����h
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	pDevice->SetTexture(0, Texture_GetTexture(texture_index));                                                            //GetTexture�̈�����ς��邱�Ƃŉ摜���u�������
	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));

	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);                                           //���u�����f�B���O�����Ƃɖ߂�
}
