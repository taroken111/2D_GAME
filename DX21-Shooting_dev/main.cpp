//***************************************************************************************************
//作成日 6月19日
//テーマ:「シューティングゲーム制作」
//作成者:藤井 賢太郎
//***************************************************************************************************

//===================================================================================================================================================================================
#include <windows.h>
#include <stdio.h>
#include "DebugPrintf.h"
#include "DebugText.h"
#include <d3dx9.h>
#include "mydirect3d.h"
#include "Texture.h"
#include "sprite.h"
#include "SpriteAnimation.h"
#include "game.h"
#include "input.h"
#include "system_timer.h"
#include "scene.h"
#include "fade.h"
#include "sound.h"
#include "common.h"

//---------------------------------------------------------------------------------------------------
//定数定義
//---------------------------------------------------------------------------------------------------
#define CLASS_NAME          "GameWindow"
#define WINDOW_CAPTION      "画面遷移"
#define SCREEN_WIDTH  (1280)                                                                        //ウィンドウの横幅
#define SCREEN_HEIGHT (720)                                                                         //ウィンドウの縦幅
#define START_SCENE   (SCENE_INDEX_TITLE)

//---------------------------------------------------------------------------------------------------
//プロトタイプ宣言
//---------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

bool Init(HINSTANCE hInstance, HWND hWnd);
void Update(HWND hWnd);
void Draw(void);
void Uninit(void);

//---------------------------------------------------------------------------------------------------
//構造体
//---------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------
//グローバル変数
//---------------------------------------------------------------------------------------------------
LPDIRECT3D9 g_pMyD3D = NULL;                                                                           //Direct3Dインターフェース(main.cpp)
LPDIRECT3DDEVICE9 g_pMyD3DDevice = NULL;														       //Direct3Dデバイス(main.cpp用)
LPD3DXFONT g_pFont = NULL;                                                                             //Direct3Dフォントインターフェース(main.cpp用)

static int g_FlameCounter = 0;
static int g_FPSBaseFlameCount = 0;
static double g_FPSBaseTime = 0;

int flame_count;                                                                                       //フレームカウンタ
float g_Rotation = 0.0f;

//---------------------------------------------------------------------------------------------------
//メイン
//---------------------------------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	RECT window_rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};                                         //実際に使えるウィンドウサイズの指定

	DWORD window_style = WS_OVERLAPPEDWINDOW ^ (WS_MAXIMIZEBOX | WS_THICKFRAME);                    //ウィンドウスタイルをDWORD型で設定

	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = CLASS_NAME;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);

	RegisterClass(&wc);
	
	AdjustWindowRect(&window_rect, window_style, FALSE);                                            //追加：実際に使えるウィンドウサイズの指定
	int window_width = window_rect.right - window_rect.left;                                        //追加：ウィンドウの横幅
	int window_height = window_rect.bottom - window_rect.top;                                       //追加：ウィンドウの縦幅
	int desktop_width = GetSystemMetrics(SM_CXSCREEN);                                              //追加：デスクトップの横幅
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);                                             //追加：デスクトップの縦幅
	int window_x = max((desktop_width - window_width) / 2, 0);                                      //追加：ウィンドウの始点のX座標
	int window_y = max((desktop_height - window_height) / 2, 0);                                    //追加：ウィンドウの始点のY座標

	HWND hWnd = CreateWindow(
		CLASS_NAME,
		WINDOW_CAPTION,
		window_style,                                                                               //ゲームをフルスクリーンでやる場合WS_POPUPスタイルにする
		window_x,                                                                                   //CW_USEDEFAULTでいい感じに設定してくれる
		window_y,
        window_width,
		window_height,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!Init(hInstance, hWnd))
	{
		return 0;
	}

	ShowWindow(hWnd, nCmdShow);

	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//↓ここからゲーム処理(更新と描画)↓
			double time = SystemTimer_GetTime();
			if (time - g_FPSBaseTime >= 1.0 / 60.0) //1 / 60 秒経つまで待機
			{
				//char buf[256];
				//sprintf(buf, "%.2f fps", ((time - g_FPSBaseTime)));
				//SetWindowText(hWnd, buf);
				g_FPSBaseTime = time;

				//ゲームの更新
				Update(hWnd);

				//ゲームの描画
				Draw();

				//フェードの更新
				Fade_Update();

				//シーンのチェック
				Scene_Check();
			}
			else
			{
				//CPUハグ回避
				Sleep(0);
			}
		}
	}

	//ゲームの終了処理
	Uninit();

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		}
		break;

	case WM_CLOSE:
		if (MessageBox(hWnd, "本当に終了していいですか？", "確認", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
		{
			DestroyWindow(hWnd);
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	};

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool Init(HINSTANCE hInstance, HWND hWnd)
{
	//サウンドの初期化
	InitSound(hWnd);


	//キーボードの初期化
	if (!Keyboard_Init(hInstance, hWnd))
	{
		return false;
	}

	//Direct3Dの初期化
	if (!MyDirect3D_Init(hWnd))
	{
		DebugPrintf("\n\n\nCreateDevice関数失敗\n\n\n");
		return false;
	}

	g_pMyD3DDevice = MyDirect3D_GetDevice();
	g_pFont = MyDirect3D_GetFont();

	//テクスチャの初期化
	if (Texture_Load() > 0)
	{
		return false;
	}

	Scene_Init(START_SCENE);

	//アニメーションの初期化
	SpriteAnimation_Init();

	//フェード初期化
	Fade_Init();

	//システムタイマーの初期化
	SystemTimer_Initialize();

	//システムタイマーの起動
	SystemTimer_Start();
	g_FlameCounter = 0;
	g_FPSBaseFlameCount = 0;
	g_FPSBaseTime = SystemTimer_GetTime();

	return true;
}

void Uninit(void)
{
	Scene_Uninit();
	Texture_Release();                                                                                               //テクスチャインターフェースの解放
	MyDirect3D_Uninit();
	Keyboard_Uninit();
	UninitSound();
}

void Update(HWND hWnd)
{
	Keyboard_Update();
	Scene_Update();
	g_FlameCounter++;
	//double time = SystemTimer_GetTime();
	//if (time - g_FPSBaseTime > 1.0f)
	//{
		//char buf[256];
		//sprintf(buf, "%.2f fps", (g_FlameCounter - g_FPSBaseFlameCount / (time - g_FPSBaseTime)));
		//SetWindowText(hWnd, buf);
	//	g_FPSBaseTime = time;
	//	g_FPSBaseFlameCount = g_FlameCounter;
	//}
}

void Draw(void)
{
	g_pMyD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(100, 80, 200, 255), 1.0f, 0);   //画面のクリア(注：黒と白色は避ける)・第五引数は深さの設定で0.0f(最も手前)にしない
	g_pMyD3DDevice->BeginScene();                                                                                    //バッチ処理開始
	/*ここに描画情報を書く*/

	Scene_Draw();
	Fade_Draw();

	/*ここまで*/
	g_pMyD3DDevice->EndScene();                                                                                      //バッチ処理終了
	g_pMyD3DDevice->Present(NULL, NULL, NULL, NULL);
}

LPD3DXFONT GetFont(void)
{
	return g_pFont;
}

int GetFrameCount(void)
{
	return g_FlameCounter;
}