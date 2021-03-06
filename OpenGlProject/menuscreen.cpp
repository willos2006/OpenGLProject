#include <stdbool.h>
#include <stdio.h>
#include <process.h> 
#include <thread>
#include <glut.h>
#include "SOIL.h"
#include <windows.h>
#include "funcs.h"

namespace menuscene {
	HANDLE input;
	bool buttonover = false;

	void MouseFunc(int button, int state, int xi, int yi) {
		float x = ((float)xi / (float)pixelPerX) - 1.0f;
		float y = (((float)yi / (float)pixelPerY) - 1.0f) * -1.0f;
		if (button == 0 && state == 0 && buttonClicked(x, y, "button")) {
			ClearForChange();
			TerminateThread(input, 0);
			changefunc = LoadMainScene;
		}
	}

	void MouseMove(int xi, int yi) {
		float x = ((float)xi / (float)pixelPerX) - 1.0f;
		float y = (((float)yi / (float)pixelPerY) - 1.0f) * -1.0f;
		if (buttonClicked(x, y, "button") && buttonover == false) {
			int exitBtnIndex = GetObjIndexByName("button");
			ButtonBigger(exitBtnIndex, 0.05f);
			glutPostRedisplay();
			buttonover = true;
		}
		if (buttonover && !buttonClicked(x, y, "button")) {
			ButtonSmaller(GetObjIndexByName("button"), 0.05f);
			buttonover = false;
		}
	}

	DWORD WINAPI userInputHandler(LPVOID lpParameter) {
		glutMouseFunc(MouseFunc);
		glutPassiveMotionFunc(MouseMove);
		Sleep(20);
		userInputHandler(NULL);
		return 0;
	}
}

void LoadMenuScene() {
	LoadIntoMem("loading.png", "loading");
	LoadObjectAndTex("backgroundmenu.png", -1, 1, 1, -1, 1, 1, -1, -1, "background", false, false);
	LoadObjectAndTex("startbtn.png", -0.725, -0.475, -0.475, -0.725, 0.15, 0.15, -0.15, -0.15, "button", false, false);
	menuscene::input = CreateThread(NULL, 0, menuscene::userInputHandler, NULL, 0, 0);
}
