#include <stdbool.h>
#include <stdio.h>
#include <process.h> 
#include <thread>
#include <glut.h>
#include "SOIL.h"
#include <windows.h>
#include "funcs.h"

float moveX = 0.0;
float moveY = 0.0;

bool up;
bool down;
bool left;
bool right;

bool canmove = true;

int pixelPerX = 1920 / 2;
int pixelPerY = 1080 / 2;

int objCount = 0;

GLuint textureArr[100];
GLuint texture;
GLuint escapetex;
int texcount = 0;
int comtexammount = 0;

struct ComTex {
	const char* name;
	int texid;
};

struct HitBox {
	float x1; float x2; float x3; float x4;
	float y1; float y2; float y3; float y4;
	const char* name;
};

struct HitBox hitboxArr[100];

struct Object objectArr[100];
Object currentObject;
int hitboxcount = 0;
struct ComTex comtex;
ComTex commontextureArr[100];
bool changeWaiting = false;
void(*changefunc)(void);

void init(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glLoadIdentity();
}

int GetObjIndexByName(const char* name) {
	for (int i = 0; i < objCount; i++) {
		if (objectArr[i].name == name) {
			return i;
		}
	}
}

int GetTexIndex(const char* name) {
	for (int i = 0; i < comtexammount; i++) {
		if (commontextureArr[i].name == name) {
			return commontextureArr[i].texid;
		}
	}
}

void LoadImage(const char* filename) {
	texture = SOIL_load_OGL_texture(filename, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	textureArr[texcount] = texture;
	texcount++;
}

void LoadObjectAndTex(const char* filename, float x1, float x2, float x3, float x4, float y1, float y2, float y3, float y4, const char* name, bool hitbox, float size) {
	currentObject.x1 = x1; currentObject.x2 = x2; currentObject.x3 = x3; currentObject.x4 = x4;
	currentObject.y1 = y1; currentObject.y2 = y2; currentObject.y3 = y3; currentObject.y4 = y4;
	currentObject.name = name;
	currentObject.textureNo = texcount;
	objectArr[objCount] = currentObject;
	if (hitbox) {
		hitboxArr[hitboxcount].x1 = x1 + size; hitboxArr[hitboxcount].x2 = x2 + size; hitboxArr[hitboxcount].x3 = x3 + size; hitboxArr[hitboxcount].x4 = x4 + size;
		hitboxArr[hitboxcount].y1 = y1 + size; hitboxArr[hitboxcount].y2 = y2 + size; hitboxArr[hitboxcount].y3 = y3 + size; hitboxArr[hitboxcount].y4 = y4 + size;
		hitboxArr[hitboxcount].name = name;
		hitboxcount++;
	}
	objCount++;
	LoadImage(filename);
}

void LoadObjectFromMem(const char* texname, float x1, float x2, float x3, float x4, float y1, float y2, float y3, float y4, const char* name, bool hitbox, float size) {
	currentObject.x1 = x1; currentObject.x2 = x2; currentObject.x3 = x3; currentObject.x4 = x4;
	currentObject.y1 = y1; currentObject.y2 = y2; currentObject.y3 = y3; currentObject.y4 = y4;
	currentObject.name = name;
	for (int i = 0; i < comtexammount; i++) {
		if (commontextureArr[i].name == texname) {
			currentObject.textureNo = commontextureArr[i].texid;
		}
	}
	if (hitbox) {
		hitboxArr[hitboxcount].x1 = x1 + size; hitboxArr[hitboxcount].x2 = x2 + size; hitboxArr[hitboxcount].x3 = x3 + size; hitboxArr[hitboxcount].x4 = x4 + size;
		hitboxArr[hitboxcount].y1 = y1 + size; hitboxArr[hitboxcount].y2 = y2 + size; hitboxArr[hitboxcount].y3 = y3 + size; hitboxArr[hitboxcount].y4 = y4 + size;
		hitboxArr[hitboxcount].name = name;
		hitboxcount++;
	}
	objectArr[objCount] = currentObject;
	objCount++;
}

void LoadIntoMem(const char* filename, const char* texname) {
	commontextureArr[comtexammount].name = texname;
	commontextureArr[comtexammount].texid = texcount;
	comtexammount++;
	LoadImage(filename);
}

void timer(int) {
	glutPostRedisplay();
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
	glutTimerFunc(1000 / 60, timer, 0);
}

void DeleteObject(const char* name, bool deltex) {
	int startIndex = GetObjIndexByName(name);
	texcount -= 1;
	objCount -= 1;
	if (deltex) {
		for (int i = objectArr[startIndex].textureNo; i <= texcount; i++) {
			textureArr[i] = textureArr[i + 1];
			for (int x = 0; x < comtexammount; x++) {
				if (commontextureArr[x].texid == i) {
					commontextureArr[x].texid--;
				}
			}
		}
	}
	for (int i = startIndex; i <= objCount; i++) {
		objectArr[i] = objectArr[i + 1];
		objectArr[i].textureNo -= 1;
	}
	int hitboxNo;
	int ishitbox = false;
	for (int i = 0; i < hitboxcount; i++) {
		if (hitboxArr[i].name == name) {
			hitboxNo = i;
			break;
			hitboxcount--;
			ishitbox = true;
		}
	}
	if (ishitbox) {
		for (int i = hitboxNo; i < hitboxcount; i++) {
			hitboxArr[i] = hitboxArr[i + 1];
		}
	}
}

bool buttonClicked(float x, float y, const char* btnName) {
	Object btnStruct = objectArr[GetObjIndexByName(btnName)];
	if (x > btnStruct.x1 && x < btnStruct.x2 && y < btnStruct.y1 && y > btnStruct.y3) {
		return true;
	}
	else {
		return false;
	}
}

void LoadEscapeTex() {
	texture = SOIL_load_OGL_texture("escapebackground.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	escapetex = texture;
}

void display(void) {
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < objCount; i++) {
		glBindTexture(GL_TEXTURE_2D, textureArr[objectArr[i].textureNo]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0);
		glVertex2f(objectArr[i].x1, objectArr[i].y1);
		glTexCoord2f(1.0, 1.0);
		glVertex2f(objectArr[i].x2, objectArr[i].y2);
		glTexCoord2f(1.0, 0.0);
		glVertex2f(objectArr[i].x3, objectArr[i].y3);
		glTexCoord2f(0.0, 0.0);
		glVertex2f(objectArr[i].x4, objectArr[i].y4);
		glEnd();
	}
	glFlush();
	glutSwapBuffers();
	if (changeWaiting) {
		changeWaiting = false;
		changefunc();
	}
}

int playerIndex() {
	for (int i = 0; i < objCount; i++) {
		if (objectArr[i].name == "player") {
			return objectArr[i].textureNo;
		}
	}
}

void ButtonBigger(int index, float amount) {
	objectArr[index].x1 -= amount;
	objectArr[index].x2 += amount;
	objectArr[index].x3 += amount;
	objectArr[index].x4 -= amount;
	objectArr[index].y1 += amount;
	objectArr[index].y2 += amount;
	objectArr[index].y3 -= amount;
	objectArr[index].y4 -= amount;
	glutPostRedisplay();
}

void ButtonSmaller(int index, float amount) {
	objectArr[index].x1 += amount;
	objectArr[index].x2 -= amount;
	objectArr[index].x3 -= amount;
	objectArr[index].x4 += amount;
	objectArr[index].y1 -= amount;
	objectArr[index].y2 -= amount;
	objectArr[index].y3 += amount;
	objectArr[index].y4 += amount;
	glutPostRedisplay();
}


void ClearForChange() {
	int donotdel;
	for (int i = 0; i < objCount; i++) {
		objectArr[i] = Object{};
		objCount--;
	}
	for (int i = 0; i < comtexammount; i++) {
		if (commontextureArr[i].name != "loading") {
			commontextureArr[i] = ComTex{};
			comtexammount--;
		}
		else {
			donotdel = commontextureArr[i].texid;
		}
	}
	for (int i = 0; i < texcount; i++) {
		if (i != donotdel) {
			textureArr[i] = GLuint{};
			texcount--;
		}
	}
	LoadObjectFromMem("loading", -1, 1, 1, -1, 1, 1, -1, -1, "loading", false);
	glutPostRedisplay();
	changeWaiting = true;
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(1920, 1080);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("This 'game' has no title yet! :)");
	init();
	glutDisplayFunc(display);
	glutFullScreen();
	LoadMenuScene();
	timer(0);
	glutMainLoop();
	return 0;
}