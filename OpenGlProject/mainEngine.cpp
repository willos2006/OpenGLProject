#include <stdbool.h>
#include <stdio.h>
#include <process.h> 
#include <thread>
#include <glut.h>
#include "SOIL.h"
#include <windows.h>
#include "funcs.h"
using namespace std;

float moveX = 0.0;
float moveY = 0.0;

bool up = false;
bool down = false;
bool left = false;
bool right = false;

int pixelPerX = 1920 / 2;
int pixelPerY = 1080 / 2;

int objCount = 0;

GLuint textureArr[100];
GLuint texture;
GLuint escapetex;
int texcount = 0;
int comtexammount = 0;

struct Object {
	float x1; float x2; float x3; float x4;
	float y1; float y2; float y3; float y4;
	int textureNo;
	const char* name;
};

struct ComTex {
	const char* name;
	int texid;
};

struct Object objectArr[100];
Object currentObject;
ComTex comtex;
ComTex commontextureArr[100];

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

void LoadObjectAndTex(const char* filename, float x1, float x2, float x3, float x4, float y1, float y2, float y3, float y4, const char* name) {
	currentObject.x1 = x1; currentObject.x2 = x2; currentObject.x3 = x3; currentObject.x4 = x4;
	currentObject.y1 = y1; currentObject.y2 = y2; currentObject.y3 = y3; currentObject.y4 = y4;
	currentObject.name = name;
	currentObject.textureNo = texcount;
	objectArr[objCount] = currentObject;
	objCount++;
	LoadImage(filename);
}

void LoadObjectFromMem(const char* texname, float x1, float x2, float x3, float x4, float y1, float y2, float y3, float y4, const char* name) {
	currentObject.x1 = x1; currentObject.x2 = x2; currentObject.x3 = x3; currentObject.x4 = x4;
	currentObject.y1 = y1; currentObject.y2 = y2; currentObject.y3 = y3; currentObject.y4 = y4;
	currentObject.name = name;
	for (int i = 0; i < comtexammount; i++) {
		if (commontextureArr[i].name == texname) {
			currentObject.textureNo = commontextureArr[i].texid;
		}
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
	glutTimerFunc(1000 / 60, timer, 0);
}

void DeleteObject(const char* name, bool deltex) {
	int startIndex = GetObjIndexByName(name);
	if (startIndex != objCount) {
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
	}
	else {
		objectArr[startIndex] = Object{};
		textureArr[objectArr[startIndex].textureNo] = GLuint{};
		objCount--;
		texcount--;
	}
}

void MovePlayer(int) {
	if (left && moveX > -0.88) {
		moveX -= 0.03;
		glutPostRedisplay();
	}
	if (right && moveX < 0.88) {
		moveX += 0.03;
		glutPostRedisplay();
	}
	if (up && moveY < 0.68) {
		moveY += 0.03;
		glutPostRedisplay();
	}
	if (down && moveY > -0.6) {
		moveY -= 0.03;
		glutPostRedisplay();
	}
	glutTimerFunc(20, MovePlayer, 0);
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
	glBindTexture(GL_TEXTURE_2D, textureArr[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 1.0);
	glVertex2f(-1, 1);
	glTexCoord2f(1.0, 1.0);
	glVertex2f(1, 1);
	glTexCoord2f(1.0, 0.0);
	glVertex2f(1, -1);
	glTexCoord2f(0.0, 0.0);
	glVertex2f(-1, -1);
	glEnd();
	for (int i = 0; i < objCount; i++) {
		if (objectArr[i].name == "player") {
			glBindTexture(GL_TEXTURE_2D, textureArr[objectArr[i].textureNo]);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0, 1.0);
			glVertex2f(objectArr[i].x1 + moveX, objectArr[i].y1 + moveY);
			glTexCoord2f(1.0, 1.0);
			glVertex2f(objectArr[i].x2 + moveX, objectArr[i].y2 + moveY);
			glTexCoord2f(1.0, 0.0);
			glVertex2f(objectArr[i].x3 + moveX, objectArr[i].y3 + moveY);
			glTexCoord2f(0.0, 0.0);
			glVertex2f(objectArr[i].x4 + moveX, objectArr[i].y4 + moveY);
			glEnd();
		}
		else {
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
	}
	glFlush();
}

int playerIndex() {
	for (int i = 0; i < objCount; i++) {
		if (objectArr[i].name == "player") {
			return objectArr[i].textureNo;
		}
	}
}

int main(int argc, char** argv) {
	FreeConsole();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(1920, 1080);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("This 'game' has no title yet! :)");
	init();
	LoadMainScene();
	glutDisplayFunc(display);
	glutFullScreen();
	timer(0);
	MovePlayer(0);
	glutMainLoop();
	return 0;
}