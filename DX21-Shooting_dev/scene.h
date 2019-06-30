#ifndef SCENE_H_
#define SCENE_H_

typedef enum SCENE_INDEX
{
	SCENE_INDEX_TITLE,
	SCENE_INDEX_GAME,
	SCENE_INDEX_RESULT,
	SCENE_INDEX_MAX
};

typedef enum FUNCTION_INDEX
{
	FUNCTION_INDEX_INIT,
	FUNCTION_INDEX_UPDATE,
	FUNCTION_INDEX_DRAW,
	FUNCTION_INDEX_UNINIT
};

void Scene_Init(SCENE_INDEX index);
void Scene_Update(void);
void Scene_Draw(void);
void Scene_Check(void);   //ñàÉtÉåÅ[ÉÄèIÇÌÇËÇ…åƒÇ‘ä÷êî
void Scene_Uninit(void);

void Scene_Change(SCENE_INDEX index);


#endif // !SCENE_H_

