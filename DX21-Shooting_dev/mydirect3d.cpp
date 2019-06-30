//*******************************************************************************************************************************************
//�쐬���F5��29��
//�e�[�}�u�֐�����(mydirect3d.cpp)�v
//�쐬�ҁF���䌫���Y
//*******************************************************************************************************************************************

//***********************************
//�w�b�_�ǂݍ���
//***********************************
#include "mydirect3d.h"
#include "common.h"
#include <windows.h>
#include <stdio.h>
#include "DebugPrintf.h"
#include <d3dx9.h>

//***********************************
//�O���[�o���ϐ�
//***********************************
LPDIRECT3D9 g_pD3D = NULL;                                                                           //Direct3D�C���^�[�t�F�[�X
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;                                                               //Direct3D�f�o�C�X

LPD3DXFONT g_pD3DXFont = NULL;                                                                       //Direct3D�t�H���g�C���^�[�t�F�[�X

//***********************************
//������Ŏ擾�����f�o�C�X��n��
//***********************************
LPDIRECT3DDEVICE9 MyDirect3D_GetDevice(void)
{
	return g_pD3DDevice;
}

//***********************************
//������Ŏ擾�����t�H���g�C���^�[�t�F�[�X��n��
//***********************************
LPD3DXFONT MyDirect3D_GetFont(void)
{
	return g_pD3DXFont;
}

//***********************************
//�C���^�[�t�F�[�X�ƃf�o�C�X�̏�����
//***********************************
bool MyDirect3D_Init(HWND hWnd)
{
	//�C���^�[�t�F�[�X�̎擾
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == false)
	{
		DebugPrintf("\n\n\n�C���^�[�t�F�[�X�̎擾���s\n\n\n");
		return false;     //�擾���s
	}

	//�f�o�C�X�̎擾
	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;                          //Stencil����������g���ꍇD3DFMT_D24S8�ɐݒ��ς���
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;           //����������҂��Ȃ��ꍇ��D3DPRESENT_INTERVAL_IMMEDIATE��ݒ�
																		/*���ꂢ�ȉ�ʂ�`�悷��ꍇMultiSample��ݒ�*/
	HRESULT hr = g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice);
	if (FAILED(hr))
	{
		DebugPrintf("\n\n\nCreateDevice�֐����s\n\n\n");
		return false;                                                   //�֐����s
	}

	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pD3DXFont);

	return true;
}

//***********************************
//�C���^�[�t�F�[�X�ƃf�o�C�X�̉��
//***********************************
void MyDirect3D_Uninit()
{
	g_pD3DXFont->Release();
	g_pD3DDevice->Release();                                            //�f�o�C�X�̉��
	g_pD3D->Release();                                                  //�C���^�[�t�F�[�X�̉��
}