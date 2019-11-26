#include <vector>
#include <iostream>
#include "Bubbles.h"
#include "Light.h"
#include "Cylinder.h"
#include <time.h>
#include <string>
#include <array>
// include header files

using namespace std;

#define WIDTH 450
#define HEIGHT 720

#define LEFTBOUND 0
#define RIGHTBOUND 20
#define UPBOUND 0
#define DOWNBOUND 32

#define PI 3.141592

// global variables
vector<Bubbles> bubbles;
vector<Bubbles> temp;
vector<Bubbles> Prediction;
Cylinder cyl1; Cylinder cyl2;
Light* light;
float s3 = sqrt(3);
float line[12];
float velocity = 0.1;
int angle = 0;
int score = 0;
int game_over = 0;
bool timer = true;
float timecount = 0;
int seccount = 0;
clock_t fps_start = clock();
clock_t fps_end;
int fps = 60;
int samecolorlength = 0;
int drop = 0;
int drop_length = 0;
float drop_velocity = velocity;
int do_prediction = 0;
int prediction_sw = 0;

void createPopupMenus();
void processMainMenu(int value);

/* void stopwatch(int Timer) {
	int newtime = (unsigned int)time(NULL);
	static int oldtime = newtime;
	if (Timer == 1) {
		newtime = (unsigned int)time(NULL);
		if (newtime != oldtime) {
			t -= 1;
			oldtime = newtime;
			if (t == 0) {
				timer = 0;
			}
		}
	}
}*/

double getRadian(int num) {
	return num * (PI / 180);
}

void draw_characters(void* font, const char* c, float x, float y, float z) {
	glRasterPos3f(x, y, z);
	for (int i = 0; i < strlen(c); i++)
		glutBitmapCharacter(font, c[i]);
}

void init() {
	light = new Light(200, 200, 200, GL_LIGHT0);
	light->setAmbient(0.3, 0.3, 0.3, 1.0);
	light->setDiffuse(0.5, 0.5, 0.5, 1.0);
	light->setSpecular(1.0, 1.0, 1.0, 1.0);

	Bubbles initialBubble;
	initialBubble.setCenter(10.0, 29.0, 0.0);
	bubbles.push_back(initialBubble);

	Bubbles nextBubble;
	temp.push_back(nextBubble);


	for (int i = 0; i < 12; i++) {
		line[i] = 7.5 + s3 * i;
	}
}

// 인접하고, 색깔이 같고, samecolor=0인 버블의 samecolor를 1로 설정하는 함수
void search(Bubbles& bub) {
	for (int i = 0; i < bubbles.size() - 1; i++) {
		if (bub.newCollisionDetection(bubbles[i]) && bub.getColor() == bubbles[i].getColor() && bubbles[i].getSamecolor() == 0) {
			bubbles[i].setSamecolor(1);
			search(bubbles[i]);
		}
	}
}

// 자신에게 체크한 뒤, 체크가 없는 인접한 버블로 가서 재탐색하는 함수
void checkSearch(Bubbles& bub) {
	if (bub.getCheck() == 0)
		bub.setCheck(1);
	for (int i = 0; i < bubbles.size(); i++) {
		if (bub.newCollisionDetection(bubbles[i]) && bubbles[i].getCheck() == 0)
			checkSearch(bubbles[i]);
	}
}

void idle() {
	// fps 설정
	fps_end = clock();
	if (fps_end - fps_start > 1000 / fps) {
		// time counting - 6초마다 타이머 리셋. seccount가 fps에 따라 1초에 30씩 증가하므로 seccount=180이면 6초임
		if (timer)
			seccount += 1;
		if (seccount == 600) {
			bubbles.back().setVelocity(velocity * sin(getRadian(angle)), velocity * -cos(getRadian(angle)), 0);
			timecount = 0;
			seccount = 0;
			prediction_sw = 0;
		}
		if (timer)
			timecount += 6.5 / 600;
		if (drop == 0) {
			if (bubbles.back().getVelocity()[0] != 0 || bubbles.back().getVelocity()[1] != 0) {
				if (temp.size() == 1) {
					temp[0].setCenter(10.0, 29.0, 0.0);
					Bubbles nextBubble;
					temp.push_back(nextBubble);
				}
			}

			// boundary check
			if (bubbles.back().getCenter()[0] + 1 >= RIGHTBOUND || bubbles.back().getCenter()[0] - 1 <= LEFTBOUND) {
				bubbles.back().setVelocity(-bubbles.back().getVelocity()[0], bubbles.back().getVelocity()[1], 0);
			}

			// collision handling
			for (int i = 0; i < bubbles.size() - 1; ++i) {
				if (bubbles.back().collisionDetection(bubbles[i])) {
					bubbles.back().setVelocity(0, 0, 0);
					// 위치 설정
					bubbles.back().decidePosition();
					// ** remove algorithm **
					bubbles.back().setSamecolor(1);
					search(bubbles.back());
					//samecolor=1인 버블의 개수를 확인
					for (int i = 0; i < bubbles.size(); i++) {
						if (bubbles[i].getSamecolor() == 1) {
							samecolorlength++;
						}
					}
					// 같은 색깔이 3개 이상 붙었을 때
					if (samecolorlength >= 3) {
						drop = 1;
						// 벡터에서 삭제
						for (int i = 0; i < bubbles.size();) {
							if (bubbles[i].getSamecolor() == 1) {
								bubbles.erase(bubbles.begin() + i);
								score += 10;
							}
							else
								i++;
						}
						// ** drop algorithm **
						// 맨 윗열 버블들을 기준으로 탐색
						for (int i = 0; i < bubbles.size(); i++) {
							if (bubbles[i].getCenter()[1] == line[0]) {
								checkSearch(bubbles[i]);
							}
						}
						// check=0인 버블의 개수를 셈
						for (int i = 0; i < bubbles.size(); i++) {
							if (bubbles[i].getCheck() == 0) {
								drop_length += 1;
								bubbles[i].setVelocity(0, drop_velocity, 0); // 드랍 속도 = 쏘는 속도의 절반
							}
						}
					}
					// 버블이 아래 경계선에 닿았으면 게임오버 - upbound check할 때는 필요없음
					if (bubbles.back().getCenter()[1] == line[11]) {
						game_over = 1;
					}
					// remove에 관련된 변수는 전부 재설정. drop에 관련된 변수는 drop할 버블이 없을 때만 재설정
					samecolorlength = 0;
					for (int i = 0; i < bubbles.size(); i++) {
						bubbles[i].setSamecolor(0);
						if (drop_length == 0) {
							bubbles[i].setCheck(0);
						}
					}
					if (drop_length == 0) {
						drop = 0;
						timer = true;
					}
					// 새로운 공 발사 준비
					bubbles.push_back(temp[0]);
					temp.erase(temp.begin());
					break;
				}
			}

			// upbound check
			if (bubbles.back().getCenter()[1] <= line[0]) {
				bubbles.back().setVelocity(0, 0, 0);
				bubbles.back().decidePosition();
				bubbles.back().setSamecolor(1);
				bubbles.back().setCheck(0);
				search(bubbles.back());
				for (int i = 0; i < bubbles.size(); i++) {
					if (bubbles[i].getSamecolor() == 1) {
						samecolorlength++;
					}
				}
				if (samecolorlength >= 3) {
					drop = 1;
					for (int i = 0; i < bubbles.size();) {
						printf("%d \n", bubbles[i].getSamecolor());
						if (bubbles[i].getSamecolor() == 1) {
							bubbles.erase(bubbles.begin() + i);
							score += 10;
						}
						else
							i++;
					}
					for (int i = 0; i < bubbles.size(); i++) {
						if (bubbles[i].getCenter()[1] == line[0]) {
							checkSearch(bubbles[i]);
						}
					}
					for (int i = 0; i < bubbles.size(); i++) {
						if (bubbles[i].getCheck() == 0) {
							drop_length += 1;
							bubbles[i].setVelocity(0, drop_velocity, 0);
						}
					}
				}
				samecolorlength = 0;
				for (int i = 0; i < bubbles.size(); i++) {
					bubbles[i].setSamecolor(0);
					if (drop_length == 0) {
						bubbles[i].setCheck(0);
					}
				}
				if (drop_length == 0) {
					drop = 0;
					timer = true;
				}
				bubbles.push_back(temp[0]);
				temp.erase(temp.begin());
			}//Prediction을 쏴야할때(대기중인 버블이 움직이지 않고, Prediction안에 버블이 있을때
			if (Prediction.size()>=1) {
				if (Prediction.back().getCenter()[1] <= line[0]) {
						Prediction.back().setVelocity(0, 0, 0);
						Prediction.back().decidePosition();
				}
				if (Prediction.back().getCenter()[0] + 1 >= RIGHTBOUND || Prediction.back().getCenter()[0] - 1 <= LEFTBOUND) {
						Prediction.back().setVelocity(-Prediction.back().getVelocity()[0], Prediction.back().getVelocity()[1], 0);
				}
				for (int i = 0; i < bubbles.size() - 1; ++i) {
					if (Prediction.back().collisionDetection(bubbles[i])) {
							Prediction.back().setVelocity(0, 0, 0);
							Prediction.back().decidePosition();
					}
				}
			}
			
		}

		// 드랍이 진행 중일 때
		else if (drop == 1 && drop_length > 0) {
			// 드랍시키는 버블이 아래 경계선에 닿으면(경계선의 y좌표가 27.3이므로 버블 중심의 y값이 26.3보다 커진다면) 삭제 및 점수 증가
			for (int i = 0; i < bubbles.size() - 1;) {
				if (bubbles[i].getCheck() == 0 && bubbles[i].getCenter()[1] > 26.3) {
					bubbles.erase(bubbles.begin() + i);
					drop_length -= 1;
					score += 30;
				}
				else {
					i++;
				}
			}
			for (int i = 0; i < bubbles.size() - 1; i++) {
				if (bubbles[i].getCheck() == 0) {
					bubbles[i].move();
				}
			}
		}

		// 드랍이 완료되었을 때 drop 관련 변수 모두 재설정
		else {
			for (int i = 0; i < bubbles.size() - 1; i++) {
				bubbles[i].setCheck(0);
			}
			drop = 0;
			timer = true;
		}
		fps_start = fps_end;
	}
	glutPostRedisplay();
}

void reset() {
	angle = 0;
	score = 0;
	game_over = 0;
	timer = true;
	timecount = 0;
	seccount = 0;
	samecolorlength = 0;

	bubbles.clear();
	temp.clear();

	Bubbles initialBubble;
	initialBubble.setCenter(10.0, 29.0, 0.0);
	bubbles.push_back(initialBubble);

	Bubbles nextBubble;
	temp.push_back(nextBubble);
}

void keyboard(unsigned char key, int x, int y) {
	// Spacebar - ball shooting
	if (key == 32 && game_over == 0) {
		if (bubbles.back().getVelocity()[0] == 0 && bubbles.back().getVelocity()[1] == 0 && drop == 0) {
			bubbles.back().setVelocity(velocity * sin(getRadian(angle)), velocity * -cos(getRadian(angle)), 0);
			timecount = 0;
			seccount = 0;
			timer = false;
			prediction_sw = 0;
		}
	}
	// ESC key - exit
	else if (key == 27)
		exit(0);
	// R key - restart the game
	else if (key == 114) {
		reset();
	}
}

void shootingAngle(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		if (angle > -80)
			angle -= 5;
			prediction_sw = 0;
		break;
	case GLUT_KEY_RIGHT:
		if (angle < 80)
			angle += 5;
			prediction_sw = 0;
		break;
	}
}


void renderScene() {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(LEFTBOUND, RIGHTBOUND, DOWNBOUND, UPBOUND, -100.0, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// display characters
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	draw_characters(GLUT_BITMAP_HELVETICA_18, "SCORE", 1.0, 1.8, 0.0);
	draw_characters(GLUT_BITMAP_HELVETICA_18, "TIME", 10.0, 1.8, 0.0);
	draw_characters(GLUT_BITMAP_HELVETICA_18, "NEXT", 0.8, 28.9, 0.0);
	glPopMatrix();

	// time bar
	glPushMatrix();
	glColor3f(1.0, (1 - timecount / 6.0), 0.0);
	glBegin(GL_QUADS);
	glVertex3f(12, 3.55, 0.0);
	glVertex3f(12, 3.95, 0.0);
	glVertex3f(18.5 - timecount, 3.95, 0.0);
	glVertex3f(18.5 - timecount, 3.55, 0.0);
	glEnd();
	glPopMatrix();

	// boundary lines
	glPushMatrix();
	glBegin(GL_QUADS);
	glColor3f(1.0, 1.0, 1.0);
	glVertex2f(0, line[0] - 1);
	glVertex2f(20, line[0] - 1);
	glVertex2f(20, line[0] - 1.5);
	glVertex2f(0, line[0] - 1.5);
	glEnd();
	glBegin(GL_QUADS);
	glVertex2f(0, 27.8);
	glVertex2f(20, 27.8);
	glVertex2f(20, 27.3);
	glVertex2f(0, 27.3);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);
	string sc = to_string(score) + " points";
	const char* scorechar = sc.c_str();
	draw_characters(GLUT_BITMAP_HELVETICA_18, scorechar, 4.4, 4.0, 0);
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	light->draw();

	glPushMatrix();
	glTranslatef(10, 29, 0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glPushMatrix();
	glRotatef(angle, 0, 1, 0);
	cyl2.draw();
	glPushMatrix();
	glTranslatef(0, 0, 2.1);
	cyl1.setProperties(0.8, 0, 1, 20, 20);
	cyl1.draw();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	createPopupMenus();
	bubbles.back().move();
	if (do_prediction == 1) {
		if (prediction_sw == 0 && game_over == 0 && bubbles.back().getVelocity()[0] == 0 && bubbles.back().getVelocity()[1] == 0)
		{
			if (Prediction.size() >= 1) {
				Prediction.erase(Prediction.begin());
			}
			Bubbles PredictionBubble;
			PredictionBubble.setMTLforprediction();
			PredictionBubble.setCenter(10.0, 29.0, 0.0);
			Prediction.push_back(PredictionBubble);
			Prediction.back().setVelocity(velocity * sin(getRadian(angle)), velocity * -cos(getRadian(angle)), 0);
			printf("MakeBubble\n");
			prediction_sw = 1;
		}
	}
	if (Prediction.size() >= 1) {
		Prediction.back().move();
	}
	
	for (vector<Bubbles>::size_type i = 0; i < bubbles.size(); i++) {
		bubbles[i].draw();
	}
	for (vector<Bubbles>::size_type i = 0; i < temp.size(); i++) {
		temp[i].draw();
	}
	if (Prediction.size() >= 1) {
		Prediction[0].draw();
	}
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);

	if (game_over == 1) {
		glColor3f(1.0, 1.0, 1.0);
		draw_characters(GLUT_BITMAP_HELVETICA_18, "Game Over", 7.75, 17, 30);
		timer = false;
	}

	glutSwapBuffers();
}

void processMainMenu(int option)
{//MakeAPrediction을 눌렀을때
	if (option == 1) {
		do_prediction = 1;
	}
}


void createPopupMenus(){
	GLint mainMenu = glutCreateMenu(processMainMenu);
	glutAddMenuEntry("MakeAPrediction", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char** argv) {

	srand((unsigned int)time(NULL));
	// init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(800, 50);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Puzzle Bobble");
	init();

	// register callbacks
	glutDisplayFunc(renderScene);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(shootingAngle);
	glutIdleFunc(idle);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 0;
}
