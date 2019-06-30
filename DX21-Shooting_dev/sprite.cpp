#include "sprite.h"
#include "mydirect3d.h"
#include "Texture.h"
#include "DebugPrintf.h"
#include <d3dx9.h>
#include <math.h>

//**************************************************************
//構造体
//**************************************************************
typedef struct Vertex2D_tag
{
	D3DXVECTOR4 position;                                                                            //ポリゴンの座標
	D3DCOLOR color;                                                                                  //ポリゴンの色設定
	D3DXVECTOR2 texcoord;                                                                            //テクスチャ座標設定
}Vertex2D;                                                                                           //ポリゴンの情報

#define FVF_VERTEX2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)                                                //RHWに1立てないと二次元化しない

//**************************************************************
//グローバル変数
//**************************************************************
D3DCOLOR g_Color;


//**************************************************************
//ポリゴン描画
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
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);                                                    //αブレンディングの有効化
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);                                               //ポリゴン側のαブレンディングの設定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);                                           //画面側のαブレンディングの設定

	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);                                        //ポリゴンとテクスチャのブレンド
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	
	pDevice->SetTexture(0, Texture_GetTexture(texture_index));                                                            //GetTextureの引数を変えることで画像が置き換わる
	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));
}

//**************************************************************
//ポリゴン描画(幅、高さの変更可能)
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
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);                                                    //αブレンディングの有効化
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);                                               //ポリゴン側のαブレンディングの設定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);                                           //画面側のαブレンディングの設定

	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);                                        //ポリゴンとテクスチャのブレンド
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	pDevice->SetTexture(0, Texture_GetTexture(texture_index));                                                            //GetTextureの引数を変えることで画像が置き換わる
	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));
}

//**************************************************************
//テクスチャの切り取り用関数
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
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);                                                    //αブレンディングの有効化
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);                                               //ポリゴン側のαブレンディングの設定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);                                           //画面側のαブレンディングの設定

	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);                                        //ポリゴンとテクスチャのブレンド
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	pDevice->SetTexture(0, Texture_GetTexture(texture_index));                                                            //GetTextureの引数を変えることで画像が置き換わる
	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));
}

//**************************************************************
//テクスチャの切り取り + 回転 + 拡大・縮小用関数
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
	//回転の中心座標を画面左上にそろえる
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
	//回転計算
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
	//左上に移動させた分をもとに戻す
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
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);                                                    //αブレンディングの有効化
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);                                               //ポリゴン側のαブレンディングの設定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);                                           //画面側のαブレンディングの設定

	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);                                        //ポリゴンとテクスチャのブレンド
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	pDevice->SetTexture(0, Texture_GetTexture(texture_index));                                                            //GetTextureの引数を変えることで画像が置き換わる
	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));
}

//**************************************************************
//拡大・縮小用関数
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
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);                                                    //αブレンディングの有効化
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);                                               //ポリゴン側のαブレンディングの設定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);                                           //画面側のαブレンディングの設定

	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);                                        //ポリゴンとテクスチャのブレンド
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	pDevice->SetTexture(0, Texture_GetTexture(texture_index));                                                            //GetTextureの引数を変えることで画像が置き換わる
	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));
}

//**************************************************************
//ポリゴンカラー設定
//**************************************************************
void Sprite_SetColor(D3DCOLOR color)
{
	g_Color = color;
}

//**************************************************************
//ブレンディング設定(黒を透過する)
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
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);                                                    //αブレンディングの有効化
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);                                               //ポリゴン側のαブレンディングの設定
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);                                                   //αブレンディングを加算合成に変更

	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);                                        //ポリゴンとテクスチャのブレンド
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	pDevice->SetTexture(0, Texture_GetTexture(texture_index));                                                            //GetTextureの引数を変えることで画像が置き換わる
	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));

	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);                                           //αブレンディングをもとに戻す
}
