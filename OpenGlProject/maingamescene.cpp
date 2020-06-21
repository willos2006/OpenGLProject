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

	void MouseFunc(int button, int state, int xi, int yi) {
		float x = ((float)xi / (float)pixelPerX) - 1.0f;
		float y = (((float)yi / (float)pixelPerY) - 1.0f) * -1.0f;
		if (button == 0 && state == 0 && buttonClicked(x, y, "exitbtn") && escapebackground) {
			exit(0);
		}
	}

	void KeyDownFuncs(unsigned char key, int x, int y) {
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
		if (key == 'w') {
			up = false;
		}
		if (key == 'a') {
			left = false;
		}
		if (key == 's') {
			down = false;
		}
		if (key == 'd') {
			right = false;
		}
	}

	DWORD WINAPI userInputHandler(LPVOID lpParameter) {
		glutMouseFunc(MouseFunc);
		glutKeyboardUpFunc(KeyUpFuncs);
		glutKeyboardFunc(KeyDownFuncs);
		Sleep(20);
		userInputHandler(NULL);
		return 0;
	}
}

void LoadMainScene() {
	glutPostOverlayRedisplay();
	LoadObjectAndTex("background.png", -1, 1, 1, -1, 1, 1, -1, -1, "background");
	LoadObjectAndTex("player1.png", -0.1, 0.1, 0.1, -0.1, 0.3, 0.3, -0.3, -0.3, "player");
	LoadIntoMem("escapebackground.png", "escapebackground");
	LoadIntoMem("exitbtn.png", "exitbtn");
	DeleteObject("loading", true);
	CreateThread(NULL, 0, maingamescene::userInputHandler, NULL, 0, 0);
}
