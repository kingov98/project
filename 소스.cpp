#include <vector>
#include <iostream>
#include "Light.h"
#include "Bubbles.h"
#include "Cylinder.h"
#include "time.h"
#include "string"
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
Cylinder cyl1; Cylinder cyl2;
Light* light;
double s3 = sqrt(3);
double line[12];
double velocity = 0.5;
int angle = 0;
int game_over = 0;
int timer = 0;
int t = 100;
int arr[10][12];
int samecolorlength = 0;

void stopwatch(int Timer) {
	int NewTime = (int)time(NULL);
	static int oldTime = NewTime;
	if (Timer == 1) {
		NewTime = (int)time(NULL);
		if (NewTime != oldTime) {
			t -= 1;
			oldTime = NewTime;
			if (t == 0) {
				timer = 0;
				game_over = 1;
			}
		}
	}

}

double getRadian(int num) {
	return num * (PI / 180);
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

	/* for (double i = 1; i < 20; i += 2) {
		for (double j = 7.5; j < 28; j += 2 * s3) {
			Bubbles bubble1(initialBubble);
			bubble1.setCenter(i, j, 0);
			bubbles.push_back(bubble1);
		}
	}

	for (double i = 2; i < 20; i += 2) {
		for (double j = 7.5 + s3; j < 28; j += 2 * s3) {
			Bubbles bubble2(initialBubble);
			bubble2.setCenter(i, j, 0);
			bubbles.push_back(bubble2);
		}
	} */
}

void draw_characters(void* font, const char* c, float x, float y,float z) {
	glColor3f(1.0, 1.0, 1.0);
	glRasterPos3f(x, y, z);
	for (int i = 0; i < strlen(c); i++)
		glutBitmapCharacter(font, c[i]);
}

void idle() {
	// time counting

	if (bubbles.back().getVelocity()[0] != 0 || bubbles.back().getVelocity()[1] != 0) {
		if (temp.size() == 1) {
			temp[0].setCenter(10.0, 29.0, 0.0);
			Bubbles nextBubble;
			temp.push_back(nextBubble);
		}
	}

	// boundary check
	if (bubbles.back().getCenter()[0] + 1 >= RIGHTBOUND || bubbles.back().getCenter()[0] - 1 <= LEFTBOUND)
		bubbles.back().setVelocity(-bubbles.back().getVelocity()[0], bubbles.back().getVelocity()[1], 0);

	// upbound check
	if (bubbles.back().getCenter()[1] <= line[0]) {
		bubbles.back().setVelocity(0, 0, 0);
		bubbles.back().decidePosition();
		bubbles.back().setsamecolor(1);
		for (int i = 0; i < bubbles.size() - 1; i++) {
			bubbles.back().search(bubbles[i]);
		}
		for (int i = 0; i < bubbles.size() - 1; i++) {
			if (bubbles[i].getsamecolor() == 1) {
				for(int j=0; j < bubbles.size() - 1; j++)
				bubbles[i].search(bubbles[j]);
			}
		}
		for (int i = 0; i < bubbles.size(); i++) {
			if (bubbles[i].getsamecolor() == 1) {
				samecolorlength++;
			}
		}
		if (samecolorlength >= 3) {
			for (int i = 0; i < bubbles.size(); i++) {
				if (bubbles[i].getsamecolor() == 1) {
					bubbles[i].setCenter(1,1,0);
				}
			}
		}
		samecolorlength = 0;
		for (int i = 0; i < bubbles.size(); i++) {
			bubbles[i].setsamecolor(0);
		}
		bubbles.push_back(temp[0]);
		temp.erase(temp.begin());
	}

	// collision handling
	for (vector<Bubbles>::size_type i = 0; i < bubbles.size() - 1; ++i) {
		if (bubbles.back().collisionDetection(bubbles[i])) {
			bubbles.back().setVelocity(0, 0, 0);
			bubbles.back().decidePosition();
			bubbles.back().setsamecolor(1);
			for (int i = 0; i < bubbles.size() - 1; i++) {
				bubbles.back().search(bubbles[i]);//쏜 버블과 색이 같으면서 인접한 버블의 samecolor=1
			}
			for (int i = 0; i < bubbles.size() - 1; i++) {
				if (bubbles[i].getsamecolor() == 1) {
					for (int j = 0; j < bubbles.size() - 1; j++)
						bubbles[i].search(bubbles[j]);//위에서 1로 만든 애들을 상대로 다시 search 
				}
			}
			for (int i = 0; i < bubbles.size(); i++) {
				if (bubbles[i].getsamecolor() == 1) {
					samecolorlength++;//samecolor=1인 애들의 갯수를 세고
				}
			}
			if (samecolorlength >= 3) {
				for (int i = 0; i < bubbles.size(); i++) {
					if (bubbles[i].getsamecolor() == 1) {
						bubbles[i].setCenter(1, 1, 0);//3보다 크면 samecolor=1인애들의 중심을 이동
					}
				}
			}
			samecolorlength = 0;
			for (int i = 0; i < bubbles.size(); i++) {
				bubbles[i].setsamecolor(0);//3보다 작으면 전부 원래대로 samecolor=0으로 만듬
			}
			bubbles.push_back(temp[0]);
			temp.erase(temp.begin());

			break;
		}
	}


	glutPostRedisplay();
}

void reset() {
	angle = 0;
	game_over = 0;
	timer = 0;
	t = 100;

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
	if (key == 32 && game_over==0) {
		if (bubbles.back().getVelocity()[0] == 0 && bubbles.back().getVelocity()[1] == 0) {
			timer = 1;
			bubbles.back().setVelocity(velocity * sin(getRadian(angle)), velocity * -cos(getRadian(angle)), 0);
		}
	}
	if (key == 114) {
		reset();
	}

	// ESC key
	if (key == 27)
		exit(0);
}

void shootingAngle(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		if (angle > -85)
			angle -= 5;
		break;
	case GLUT_KEY_RIGHT:
		if (angle < 85)
			angle += 5;
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
	draw_characters(GLUT_BITMAP_HELVETICA_18, "SCORE", 1, 1.8,0);
	draw_characters(GLUT_BITMAP_HELVETICA_18, "TIME", 10, 1.8,0);
	std::string s = std::to_string(t);
	char const* pchar = s.c_str();
	draw_characters(GLUT_BITMAP_HELVETICA_18, pchar, 15, 1.8,0);
	draw_characters(GLUT_BITMAP_HELVETICA_18, "NEXT", 0.8, 28.9,0);
	if (game_over == 1) {
		draw_characters(GLUT_BITMAP_HELVETICA_18, "Game Over", 7.75, 16, 500);
		timer = 0;
	}
	glPopMatrix();

	// boundary lines
	glPushMatrix();
	glBegin(GL_QUADS);
	glVertex2f(0, line[0] - 1);
	glVertex2f(20, line[0] - 1);
	glVertex2f(20, line[0] - 1.45);
	glVertex2f(0, line[0] - 1.45);
	glEnd();

	glBegin(GL_QUADS);
	glVertex2f(0, 27.8);
	glVertex2f(20, 27.8);
	glVertex2f(20, 27.3);
	glVertex2f(0, 27.3);
	glEnd();
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
	glPopMatrix();
	glPopMatrix();

	bubbles.back().move();
	
	for (vector<Bubbles>::size_type i = 0; i < bubbles.size(); i++) {
		bubbles[i].draw();
	}
	for (vector<Bubbles>::size_type i = 0; i < temp.size(); i++) {
		temp[i].draw();
	}

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	stopwatch(timer);

	glutSwapBuffers();
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
