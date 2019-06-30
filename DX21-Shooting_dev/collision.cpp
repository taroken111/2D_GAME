//============================================
//ヘッダインクルード
//============================================
#include "collision.h"
#include <math.h>
//============================================
//当たり判定(円同士)
//============================================
//デバッグ時に当たり判定の範囲がわかるように円に近い図形を描画するプログラムを作るとやりやすい
bool Collision_IsHitCircle(const Circle* pA, const Circle* pB)
{
	float ab_r = pA->radius + pB->radius;                                                      //円Aと円Bの半径の和

	float ll = (pA->cx - pB->cx) * (pA->cx - pB->cx) + (pA->cy - pB->cy) * (pA->cy - pB->cy);  //円Aの中心座標と円Bの中心座標の距離

	return (ab_r * ab_r) >= ll;
}

//============================================
//当たり判定(円と点)
//============================================
bool Collision_IsHitPointToCircle(const Circle* pA, const Point* pB)
{
	float distance;
	distance = (pA->cx - pB->cx) * (pA->cx - pB->cx) + (pA->cy - pB->cy) * (pA->cy - pB->cy);
	return (pA->radius * pA->radius) >= distance;
}

//============================================
//当たり判定(矩形と点)
//============================================
bool Collision_IsHitPointToSquare(const Square* pA, const Point* pB)
{
	if ((pA->top_left.cx <= pB->cx) && (pA->top_right.cx >= pB->cx))
	{
		if ((pA->top_left.cy <= pB->cy) && (pA->bottom_right.cy >= pB->cy))
		{
			return true;
		}
	}

	return false;
}

//============================================
//当たり判定(傾いた矩形と点)
//============================================
bool Collision_IsHitPointToSquareRotate(const Square_Rotate* pA, const Point* pB)
{
	Point PosXY;

	float BufX = pB->cx;
	float BufY = pB->cy;
	float ReverseRotate = (pA->rotate) * -1.0f;

	//PointをSquare_Rotateの中心座標を中心にReverseRotate分回転
	BufX -= pA->cx;
	BufY -= pA->cy;
	PosXY.cx = BufX * cos(ReverseRotate) - BufY * sin(ReverseRotate) + pA->cx;
	PosXY.cy = BufX * sin(ReverseRotate) + BufY * cos(ReverseRotate) + pA->cy;

	//傾きをもとに戻した矩形と点の当たり判定
	if (Collision_IsHitPointToSquare(&pA->SquarePos, &PosXY))
	{
		return true;
	}
	else
	{
		return false;
	}

}

//============================================
//当たり判定(障害物とプレイヤー等)
//============================================
bool Collision_IsHitStage(const Square* pA, const Square_Rotate* pB)
{
	Point WorkBL, WorkBR, WorkTL, WorkTR;
	float ReverseRotate = (pB->rotate);

	WorkBL.cx = pB->SquarePos.bottom_left.cx - pB->cx;
	WorkBL.cy = pB->SquarePos.bottom_left.cy - pB->cy;

	WorkBR.cx = pB->SquarePos.bottom_right.cx - pB->cx;
	WorkBR.cy = pB->SquarePos.bottom_right.cy - pB->cy;

	WorkTL.cx = pB->SquarePos.top_left.cx - pB->cx;
	WorkTL.cy = pB->SquarePos.top_left.cy - pB->cy;

	WorkTR.cx = pB->SquarePos.top_right.cx - pB->cx;
	WorkTR.cy = pB->SquarePos.top_right.cy - pB->cy;

	WorkBL.cx = (pB->SquarePos.bottom_left.cx - pB->cx) * cos(ReverseRotate) - (pB->SquarePos.bottom_left.cy - pB->cy) * sin(ReverseRotate) + pB->cx;
	WorkBL.cy = (pB->SquarePos.bottom_left.cx - pB->cx) * sin(ReverseRotate) + (pB->SquarePos.bottom_left.cy - pB->cy) * cos(ReverseRotate) + pB->cy;

	WorkBR.cy = (pB->SquarePos.bottom_right.cx - pB->cx) * sin(ReverseRotate) + (pB->SquarePos.bottom_right.cy - pB->cy) * cos(ReverseRotate) + pB->cy;
	WorkBR.cx = (pB->SquarePos.bottom_right.cx - pB->cx) * cos(ReverseRotate) - (pB->SquarePos.bottom_right.cy - pB->cy) * sin(ReverseRotate) + pB->cx;

	WorkTL.cx = (pB->SquarePos.top_left.cx - pB->cx) * cos(ReverseRotate) - (pB->SquarePos.top_left.cy - pB->cy) * sin(ReverseRotate) + pB->cx;
	WorkTL.cy = (pB->SquarePos.top_left.cx - pB->cx) * sin(ReverseRotate) + (pB->SquarePos.top_left.cy - pB->cy) * cos(ReverseRotate) + pB->cy;

	WorkTR.cx = (pB->SquarePos.top_right.cx - pB->cx) * cos(ReverseRotate) - (pB->SquarePos.top_right.cy - pB->cy) * sin(ReverseRotate) + pB->cx;
	WorkTR.cy = (pB->SquarePos.top_right.cx - pB->cx) * sin(ReverseRotate) + (pB->SquarePos.top_right.cy - pB->cy) * cos(ReverseRotate) + pB->cy;

	//垂直な物体の中に回転している矩形の4頂点いずれかが入っているのか
	if (Collision_IsHitPointToSquare(pA, &WorkBL) || Collision_IsHitPointToSquare(pA, &WorkBR) || Collision_IsHitPointToSquare(pA, &WorkTL) || Collision_IsHitPointToSquare(pA, &WorkTR))
	{
		return true;
	}

	//斜めの矩形の中に障害物の4頂点のいずれかが入っているのか
	if (Collision_IsHitPointToSquareRotate(pB, &pA->bottom_left) || Collision_IsHitPointToSquareRotate(pB, &pA->bottom_right) || Collision_IsHitPointToSquareRotate(pB, &pA->top_left) || Collision_IsHitPointToSquareRotate(pB, &pA->top_right))
	{
		return true;
	}

	return false;
}