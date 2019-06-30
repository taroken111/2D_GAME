//***************************************************************************************************
//�쐬�� 6��19��
//�e�[�}:�u�V���[�e�B���O�Q�[������v
//�쐬��:���� �����Y
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
//�萔��`
//---------------------------------------------------------------------------------------------------
#define CLASS_NAME          "GameWindow"
#define WINDOW_CAPTION      "��ʑJ��"
#define SCREEN_WIDTH  (1280)                                                                        //�E�B���h�E�̉���
#define SCREEN_HEIGHT (720)                                                                         //�E�B���h�E�̏c��
#define START_SCENE   (SCENE_INDEX_TITLE)

//---------------------------------------------------------------------------------------------------
//�v���g�^�C�v�錾
//---------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

bool Init(HINSTANCE hInstance, HWND hWnd);
void Update(HWND hWnd);
void Draw(void);
void Uninit(void);

//---------------------------------------------------------------------------------------------------
//�\����
//---------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------
//�O���[�o���ϐ�
//---------------------------------------------------------------------------------------------------
LPDIRECT3D9 g_pMyD3D = NULL;                                                                           //Direct3D�C���^�[�t�F�[�X(main.cpp)
LPDIRECT3DDEVICE9 g_pMyD3DDevice = NULL;														       //Direct3D�f�o�C�X(main.cpp�p)
LPD3DXFONT g_pFont = NULL;                                                                             //Direct3D�t�H���g�C���^�[�t�F�[�X(main.cpp�p)

static int g_FlameCounter = 0;
static int g_FPSBaseFlameCount = 0;
static double g_FPSBaseTime = 0;

int flame_count;                                                                                       //�t���[���J�E���^
float g_Rotation = 0.0f;

//---------------------------------------------------------------------------------------------------
//���C��
//---------------------------------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	RECT window_rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};                                         //���ۂɎg����E�B���h�E�T�C�Y�̎w��

	DWORD window_style = WS_OVERLAPPEDWINDOW ^ (WS_MAXIMIZEBOX | WS_THICKFRAME);                    //�E�B���h�E�X�^�C����DWORD�^�Őݒ�

	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = CLASS_NAME;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);

	RegisterClass(&wc);
	
	AdjustWindowRect(&window_rect, window_style, FALSE);                                            //�ǉ��F���ۂɎg����E�B���h�E�T�C�Y�̎w��
	int window_width = window_rect.right - window_rect.left;                                        //�ǉ��F�E�B���h�E�̉���
	int window_height = window_rect.bottom - window_rect.top;                                       //�ǉ��F�E�B���h�E�̏c��
	int desktop_width = GetSystemMetrics(SM_CXSCREEN);                                              //�ǉ��F�f�X�N�g�b�v�̉���
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);                                             //�ǉ��F�f�X�N�g�b�v�̏c��
	int window_x = max((desktop_width - window_width) / 2, 0);                                      //�ǉ��F�E�B���h�E�̎n�_��X���W
	int window_y = max((desktop_height - window_height) / 2, 0);                                    //�ǉ��F�E�B���h�E�̎n�_��Y���W

	HWND hWnd = CreateWindow(
		CLASS_NAME,
		WINDOW_CAPTION,
		window_style,                                                                               //�Q�[�����t���X�N���[���ł��ꍇWS_POPUP�X�^�C���ɂ���
		window_x,                                                                                   //CW_USEDEFAULT�ł��������ɐݒ肵�Ă����
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
			//����������Q�[������(�X�V�ƕ`��)��
			double time = SystemTimer_GetTime();
			if (time - g_FPSBaseTime >= 1.0 / 60.0) //1 / 60 �b�o�܂őҋ@
			{
				//char buf[256];
				//sprintf(buf, "%.2f fps", ((time - g_FPSBaseTime)));
				//SetWindowText(hWnd, buf);
				g_FPSBaseTime = time;

				//�Q�[���̍X�V
				Update(hWnd);

				//�Q�[���̕`��
				Draw();

				//�t�F�[�h�̍X�V
				Fade_Update();

				//�V�[���̃`�F�b�N
				Scene_Check();
			}
			else
			{
				//CPU�n�O���
				Sleep(0);
			}
		}
	}

	//�Q�[���̏I������
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
		if (MessageBox(hWnd, "�{���ɏI�����Ă����ł����H", "�m�F", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK)
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
	//�T�E���h�̏�����
	InitSound(hWnd);


	//�L�[�{�[�h�̏�����
	if (!Keyboard_Init(hInstance, hWnd))
	{
		return false;
	}

	//Direct3D�̏�����
	if (!MyDirect3D_Init(hWnd))
	{
		DebugPrintf("\n\n\nCreateDevice�֐����s\n\n\n");
		return false;
	}

	g_pMyD3DDevice = MyDirect3D_GetDevice();
	g_pFont = MyDirect3D_GetFont();

	//�e�N�X�`���̏�����
	if (Texture_Load() > 0)
	{
		return false;
	}

	Scene_Init(START_SCENE);

	//�A�j���[�V�����̏�����
	SpriteAnimation_Init();

	//�t�F�[�h������
	Fade_Init();

	//�V�X�e���^�C�}�[�̏�����
	SystemTimer_Initialize();

	//�V�X�e���^�C�}�[�̋N��
	SystemTimer_Start();
	g_FlameCounter = 0;
	g_FPSBaseFlameCount = 0;
	g_FPSBaseTime = SystemTimer_GetTime();

	return true;
}

void Uninit(void)
{
	Scene_Uninit();
	Texture_Release();                                                                                               //�e�N�X�`���C���^�[�t�F�[�X�̉��
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
	g_pMyD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(100, 80, 200, 255), 1.0f, 0);   //��ʂ̃N���A(���F���Ɣ��F�͔�����)�E��܈����͐[���̐ݒ��0.0f(�ł���O)�ɂ��Ȃ�
	g_pMyD3DDevice->BeginScene();                                                                                    //�o�b�`�����J�n
	/*�����ɕ`���������*/

	Scene_Draw();
	Fade_Draw();

	/*�����܂�*/
	g_pMyD3DDevice->EndScene();                                                                                      //�o�b�`�����I��
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