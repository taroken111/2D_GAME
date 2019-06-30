//*******************************************************************
//ヘッダインクルード
//*******************************************************************
#include "fade.h"
#include "common.h"
#include "mydirect3d.h"

//*******************************************************************
//構造体宣言
//*******************************************************************
typedef struct FadeVertex_tag
{
	D3DXVECTOR4 position;
	D3DCOLOR color;
}FadeVertex;

#define FVF_FADE_VERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

static bool g_bFade = false;                                  //フェードしているかどうか
static bool g_bOut = false;                                   //trueならフェードアウト
static float g_Alpha = 0.0f;
static float g_addAlpha = 0.0f;                                  //α値の増減量(1 or 0)
static D3DCOLOR g_color = D3DCOLOR_RGBA(255, 255, 255, 255);
static int g_fadeFrame = 0;
static int g_fadeFrameCount = 0;

//*******************************************************************
//フェード初期化
//*******************************************************************
void Fade_Init(void)
{
	g_bFade = false;
	g_Alpha = 0.0f;
}

//*******************************************************************
//フェード更新
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
//フェード描画
//*******************************************************************
void Fade_Draw(void)
{
	if (g_Alpha <= 0.0000001f)
	{
		//フェードしていないときはフェード用のポリゴンを描画しない
		return;
	}
	D3DXCOLOR color(g_color);   //D3DXCOLOR型：今までのD3DCOLOR内の値をfloatで扱えるようになる(ただし、許可を得てから使う)
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
//フェード開始
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
//フェードの有効無効チェック
//*******************************************************************
bool Fade_IsFade(void)
{
	return g_bFade;
}