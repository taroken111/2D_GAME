#include <d3dx9.h>
#include "effect.h"
#include "mydirect3d.h"
#include "Texture.h"

//*******************************************************************
//マクロ定義
//*******************************************************************
#define EFFECT_COUNT_MAX     (2048)     //エフェクトの最大数

//*******************************************************************
//構造体宣言
//*******************************************************************
typedef struct EffectVertex2D_tag
{
	D3DXVECTOR4 position;                                                                            //ポリゴンの座標
	D3DCOLOR color;                                                                                  //ポリゴンの色設定
	D3DXVECTOR2 texcoord;                                                                            //テクスチャ座標設定
}EffectVertex;                                                                                     //ポリゴンの情報
#define FVF_EFFECT (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)                                  //RHWに1立てないと二次元化しない

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
//グローバル変数
//*******************************************************************
static EFFECT g_Effect[EFFECT_COUNT_MAX];
static int g_EffectFrameCount = 0;
static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;   //必要な頂点数
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;     //頂点の組み合わせ方

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
		sizeof(WORD) * 6 * EFFECT_COUNT_MAX,      //あまりに大量のエフェクトの場合はダブルWORD型を使い、D3DFMT_INDEX16の16を32に変更
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
	EffectVertex *pV;                            //ここに描画データを「すべて」書き込む
	g_pVertexBuffer->Lock(0, 0, (void**)&pV, 0);
	
	LPWORD pIndex;
	g_pIndexBuffer->Lock(0, 0, (void**)&pIndex, 0);

	//右下から時計回りで描画
	WORD n = 0;                                   //エフェクトの有効中の数
	for (int i = 0;i < EFFECT_COUNT_MAX;i++)
	{
		if (g_Effect[i].life >= 0)
		{
			for (int j = 0;j < 4;j++)            //jは頂点のこと
			{
				pV[n * 4 + j].position.x = cos(D3DXToRadian(45 + j * 90)) * g_Effect[i].radius + g_Effect[i].x;
				pV[n * 4 + j].position.y = sin(D3DXToRadian(45 + j * 90)) * g_Effect[i].radius + g_Effect[i].y;
				pV[n * 4 + j].position.z = 1.0f;
				pV[n * 4 + j].position.w = 1.0f;
				pV[n * 4 + j].color = g_Effect[i].color;
			}

			//注意：今回右下から時計回りに値を入れているのでテクスチャ座標に注意!!
			pV[n * 4 + 0].texcoord = D3DXVECTOR2(1.0f, 1.0f);
			pV[n * 4 + 1].texcoord = D3DXVECTOR2(0.0f, 1.0f);
			pV[n * 4 + 2].texcoord = D3DXVECTOR2(0.0f, 0.0f);
			pV[n * 4 + 3].texcoord = D3DXVECTOR2(1.0f, 0.0f);

			//注意：エフェクトの頂点配列番号とインデックスの代入番号を統一する
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

	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(EffectVertex));     //使用する頂点バッファの指定
	pDevice->SetIndices(g_pIndexBuffer);
	pDevice->SetFVF(FVF_EFFECT);
	pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_EFFECT));

	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);                                           //αブレンディングを加算合成に変更

	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0, 0,
		n * 4,                      //今、有効なエフェクトの頂点数
		0,
		n * 2                      //今、有効なエフェクトの頂点の組み合わせ数
	);


	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);                                   //αブレンディングをもとに戻す
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

void Effect_Create(float x, float y, int life, float radius, D3DCOLOR color)    //第三引数：エフェクトの寿命, 第四引数：ポリゴンの対角線の半分
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
