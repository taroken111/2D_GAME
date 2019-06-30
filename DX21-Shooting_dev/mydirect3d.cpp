//*******************************************************************************************************************************************
//作成日：5月29日
//テーマ「関数分け(mydirect3d.cpp)」
//作成者：藤井賢太郎
//*******************************************************************************************************************************************

//***********************************
//ヘッダ読み込み
//***********************************
#include "mydirect3d.h"
#include "common.h"
#include <windows.h>
#include <stdio.h>
#include "DebugPrintf.h"
#include <d3dx9.h>

//***********************************
//グローバル変数
//***********************************
LPDIRECT3D9 g_pD3D = NULL;                                                                           //Direct3Dインターフェース
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;                                                               //Direct3Dデバイス

LPD3DXFONT g_pD3DXFont = NULL;                                                                       //Direct3Dフォントインターフェース

//***********************************
//こちらで取得したデバイスを渡す
//***********************************
LPDIRECT3DDEVICE9 MyDirect3D_GetDevice(void)
{
	return g_pD3DDevice;
}

//***********************************
//こちらで取得したフォントインターフェースを渡す
//***********************************
LPD3DXFONT MyDirect3D_GetFont(void)
{
	return g_pD3DXFont;
}

//***********************************
//インターフェースとデバイスの初期化
//***********************************
bool MyDirect3D_Init(HWND hWnd)
{
	//インターフェースの取得
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == false)
	{
		DebugPrintf("\n\n\nインターフェースの取得失敗\n\n\n");
		return false;     //取得失敗
	}

	//デバイスの取得
	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;                          //Stencilをしっかり使う場合D3DFMT_D24S8に設定を変える
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;           //垂直同期を待たない場合はD3DPRESENT_INTERVAL_IMMEDIATEを設定
																		/*きれいな画面を描画する場合MultiSampleを設定*/
	HRESULT hr = g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice);
	if (FAILED(hr))
	{
		DebugPrintf("\n\n\nCreateDevice関数失敗\n\n\n");
		return false;                                                   //関数失敗
	}

	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pD3DXFont);

	return true;
}

//***********************************
//インターフェースとデバイスの解放
//***********************************
void MyDirect3D_Uninit()
{
	g_pD3DXFont->Release();
	g_pD3DDevice->Release();                                            //デバイスの解放
	g_pD3D->Release();                                                  //インターフェースの解放
}