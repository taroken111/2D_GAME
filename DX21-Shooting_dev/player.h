#ifndef PLAYER_H_
#define PLAYER_H_

#include <d3dx9.h>
#include "collision.h"

#define TANK_HEAD_CENTER_WIDTH    (107.0f)
#define TANK_HEAD_CENTER_HEIGHT   (72.0f)
//*******************************************************************
//構造体宣言
//*******************************************************************
typedef struct
{
	float PosX;
	float PosY;
	float width;
	float height;
	float Speed;
	float Deg;
	float HeadDeg;
	float Angle;
	float HeadAngle;
	float HP_Player;
	float HP_City;
	float SpeedBooster;

	int CoolTime;
	int HowManySpeedUp;
	int HowManyCoolDown;

	int PlayerScore;
	int ReSpawnCount;
	float Score_Boost;

	D3DXVECTOR2 vecSpeed;

	bool EndGame;
	bool JudgeWinOrLose;
	bool OpenMenu;

	bool BulletType_Normal;
	bool BulletType_MultiMissile;
	bool BulletType_Bomb;
}PLAYER;

//*******************************************************************
//プロトタイプ宣言
//*******************************************************************
void Player_Init(void);
void Player_Update(void);
void Player_Draw(void);
PLAYER* Player_GetPlayer(void);
Circle* Player_GetCollision(void);
Square_Rotate* Player_GetSquareRotate(void);

#endif
