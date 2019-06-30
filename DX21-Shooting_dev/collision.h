#ifndef COLLISION_H_
#define COLLISION_H_

//==============================================
//構造体宣言
//==============================================
typedef struct Circle_tag
{
	float cx, cy;
	float radius;
}Circle;

typedef struct
{
	float cx, cy;
}Point;

typedef struct
{
	Point top_left;
	Point top_right;
	Point bottom_left;
	Point bottom_right;
}Square;

typedef struct
{
	Square SquarePos;
	float cx;
	float cy;
	float rotate;
}Square_Rotate;

//==============================================
//プロトタイプ宣言
//==============================================
bool Collision_IsHitCircle(const Circle* pA, const Circle* pB);
bool Collision_IsHitPointToCircle(const Circle* pA, const Point* pB);
bool Collision_IsHitPointToSquare(const Square* pA, const Point* pB);
bool Collision_IsHitPointToSquareRotate(const Square_Rotate* pA, const Point* pB);
bool Collision_IsHitStage(const Square* pA, const Square_Rotate* pB);

#endif
