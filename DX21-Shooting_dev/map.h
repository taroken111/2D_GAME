#ifndef MAP_H_
#define MAP_H_

#include "collision.h"

#define MAX_OBJECT     (5)
//#define MAX_OBJECT     (20)

typedef struct
{
	float PosX;
	float PosY;
	float width;
	float height;
}MAP;

void Map_Init(void);
void Map_Draw(void);
MAP* Map_GetMap(void);
Square* Map_GetObjectCollision(int index);

#endif
