#pragma once

void ClearForChange();
extern struct Object {
	float x1; float x2; float x3; float x4;
	float y1; float y2; float y3; float y4;
	int textureNo;
	const char* name;
};
void ButtonBigger(int index, float amount);
void ButtonSmaller(int index, float amount);
extern struct Object objectArr[100];
void LoadImage(const char* filename);
void LoadIntoMem(const char* filename, const char* texname);
void LoadObjectAndTex(const char* filename, float x1, float x2, float x3, float x4, float y1, float y2, float y3, float y4, const char* name, bool hitbox, float size = 0.0f);
void LoadMainScene();
void LoadMenuScene();
void LoadObjectFromMem(const char* texname, float x1, float x2, float x3, float x4, float y1, float y2, float y3, float y4, const char* name, bool hitbox, float size = 0.0f);
void DeleteObject(const char* name, bool deltex);
bool buttonClicked(float x, float y, const char* btnName);
extern bool up;
extern bool down;
extern bool left;
extern bool right;
extern int pixelPerX;
extern int pixelPerY;
extern bool canmove;
extern void(*changefunc)(void);
int GetObjIndexByName(const char* name);
int GetTexIndex(const char* name);
extern int objCount;