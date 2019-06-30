//#include <d3dx9.h>
//#include "mydirect3d.h"
//
//
//#define FVF_DEBUG_VERTEX (D3DFVF_XYZ)
//
//static int g_CircleDrawCount = 0;
//static DebugVertex* g_pVertex;
//static WORD* g_pIndex;
//
//
//void DebugPrimitive_Init(void)
//{
//	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
//
//	pDevice->CreateIndexBuffer(
//		sizeof(WORD) * CIRCLE_VERTEX_COUNT * 2 * CIRCLEDRAW
//	)
//}
//
//void DebugPrimitive_BatchBegin(void)
//{
//	g_CircleDrawCount = 0;
//
//	g_pVertexBuffer->Lock(0, 0, (void**)&g_pVertex, 0);
//
//	g_pIndexBuffer->Lock(0, 0, (void**)&g_pIndex, 0);
//}
//
//void DebugPrimitive_BatchDrawCircle(float x, float y)
//{
//	itn n = g_CircleDrawCount * CIRCLE_VERTEX_COUNT;
//	float a = D3DX_PI * 2 / CIRCLE_VERTEX_COUNT;
//	for (int i = 0;i < CIRCLE_VERTEX_COUNT;i++)
//	{
//		g_pVertex[i].position.x = cos(a * i) * radius + x;
//		g_pVertex[i].position.y = sin(a * i) * radius + y;
//		g_pVertex[i].position.z = 1.0f;
//		g_pVertex[i].position.w = 1.0f;
//
//		g_pVertex[i].color = D3DCOLOR_RGBA(0, 255, 0, 255);
//	}
//}
//
//void DebugPrimitive_Uninit(void)
//{
//
//}