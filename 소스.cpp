#include <vector>
#include "Light.h"
#include "Bubbles.h"
#include "Cylinder.h"
#include "math.h"
// include header files

using namespace std;

#define WIDTH 450
#define HEIGHT 720

#define LEFTBOUND 0
#define RIGHTBOUND 20
#define DOWNBOUND 0
#define UPBOUND 32

#define PI 3.141592

// global variables
vector<Bubbles> bubbles;
vector<Bubbles> temp;
Cylinder cyl1; Cylinder cyl2;
Light* light;
double s3 = sqrt(3);
double line[12];
int angle = 0;
int radius = 1;
float tilewidth = 2 * radius;
float tileheight = s3 * radius;
int y_offset = 150;

double getRadian(int num) {
	return num * (PI / 180);
}

int getTileCoordinateX(int x,int y) {
	int tilex = x / tilewidth;
	int tiley = (y - y_offset - radius) / tileheight;

	// X offset for odd rows
	if (((y - y_offset-radius) / (int)tileheight) % 2) {
		tilex += tilewidth / 2;
	}

	return  tilex;
}
int getTileCoordinateY(int x, int y) {
	int tiley = (y - y_offset - radius) / tileheight;

	return  tiley;
}

float setTileCoordinateX(int tilex, int tiley){
	int y = tiley * tileheight + y_offset + radius;
	int x = tilex * tilewidth;

	if ((y - y_offset) % 2) {
		x += tilewidth / 2;
	}
	return x;
}
float setTileCoordinateY(int tilex, int tiley) {
	int y = tiley * tileheight + y_offset + radius;

	return y;
}

void init() {
	light = new Light(200, 200, 200, GL_LIGHT0);
	light->setAmbient(0.3, 0.3, 0.3, 1.0);
	light->setDiffuse(0.5, 0.5, 0.5, 1.0);
	light->setSpecular(1.0, 1.0, 1.0, 1.0);

	Bubbles initialBubble;
	initialBubble.setCenter(10, 2.5, 0);
	bubbles.push_back(initialBubble);

	Bubbles nextBubble;
	temp.push_back(nextBubble);

	for (int i = 0; i < 12; i++) {
		line[i] = 5.5 + s3 * i;
	}

	/* for (double i = 1; i < 20; i += 2) {
		for (double j = 5.5 + s3; j < 25; j += 2 * s3) {
			Bubbles bubble1(bubble);
			bubble1.setCenter(i, j, 0);
			bubbles.push_back(bubble1);
		}
	}

	for (double i = 2; i < 20; i += 2) {
		for (double j = 5.5; j < 25; j += 2 * s3) {
			Bubbles bubble2(bubble);
			bubble2.setCenter(i, j, 0);
			bubbles.push_back(bubble2);
		}
	} */
}

void draw_characters(void* font, const char* c, float x, float y) {
	glColor3f(1.0, 1.0, 1.0);
	glRasterPos2f(x, y);
	for (int i = 0; i < strlen(c); i++)
		glutBitmapCharacter(font, c[i]);
}

void idle() {
	// time counting

	if (bubbles.back().getVelocity()[0] != 0 || bubbles.back().getVelocity()[1] != 0) {
		if (temp.size() == 1) {
			temp[0].setCenter(10, 2.5, 0);
			Bubbles nextBubble;
			temp.push_back(nextBubble);
		}

		// boundary check
		if (bubbles.back().getCenter()[0] + 1 >= RIGHTBOUND || bubbles.back().getCenter()[0] - 1 <= LEFTBOUND)
			bubbles.back().setVelocity(-bubbles.back().getVelocity()[0], bubbles.back().getVelocity()[1], 0);

		// upbound check
		if (bubbles.back().getCenter()[1] >= line[11]) {
			bubbles.back().setVelocity(0, 0, 0);
			bubbles.back().setY(line[11]);
			bubbles.push_back(temp[0]);
			temp.erase(temp.begin());
		}
		
		// collision handling
		for (vector<Bubbles>::size_type i = 0; i < bubbles.size() - 1; i++) {
			if (bubbles.back().collisionDetection(bubbles[i])) {
				bubbles.back().collisionHandling(bubbles[i]);
				bubbles.push_back(temp[0]);
				temp.erase(temp.begin());
				break;
			}
		}

	}

	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	// Spacebar - ball shooting
	if (key == 32) {
		if (bubbles.back().getVelocity()[0] == 0 && bubbles.back().getVelocity()[1] == 0)
			bubbles.back().setVelocity(0.3*sin(getRadian(angle)), 0.3*cos(getRadian(angle)), 0);
	}

	// ESC key
	if (key == 27)
		exit(0);
}

void shootingAngle(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		if (angle > -85) {
		angle -= 5;
	}
		break;
	case GLUT_KEY_RIGHT:
		if (angle < 85) {
			angle += 5;
		}
		break;
	}
}

void processMouse(int button, int state, int x, int y) {
	printf("(%d, %d, %d, %d)\n", button, state, x, y);
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
	draw_characters(GLUT_BITMAP_HELVETICA_18, "SCORE", 1, 30.2);
	draw_characters(GLUT_BITMAP_HELVETICA_18, "TIME", 10, 30.2);
	draw_characters(GLUT_BITMAP_HELVETICA_18, "NEXT", 0.8, 3.1);
	glPopMatrix();

	glPushMatrix();
	glBegin(GL_QUADS);
	glVertex2f(0,4.2);
	glVertex2f(20,4.2);
	glVertex2f(20,4.7);
	glVertex2f(0,4.7);
	glEnd();

	glBegin(GL_QUADS);
	glVertex2f(0, line[11] + 0.9);
	glVertex2f(20, line[11] + 0.9);
	glVertex2f(20, line[11] + 1.35);
	glVertex2f(0, line[11] + 1.35);
	glEnd();
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	light->draw();

	glPushMatrix();
	glTranslatef(10, 2.5, 0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glPushMatrix();
	glRotatef(-angle, 0, 1, 0);
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

	glutSwapBuffers();
}

int main(int argc, char** argv) {
	// init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(800, 50);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Puzzle Bobble");
	init();

	// register callbacks
	glutDisplayFunc(renderScene);
	glutMouseFunc(processMouse);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(shootingAngle);
	glutIdleFunc(idle);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 0;
}
