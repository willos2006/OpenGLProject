#pragma once

void ClearForChange();
void LoadImage(const char* filename);
void LoadIntoMem(const char* filename, const char* texname);
void LoadObjectAndTex(const char* filename, float x1, float x2, float x3, float x4, float y1, float y2, float y3, float y4, const char* name);
void LoadMainScene();
void LoadMenuScene();
void LoadObjectFromMem(const char* texname, float x1, float x2, float x3, float x4, float y1, float y2, float y3, float y4, const char* name);
void DeleteObject(const char* name, bool deltex);
bool buttonClicked(float x, float y, const char* btnName);
extern bool up;
extern bool down;
extern bool left;
extern bool right;
extern int pixelPerX;
extern int pixelPerY;
extern void(*changefunc)(void);