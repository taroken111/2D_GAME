//============================================
//�w�b�_�C���N���[�h
//============================================
#include "collision.h"
#include <math.h>
//============================================
//�����蔻��(�~���m)
//============================================
//�f�o�b�O���ɓ����蔻��͈̔͂��킩��悤�ɉ~�ɋ߂��}�`��`�悷��v���O���������Ƃ��₷��
bool Collision_IsHitCircle(const Circle* pA, const Circle* pB)
{
	float ab_r = pA->radius + pB->radius;                                                      //�~A�Ɖ~B�̔��a�̘a

	float ll = (pA->cx - pB->cx) * (pA->cx - pB->cx) + (pA->cy - pB->cy) * (pA->cy - pB->cy);  //�~A�̒��S���W�Ɖ~B�̒��S���W�̋���

	return (ab_r * ab_r) >= ll;
}

//============================================
//�����蔻��(�~�Ɠ_)
//============================================
bool Collision_IsHitPointToCircle(const Circle* pA, const Point* pB)
{
	float distance;
	distance = (pA->cx - pB->cx) * (pA->cx - pB->cx) + (pA->cy - pB->cy) * (pA->cy - pB->cy);
	return (pA->radius * pA->radius) >= distance;
}

//============================================
//�����蔻��(��`�Ɠ_)
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
//�����蔻��(�X������`�Ɠ_)
//============================================
bool Collision_IsHitPointToSquareRotate(const Square_Rotate* pA, const Point* pB)
{
	Point PosXY;

	float BufX = pB->cx;
	float BufY = pB->cy;
	float ReverseRotate = (pA->rotate) * -1.0f;

	//Point��Square_Rotate�̒��S���W�𒆐S��ReverseRotate����]
	BufX -= pA->cx;
	BufY -= pA->cy;
	PosXY.cx = BufX * cos(ReverseRotate) - BufY * sin(ReverseRotate) + pA->cx;
	PosXY.cy = BufX * sin(ReverseRotate) + BufY * cos(ReverseRotate) + pA->cy;

	//�X�������Ƃɖ߂�����`�Ɠ_�̓����蔻��
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
//�����蔻��(��Q���ƃv���C���[��)
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

	//�����ȕ��̂̒��ɉ�]���Ă����`��4���_�����ꂩ�������Ă���̂�
	if (Collision_IsHitPointToSquare(pA, &WorkBL) || Collision_IsHitPointToSquare(pA, &WorkBR) || Collision_IsHitPointToSquare(pA, &WorkTL) || Collision_IsHitPointToSquare(pA, &WorkTR))
	{
		return true;
	}

	//�΂߂̋�`�̒��ɏ�Q����4���_�̂����ꂩ�������Ă���̂�
	if (Collision_IsHitPointToSquareRotate(pB, &pA->bottom_left) || Collision_IsHitPointToSquareRotate(pB, &pA->bottom_right) || Collision_IsHitPointToSquareRotate(pB, &pA->top_left) || Collision_IsHitPointToSquareRotate(pB, &pA->top_right))
	{
		return true;
	}

	return false;
}