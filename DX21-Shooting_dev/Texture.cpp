//***************************************************************************************************
//�쐬�� 6��4��
//�e�[�}:�u�֐�����(Texture.cpp)�v
//�쐬��:���� �����Y
//***************************************************************************************************
#include "Texture.h"
#include "mydirect3d.h"
#include "DebugPrintf.h"

//***********************************
//�\���̐錾
//***********************************
typedef struct TextureFile_tag                                                                     //TextureFile_tag�̓^�O���B�Ȃ��Ă��悢
{
	const char filename[TEXTURE_FILENAME_MAX];                                                     //��x�o�^������ς��邱�Ƃ��Ȃ��ϐ���const������
	int width;
	int height;
}TextureFile;                                                                                      //�e�N�X�`���̃f�[�^�x�[�X1���R�[�h������̍\����

//***********************************
//�O���[�o���錾
//***********************************
static TextureFile g_TextureFiles[] = {
	{ "asset/texture/background.jpg", 2560, 1440},
	{ "asset/texture/enemy.png", 128, 128},
	{ "asset/texture/explosion.png", 640, 512},
	{ "asset/texture/number.tga", 320, 32},
	{ "asset/texture/TITLE.png", 1280, 720},
	{ "asset/texture/RESULT.jpg", 1280, 720},
	{ "asset/texture/radar.png", 256, 256},
	{ "asset/texture/enemyradar.png", 20, 20},
	{ "asset/texture/Tank_Body.png", 256, 160},
	{ "asset/texture/Tank_Head.png", 256, 160},
	{ "asset/texture/Bullet_Normal.png", 16, 16},
	{ "asset/texture/effect000.jpg", 80, 80},
	{ "asset/texture/Map.png", 2560, 1440},
	{ "asset/texture/Body_MultiMissile.png", 128, 160},
	{ "asset/texture/Bullet_MultiMissile.png", 16, 16},
	{ "asset/texture/Fire_MultiMissile.png", 16, 16},
	{ "asset/texture/Reticle_MultiMissile.png", 64, 64},
	{ "asset/texture/item_multimissile.png", 32, 32},
	{ "asset/texture/HP_player.png", 1000, 32},
	{ "asset/texture/HP_gauge.png", 1000, 32},
	{ "asset/texture/HG_1.png", 500, 16},
	{ "asset/texture/HG_2.png", 500, 16},
	{ "asset/texture/HG_3.png", 500, 16},
	{ "asset/texture/fireball.png", 32, 32},
	{ "asset/texture/item_normal.png", 64, 64},
	{ "asset/texture/item_speedup.png", 32, 32},
	{ "asset/texture/item_cooldown.png", 32, 32},
	{ "asset/texture/HP_item.jpg", 256, 256},
	{ "asset/texture/GAME_OVER.png", 1280, 720},
	{ "asset/texture/GAME_CLEAR.png", 1280, 720},
	{ "asset/texture/Bomb_Body.png",170, 106},
	{ "asset/texture/item_bigbomb.png", 32, 32},
	{ "asset/texture/HP_name.png", 128, 32},
	{ "asset/texture/SB_name.png", 128, 32},
	{ "asset/texture/life.png", 32, 32},
};
static const int TEXTURE_FILE_COUNT = sizeof(g_TextureFiles) / sizeof(g_TextureFiles[0]);          //�\���̔z��̑S�̂��\���̈������̃f�[�^�Ŋ���Ɖ��e�N�X�`�����ǂݍ��܂ꂽ���Z�o�ł���B
                                                                                                   //�������A�z��̂݁B�|�C���^�ŊǗ����Ă���ꍇ�͂ł��Ȃ�
static LPDIRECT3DTEXTURE9 g_pTexture[TEXTURE_FILE_COUNT];

//***********************************
//�e�N�X�`���ǂݍ���
//***********************************
int Texture_Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	if (!pDevice)
	{
		DebugPrintf("\n\n\nDevice Return NULL!!\n\n\n");
		return TEXTURE_FILE_COUNT;
	}

	HRESULT hr;
	int failed_count = 0;

	for (int i = 0;i < TEXTURE_FILE_COUNT;i++)
	{
		hr = D3DXCreateTextureFromFile(pDevice, g_TextureFiles[i].filename, &g_pTexture[i]);
		if (FAILED(hr))
		{
			DebugPrintf("\n\n\nTexture[%d] Failed!!\n\n\n", i);
			failed_count++;
		}
	}

	return failed_count;
}

//***********************************
//�e�N�X�`��Release
//***********************************
void Texture_Release(void)
{
	for (int i = 0;i < TEXTURE_FILE_COUNT;i++)
	{
		if (g_pTexture[i] != NULL)
		{
			g_pTexture[i]->Release();
			g_pTexture[i] = NULL;
		}
	}
}

//***********************************
//�w�肵���e�N�X�`�����f�o�C�X�ɑ���
//***********************************
LPDIRECT3DTEXTURE9 Texture_GetTexture(int index)
{
	return g_pTexture[index];
}

//***********************************
//�e�N�X�`�����擾
//***********************************
int Texture_GetWidth(int index)
{
	return g_TextureFiles[index].width;
}

//***********************************
//�e�N�X�`�������擾
//***********************************
int Texture_GetHeight(int index)
{
	return g_TextureFiles[index].height;
}
