#include <stdbool.h>
#include <stdio.h>
#include <process.h> 
#include <thread>
#include <glut.h>
#include "SOIL.h"
#include <windows.h>
#include "funcs.h"

namespace maingamescene {

	bool escapebackground = false;
	bool escapebuttonover = false;
	int turn = 0;

	void MouseFunc(int button, int state, int xi, int yi) {
		float x = ((float)xi / (float)pixelPerX) - 1.0f;
		float y = (((float)yi / (float)pixelPerY) - 1.0f) * -1.0f;
		if (button == 0 && state == 0 && buttonClicked(x, y, "exitbtn") && escapebackground) {
			exit(0);
		}
	}

	void MouseMove(int xi, int yi) {
		float x = ((float)xi / (float)pixelPerX) - 1.0f;
		float y = (((float)yi / (float)pixelPerY) - 1.0f) * -1.0f;
		if (buttonClicked(x, y, "exitbtn") && escapebuttonover == false) {
			int exitBtnIndex = GetObjIndexByName("exitbtn");
			ButtonBigger(exitBtnIndex, 0.05f);
			glutPostRedisplay();
			escapebuttonover = true;
		}
		if(escapebuttonover && !buttonClicked(x, y, "exitbtn")){
			ButtonSmaller(GetObjIndexByName("exitbtn"), 0.05f);
			escapebuttonover = false;
		}
	}

	void KeyDownFuncs(unsigned char key, int x, int y) {
		if (!escapebackground) {
			if (key == 'w') {
				up = true;
			}
			if (key == 'a') {
				left = true;
			}
			if (key == 's') {
				down = true;
			}
			if (key == 'd') {
				right = true;
			}
		}
		if (key == 27) {
			if (escapebackground == false) {
				LoadObjectFromMem("escapebackground", -1, 1, 1, -1, 1, 1, -1, -1, "escapebackground");
				LoadObjectFromMem("exitbtn", -0.125, 0.125, 0.125, -0.125, 0.15, 0.15, -0.15, -0.15, "exitbtn");
				escapebackground = true;
				glutPostRedisplay();
			}
			else {
				DeleteObject("escapebackground", false);
				DeleteObject("exitbtn", false);
				escapebackground = false;
				glutPostRedisplay();
			}
		}
	}

	void KeyUpFuncs(unsigned char key, int x, int y) {
		int playerIndex = GetObjIndexByName("player");
		if (key == 'w') {
			up = false;
			objectArr[playerIndex].textureNo = GetTexIndex("player");
			turn = 0;
		}
		if (key == 'a') {
			left = false;
			objectArr[playerIndex].textureNo = GetTexIndex("player");
			turn = 0;
		}
		if (key == 's') {
			down = false;
			objectArr[playerIndex].textureNo = GetTexIndex("player");
			turn = 0;
		}
		if (key == 'd') {
			right = false;
			objectArr[playerIndex].textureNo = GetTexIndex("player");
			turn = 0;
		}
		glutPostRedisplay();
	}

	DWORD WINAPI userInputHandler(LPVOID lpParameter) {
		glutMouseFunc(MouseFunc);
		glutPassiveMotionFunc(MouseMove);
		glutKeyboardUpFunc(KeyUpFuncs);
		glutKeyboardFunc(KeyDownFuncs);
		Sleep(20);
		userInputHandler(NULL);
		return 0;
	}
	
	DWORD WINAPI anims(LPVOID lpParameter) {
		int playerIndex = GetObjIndexByName("player");
		if (right && up || left && up) {
			if (turn == 0) {
				objectArr[playerIndex].textureNo = GetTexIndex("up1");
				turn = 1;
			}
			else {
				objectArr[playerIndex].textureNo = GetTexIndex("up2");
				turn = 0;
			}
		}
		else if (left && down || right && down) {

			if (turn == 0) {
				objectArr[playerIndex].textureNo = GetTexIndex("down1");
				turn = 1;
			}
			else {
				objectArr[playerIndex].textureNo = GetTexIndex("down2");
				turn = 0;
			}
		}
		else if (left) {
			if (turn == 0) {
				objectArr[playerIndex].textureNo = GetTexIndex("left1");
				turn = 1;
			}
			else {
				objectArr[playerIndex].textureNo = GetTexIndex("left2");
				turn = 0;
			}
		}
		else if (right) {
			if (turn == 0) {
				objectArr[playerIndex].textureNo = GetTexIndex("right1");
				turn = 1;
			}
			else {
				objectArr[playerIndex].textureNo = GetTexIndex("right2");
				turn = 0;
			}
		}
		else if (up) {
			if (turn == 0) {
				objectArr[playerIndex].textureNo = GetTexIndex("up1");
				turn = 1;
			}
			else {
				objectArr[playerIndex].textureNo = GetTexIndex("up2");
				turn = 0;
			}
		}
		else if (down) {
			if (turn == 0) {
				objectArr[playerIndex].textureNo = GetTexIndex("down1");
				turn = 1;
			}
			else {
				objectArr[playerIndex].textureNo = GetTexIndex("down2");
				turn = 0;
			}
		}
		glutPostRedisplay();
		Sleep(200);
		anims(NULL);
		return 0;
	}
}

void LoadMainScene() {
	glutPostOverlayRedisplay();
	LoadObjectAndTex("background.png", -1, 1, 1, -1, 1, 1, -1, -1, "background");
	LoadObjectAndTex("player1.png", -0.085, 0.085, 0.085, -0.085, 0.255, 0.255, -0.255, -0.255, "player");
	LoadIntoMem("escapebackground.png", "escapebackground");
	LoadIntoMem("player1.png", "player");
	LoadIntoMem("exitbtn.png", "exitbtn");
	LoadIntoMem("up1.png", "up1");
	LoadIntoMem("up2.png", "up2");
	LoadIntoMem("down1.png", "down1");
	LoadIntoMem("down2.png", "down2");
	LoadIntoMem("left1.png", "left1");
	LoadIntoMem("left2.png", "left2"); 
	LoadIntoMem("right1.png", "right1"); 
	LoadIntoMem("right2.png", "right2"); 
	DeleteObject("loading", true);
	CreateThread(NULL, 0, maingamescene::userInputHandler, NULL, 0, 0);
	CreateThread(NULL, 0, maingamescene::anims, NULL, 0, 0);
}
