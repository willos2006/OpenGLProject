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
	bool isFullScreen = true;
	float velx = 0;
	float vely = 0;
	bool atEndLeft = false;
	bool atEndRight = false;
	bool atEndUp = false;
	bool atEndDown = false;

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
		if (key == 'f11') {
			if (!isFullScreen) {
				glutFullScreen();
				isFullScreen = true;
			}
			else {
				glutPositionWindow(0, 0);
				ButtonSmaller(GetObjIndexByName("exitbtn"), 0.05f);
				escapebuttonover = false;
				glutReshapeWindow(960, 540);
				isFullScreen = false;
			}
		}
		if (key == 'p') {
			printf("Player Positions: x1: %f, x2: %f, x3: %f, x4: %f \n                  y1: %f, y2: %f, y3: %f, y4: %f \n\n", hitboxArr[0].x1, hitboxArr[0].x2, hitboxArr[0].x3, hitboxArr[0].x4, hitboxArr[0].y1, hitboxArr[0].y2, hitboxArr[0].y3, hitboxArr[0].y4);
		}
		if (key == 27) {
			if (escapebackground == false) {
				LoadObjectFromMem("escapebackground", -1, 1, 1, -1, 1, 1, -1, -1, "escapebackground", false, false);
				LoadObjectFromMem("exitbtn", -0.125, 0.125, 0.125, -0.125, 0.15, 0.15, -0.15, -0.15, "exitbtn", false, false);
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

	float xOffset = 0.01;
	float yOffset = 0.01;

	bool leftcol() {
		int playerind;
		int occ = 0;
		for (int i = 0; i < hitboxcount; i++) {
			if (hitboxArr[i].name == "player") {
				playerind = i;
				break;
			}
		}
		for (int i = 0; i < hitboxcount; i++) {
			if (hitboxArr[i].isSolid && hitboxArr[i].name != "player") {
				if (hitboxArr[playerind].x1 <= hitboxArr[i].x2 + xOffset && hitboxArr[playerind].x1 >= hitboxArr[i].x2 - xOffset && hitboxArr[playerind].y3 >= hitboxArr[i].y1 - yOffset && hitboxArr[playerind].y3 <= hitboxArr[i].y1 + yOffset) { //top of object
					occ = 0;
				}
				else if (hitboxArr[playerind].x1 <= hitboxArr[i].x2 + xOffset && hitboxArr[playerind].x1 >= hitboxArr[i].x2 - xOffset && hitboxArr[playerind].y1 >= hitboxArr[i].y3 - yOffset && hitboxArr[playerind].y1 <= hitboxArr[i].y3 + yOffset) { //top of object
					occ = 0;
				}
				else if ((hitboxArr[playerind].x1 <= hitboxArr[i].x2 && hitboxArr[playerind].x1 >= (hitboxArr[i].x2 - xOffset)) && ((hitboxArr[playerind].y1 <= hitboxArr[i].y1 && hitboxArr[playerind].y1 >= hitboxArr[i].y3) || (hitboxArr[playerind].y3 <= hitboxArr[i].y1 && hitboxArr[playerind].y3 >= hitboxArr[i].y3))) {
					occ++;
				}
				else if ((hitboxArr[playerind].x1 > hitboxArr[i].x2 && hitboxArr[playerind].x1 < (hitboxArr[i].x2 - xOffset)) && ((hitboxArr[playerind].y1 <= hitboxArr[i].y1 && hitboxArr[playerind].y1 >= hitboxArr[i].y3) || (hitboxArr[playerind].y3 <= hitboxArr[i].y1 && hitboxArr[playerind].y3 >= hitboxArr[i].y3))) {
					occ++;
				}
				else if ((hitboxArr[playerind].x1 <= hitboxArr[i].x2 && hitboxArr[playerind].x1 >= (hitboxArr[i].x2 - xOffset)) && ((hitboxArr[playerind].y1 > hitboxArr[i].y1 && hitboxArr[playerind].y3 < hitboxArr[i].y3) || (hitboxArr[playerind].y3 <= hitboxArr[i].y1 && hitboxArr[playerind].y3 >= hitboxArr[i].y3))) {
					occ++;
				}
				else if ((hitboxArr[playerind].x1 > hitboxArr[i].x2 && hitboxArr[playerind].x1 < (hitboxArr[i].x2 - xOffset)) && ((hitboxArr[playerind].y1 > hitboxArr[i].y1 && hitboxArr[playerind].y3 < hitboxArr[i].y3) || (hitboxArr[playerind].y3 <= hitboxArr[i].y1 && hitboxArr[playerind].y3 >= hitboxArr[i].y3))) {
					occ++;
				}
			}
		}
		if (occ != 0) {
			return true;
		}
		else {
			return false;
		}
	}

	bool rightcol() {
		int playerind;
		int occ = 0;
		for (int i = 0; i < hitboxcount; i++) {
			if (hitboxArr[i].name == "player") {
				playerind = i;
				break;
			}
		}
		for (int i = 0; i < hitboxcount; i++) {
			if (hitboxArr[i].isSolid && hitboxArr[i].name != "player") {
				if (hitboxArr[playerind].x2 <= hitboxArr[i].x1 + xOffset && hitboxArr[playerind].x2 >= hitboxArr[i].x1 - xOffset && hitboxArr[playerind].y3 >= hitboxArr[i].y1 - yOffset && hitboxArr[playerind].y3 <= hitboxArr[i].y1 + yOffset) { //top of object
					occ = 0;
				}
				else if (hitboxArr[playerind].x2 <= hitboxArr[i].x1 + xOffset && hitboxArr[playerind].x2 >= hitboxArr[i].x1 - xOffset && hitboxArr[playerind].y1 >= hitboxArr[i].y3 - yOffset && hitboxArr[playerind].y1 <= hitboxArr[i].y3 + yOffset) { //top of object
					occ = 0;
				}
				else if ((hitboxArr[playerind].x2 <= (hitboxArr[i].x1 + xOffset) && hitboxArr[playerind].x2 >= hitboxArr[i].x1) && ((hitboxArr[playerind].y1 <= hitboxArr[i].y1 && hitboxArr[playerind].y1 >= hitboxArr[i].y3) || (hitboxArr[playerind].y3 <= hitboxArr[i].y1 && hitboxArr[playerind].y3 >= hitboxArr[i].y3))) {
					occ++;
				}
				else if ((hitboxArr[playerind].x2 > (hitboxArr[i].x1 + xOffset) && hitboxArr[playerind].x2 < hitboxArr[i].x1) && ((hitboxArr[playerind].y1 <= hitboxArr[i].y1 && hitboxArr[playerind].y1 >= hitboxArr[i].y3) || (hitboxArr[playerind].y3 <= hitboxArr[i].y1 && hitboxArr[playerind].y3 >= hitboxArr[i].y3))) {
					occ++;
				}
				else if ((hitboxArr[playerind].x2 <= (hitboxArr[i].x1 + xOffset) && hitboxArr[playerind].x2 >= hitboxArr[i].x1) && ((hitboxArr[playerind].y1 > hitboxArr[i].y1 && hitboxArr[playerind].y3 < hitboxArr[i].y3) || (hitboxArr[playerind].y3 > hitboxArr[i].y1 && hitboxArr[playerind].y3 < hitboxArr[i].y3))) {
					occ++;
				}
				else if ((hitboxArr[playerind].x2 > (hitboxArr[i].x1 + xOffset) && hitboxArr[playerind].x2 < hitboxArr[i].x1) && ((hitboxArr[playerind].y1 > hitboxArr[i].y1 && hitboxArr[playerind].y3 < hitboxArr[i].y3) || (hitboxArr[playerind].y3 > hitboxArr[i].y1 && hitboxArr[playerind].y3 < hitboxArr[i].y3))) {
					occ++;
				}
			}
		}
		if (occ != 0) {
			return true;
			printf("%i", occ);
		}
		else {
			return false;
		}
	}

	bool upcol() {
		int playerind;
		int occ = 0;
		for (int i = 0; i < hitboxcount; i++) {
			if (hitboxArr[i].name == "player") {
				playerind = i;
				break;
			}
		}
		for (int i = 0; i < hitboxcount; i++) {
			if (hitboxArr[i].isSolid && hitboxArr[i].name != "player") {
				if (hitboxArr[playerind].x2 >= hitboxArr[i].x4 - xOffset && hitboxArr[playerind].x2 <= hitboxArr[i].x4 + xOffset && hitboxArr[playerind].y1 >= hitboxArr[i].y3 - yOffset && hitboxArr[playerind].y1 <= hitboxArr[i].y3 + yOffset) {
					occ = 0;
				}
				else if (hitboxArr[playerind].x1 >= hitboxArr[i].x3 - xOffset && hitboxArr[playerind].x1 <= hitboxArr[i].x3 + xOffset && hitboxArr[playerind].y1 >= hitboxArr[i].y3 - yOffset && hitboxArr[playerind].y1 <= hitboxArr[i].y3 + yOffset) {
					occ = 0;
				}
				else if ((hitboxArr[playerind].x1 > hitboxArr[i].x1 && hitboxArr[playerind].x2 < hitboxArr[i].x2) && (hitboxArr[playerind].y1 < hitboxArr[i].y3 + yOffset && hitboxArr[playerind].y1 > hitboxArr[i].y3 - yOffset)) {
					occ++;
				}
				else if ((hitboxArr[playerind].x1 > hitboxArr[i].x1 && hitboxArr[playerind].x1 < hitboxArr[i].x2) && (hitboxArr[playerind].y1 < hitboxArr[i].y3 + yOffset && hitboxArr[playerind].y1 > hitboxArr[i].y3 - yOffset)) {
					occ++;
				}
				else if ((hitboxArr[playerind].x2 > hitboxArr[i].x1 && hitboxArr[playerind].x2 < hitboxArr[i].x2) && (hitboxArr[playerind].y1 < hitboxArr[i].y3 + yOffset && hitboxArr[playerind].y1 > hitboxArr[i].y3 - yOffset)) {
					occ++;
				}
			}
		}
		if (occ != 0) {
			return true;
			printf("%i", occ);
		}
		else {
			return false;
		}
	}

	bool downcol() {
		int playerind;
		int occ = 0;
		for (int i = 0; i < hitboxcount; i++) {
			if (hitboxArr[i].name == "player") {
				playerind = i;
				break;
			}
		}
		for (int i = 0; i < hitboxcount; i++) {
			if (hitboxArr[i].isSolid && hitboxArr[i].name != "player") {
				if (hitboxArr[playerind].x2 >= hitboxArr[i].x4 - xOffset && hitboxArr[playerind].x2 <= hitboxArr[i].x4 + xOffset && hitboxArr[playerind].y3 >= hitboxArr[i].y1 - yOffset && hitboxArr[playerind].y3 <= hitboxArr[i].y1 + yOffset) {
					occ = 0;
				}
				else if (hitboxArr[playerind].x1 >= hitboxArr[i].x3 - xOffset && hitboxArr[playerind].x1 <= hitboxArr[i].x3 + xOffset && hitboxArr[playerind].y3 >= hitboxArr[i].y1 - yOffset && hitboxArr[playerind].y3 <= hitboxArr[i].y1 + yOffset) {
					occ = 0;
				}
				else if ((hitboxArr[playerind].x1 > hitboxArr[i].x1 && hitboxArr[playerind].x2 < hitboxArr[i].x2) && (hitboxArr[playerind].y3 < hitboxArr[i].y1 + yOffset && hitboxArr[playerind].y3 > hitboxArr[i].y1 - yOffset)) {
					occ++;
				}
				else if ((hitboxArr[playerind].x1 > hitboxArr[i].x1 && hitboxArr[playerind].x1 < hitboxArr[i].x2) && (hitboxArr[playerind].y3 < hitboxArr[i].y1 + yOffset && hitboxArr[playerind].y3 > hitboxArr[i].y1 - yOffset)) {
					occ++;
				}
				else if ((hitboxArr[playerind].x2 > hitboxArr[i].x1 && hitboxArr[playerind].x2 < hitboxArr[i].x2) && (hitboxArr[playerind].y3 < hitboxArr[i].y1 + yOffset && hitboxArr[playerind].y3 > hitboxArr[i].y1 - yOffset)) {
					occ++;
				}
			}
		}
		if (occ != 0) {
			return true;
			printf("%i", occ);
		}
		else {
			return false;
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
		if (key == 'c') {
			printf("%d", leftcol());
		}
		glutPostRedisplay();
	}

	void SpecialFunc(int key, int x, int y) {
		if (key == GLUT_KEY_F11) {
			if (!isFullScreen) {
				glutFullScreen();
				pixelPerX = 1920 / 2;
				pixelPerY = 1080 / 2;
				isFullScreen = true;
			}
			else {
				glutPositionWindow(0, 0);
				glutReshapeWindow(960, 540);
				pixelPerX = 960 / 2;
				pixelPerY = 540 / 2;
				isFullScreen = false;
			}
		}
	}

	DWORD WINAPI userInputHandler(LPVOID lpParameter) {
		glutMouseFunc(MouseFunc);
		glutPassiveMotionFunc(MouseMove);
		glutKeyboardUpFunc(KeyUpFuncs);
		glutKeyboardFunc(KeyDownFuncs);
		glutSpecialFunc(SpecialFunc);
		Sleep(20);
		userInputHandler(NULL);
		return 0;
	}

	//test to see if i can move background...
	DWORD WINAPI movingAnims(LPVOID lpParameter) {
		int backgroundInd = GetObjIndexByName("background");
		int playerInd = GetObjIndexByName("player");
		if (objectArr[backgroundInd].x1 <= -1.0f - 0.02 && left && ((objectArr[playerInd].x1 + objectArr[playerInd].x2) / 2 > -0.02 && (objectArr[playerInd].x1 + objectArr[playerInd].x2) / 2 < 0.02)) {
			atEndLeft = false;
			objectArr[backgroundInd].x1 += velx;
			objectArr[backgroundInd].x2 += velx;
			objectArr[backgroundInd].x3 += velx;
			objectArr[backgroundInd].x4 += velx;
			for (int i = 0; i < objCount; i++) {
				if (objectArr[i].name != "player" && objectArr[i].name != "background" && objectArr[i].isSolid) {
					objectArr[i].x1 += velx;
					objectArr[i].x2 += velx;
					objectArr[i].x3 += velx;
					objectArr[i].x4 += velx;
					for (int x = 0; x < hitboxcount; x++) {
						if (hitboxArr[x].name == objectArr[i].name) {
							hitboxArr[x].x1 += velx;
							hitboxArr[x].x2 += velx;
							hitboxArr[x].x3 += velx;
							hitboxArr[x].x4 += velx;
							break;
						}
					}
				}
			}
		}
		else if (left) {
			atEndLeft = true;
		}
		if (objectArr[backgroundInd].x2 >= 1.0f + 0.02 && right && ((objectArr[playerInd].x1 + objectArr[playerInd].x2) / 2 > -0.02 && (objectArr[playerInd].x1 + objectArr[playerInd].x2) / 2 < 0.02)) {
			atEndRight = false;
			objectArr[backgroundInd].x1 += velx;
			objectArr[backgroundInd].x2 += velx;
			objectArr[backgroundInd].x3 += velx;
			objectArr[backgroundInd].x4 += velx;
			for (int i = 0; i < objCount; i++) {
				if (objectArr[i].name != "player" && objectArr[i].name != "background" && objectArr[i].isSolid) {
					objectArr[i].x1 += velx;
					objectArr[i].x2 += velx;
					objectArr[i].x3 += velx;
					objectArr[i].x4 += velx;
					for (int x = 0; x < hitboxcount; x++) {
						if (hitboxArr[x].name == objectArr[i].name) {
							hitboxArr[x].x1 += velx;
							hitboxArr[x].x2 += velx;
							hitboxArr[x].x3 += velx;
							hitboxArr[x].x4 += velx;
							break;
						}
					}
				}
			}
		}
		else if (right) {
			atEndRight = true;
		}
		if (objectArr[backgroundInd].y1 >= 1.0f + 0.02 && up && ((objectArr[playerInd].y1 + objectArr[playerInd].y3) / 2 > -0.02 && (objectArr[playerInd].y1 + objectArr[playerInd].y3) / 2 < 0.02)) {
			atEndUp = false;
			objectArr[backgroundInd].y1 += vely;
			objectArr[backgroundInd].y2 += vely;
			objectArr[backgroundInd].y3 += vely;
			objectArr[backgroundInd].y4 += vely;
			for (int i = 0; i < objCount; i++) {
				if (objectArr[i].name != "player" && objectArr[i].name != "background" && objectArr[i].isSolid) {
					objectArr[i].y1 += vely;
					objectArr[i].y2 += vely;
					objectArr[i].y3 += vely;
					objectArr[i].y4 += vely;
					for (int x = 0; x < hitboxcount; x++) {
						if (hitboxArr[x].name == objectArr[i].name) {
							hitboxArr[x].y1 += vely;
							hitboxArr[x].y2 += vely;
							hitboxArr[x].y3 += vely;
							hitboxArr[x].y4 += vely;
							break;
						}
					}
				}
			}
		}
		else if (up) {
			atEndUp = true;
		}
		if (objectArr[backgroundInd].y3 <= -1.0f - 0.02 && down && ((objectArr[playerInd].y1 + objectArr[playerInd].y3) / 2 > -0.02 && (objectArr[playerInd].y1 + objectArr[playerInd].y3) / 2 < 0.02)) {
			atEndDown = false;
			objectArr[backgroundInd].y1 += vely;
			objectArr[backgroundInd].y2 += vely;
			objectArr[backgroundInd].y3 += vely;
			objectArr[backgroundInd].y4 += vely;
			for (int i = 0; i < objCount; i++) {
				if (objectArr[i].name != "player" && objectArr[i].name != "background" && objectArr[i].isSolid) {
					objectArr[i].y1 += vely;
					objectArr[i].y2 += vely;
					objectArr[i].y3 += vely;
					objectArr[i].y4 += vely;
					for (int x = 0; x < hitboxcount; x++) {
						if (hitboxArr[x].name == objectArr[i].name) {
							hitboxArr[x].y1 += vely;
							hitboxArr[x].y2 += vely;
							hitboxArr[x].y3 += vely;
							hitboxArr[x].y4 += vely;
							break;
						}
					}
				}
			}
		}
		else if (down) {
			atEndDown = true;
		}
		Sleep(20);
		movingAnims(NULL);
		return 0;
	}
	
	DWORD WINAPI anims(LPVOID lpParameter) {
		int playerIndex = GetObjIndexByName("player");
		if (right && up || right && down) {
			if (turn == 0) {
				objectArr[playerIndex].textureNo = GetTexIndex("right1");
				turn = 1;
			}
			else {
				objectArr[playerIndex].textureNo = GetTexIndex("right2");
				turn = 0;
			}
		}
		else if (left && up || left && down) {

			if (turn == 0) {
				objectArr[playerIndex].textureNo = GetTexIndex("left1");
				turn = 1;
			}
			else {
				objectArr[playerIndex].textureNo = GetTexIndex("left2");
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

	bool CheckIfCollideStop() {
		int collideNo = 0;
		for (int i = 0; i < hitboxcount; i++) {
			if (hitboxArr[i].isSolid && hitboxArr[i].name != "player") {
				if (IsColliding("player", hitboxArr[i].name)) {
					collideNo++;
				}
			}
		}
		if (collideNo > 0) {
			return true;
		}
		else {
			return false;
		}
	}

	void MovePlayer(int) {
		if (canmove) {
			bool velydone = false;
			bool velxdone = false;
			int playerIndex = GetObjIndexByName("player");
			int playerIndexHitbox;
			for (int i = 0; i < hitboxcount; i++) {
				if (hitboxArr[i].name == "player") {
					playerIndexHitbox = i;
				}
			}
			if (left && objectArr[playerIndex].x1 > -1.0f && !leftcol()) {
				if (atEndLeft) {
					objectArr[playerIndex].x1 -= 0.02;
					objectArr[playerIndex].x2 -= 0.02;
					objectArr[playerIndex].x3 -= 0.02;
					objectArr[playerIndex].x4 -= 0.02;
					hitboxArr[playerIndexHitbox].x1 -= 0.02;
					hitboxArr[playerIndexHitbox].x2 -= 0.02;
					hitboxArr[playerIndexHitbox].x3 -= 0.02;
					hitboxArr[playerIndexHitbox].x4 -= 0.02;
				}
				velx = 0.02;
			}
			else if (right && objectArr[playerIndex].x2 < 1.0f && !rightcol()) {
				if (atEndRight) {
					objectArr[playerIndex].x1 += 0.02;
					objectArr[playerIndex].x2 += 0.02;
					objectArr[playerIndex].x3 += 0.02;
					objectArr[playerIndex].x4 += 0.02;
					hitboxArr[playerIndexHitbox].x1 += 0.02;
					hitboxArr[playerIndexHitbox].x2 += 0.02;
					hitboxArr[playerIndexHitbox].x3 += 0.02;
					hitboxArr[playerIndexHitbox].x4 += 0.02;
				}
				velx = -0.02;
			}
			else {
				velx = 0;
			}
			if (up && objectArr[playerIndex].y1 < 1.0f && !upcol()) {
				if (atEndUp) {
					objectArr[playerIndex].y1 += 0.02;
					objectArr[playerIndex].y2 += 0.02;
					objectArr[playerIndex].y3 += 0.02;
					objectArr[playerIndex].y4 += 0.02;
					hitboxArr[playerIndexHitbox].y1 += 0.02;
					hitboxArr[playerIndexHitbox].y2 += 0.02;
					hitboxArr[playerIndexHitbox].y3 += 0.02;
					hitboxArr[playerIndexHitbox].y4 += 0.02;
				}
				vely = -0.02;
			}
			else if (down && objectArr[playerIndex].y3 > -1.0f && !downcol()) {
				if (atEndDown) {
					objectArr[playerIndex].y1 -= 0.02;
					objectArr[playerIndex].y2 -= 0.02;
					objectArr[playerIndex].y3 -= 0.02;
					objectArr[playerIndex].y4 -= 0.02;
					hitboxArr[playerIndexHitbox].y1 -= 0.02;
					hitboxArr[playerIndexHitbox].y2 -= 0.02;
					hitboxArr[playerIndexHitbox].y3 -= 0.02;
					hitboxArr[playerIndexHitbox].y4 -= 0.02;
				}
				vely = 0.02;
			}
			else {
				vely = 0;
			}
		}
		glutTimerFunc(20, MovePlayer, 0);
	}
}

void LoadMainScene() {
	glutPostOverlayRedisplay();
	LoadObjectAndTex("background2.png", -2, 2, 2, -2, 2, 2, -2, -2, "background", false);
	LoadObjectAndTex("player1.png", -0.085, 0.085, 0.085, -0.085, 0.255, 0.255, -0.255, -0.255, "player", true, true);
	LoadObjectAndTex("button.png", 0.175, 0.425, 0.425, 0.175, 0.15, 0.15, -0.7, -0.7, "testcol", true, true);
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
	if (isLoadScreen) {
		DeleteObject("loading", true);
		isLoadScreen = false;
	}
	CreateThread(NULL, 0, maingamescene::userInputHandler, NULL, 0, 0);
	CreateThread(NULL, 0, maingamescene::anims, NULL, 0, 0);
	maingamescene::MovePlayer(0);
	CreateThread(NULL, 0, maingamescene::movingAnims, NULL, 0, 0);
}