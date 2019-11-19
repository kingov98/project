#include "Bubbles.h"
#include <iostream>
#include <ctime>
#include <math.h>


int radius = 1;
float tileheight = sqrt(3) * radius;
float tilewidth = 2 * radius;
float y_offset = 4.75;

Bubbles::Bubbles() {
	properties.setXYZ(1, 100, 100);
	setCenter(1.8, 30, 0);
	setMTL();
}

Bubbles::Bubbles(const Bubbles& bub) : Shape3D(bub) {
	properties = bub.properties;
}

void Bubbles::setMTL() {
	int red[4] = { 1.0, 0.0, 0.0, 1 };
	int yellow[4] = { 1.0, 1.0, 0.0, 1 };
	int green[4] = { 0.0, 1.0, 0.0, 1 };
	int blue[4] = { 0.0, 0.0, 1.0, 1 };
	
	srand((unsigned int)time(NULL));
	const int color = rand() % 4;
	switch (color) {
	case 0:
		mtl.setAmbient(red[0], red[1], red[2], red[3]);
		break;
	case 1:
		mtl.setAmbient(yellow[0], yellow[1], yellow[2], yellow[3]);
		break;
	case 2:
		mtl.setAmbient(green[0], green[1], green[2], green[3]);
		break;
	case 3:
		mtl.setAmbient(blue[0], blue[1], blue[2], blue[3]);
		break;
	}
}

Vector3 Bubbles::getProperties() const {
	return properties;
}

bool Bubbles::collisionDetection(const Bubbles& bub) {
	// collision detection
	Vector3 c3 = this->getCenter() - bub.getCenter();
	if (sqrt(dotProduct(c3, c3)) <= this->getProperties()[0] + bub.getProperties()[0])
		return true;
	else
		return false;
}

void Bubbles::collisionHandling(Bubbles& bub) {
	this->setVelocity(0, 0, 0);
}


void Bubbles::decidePosition(Bubbles& bub) {
	float x = this->getCenter()[0];
	float y = this->getCenter()[1];


	int tiley = (y - y_offset - radius + (tileheight / 2)) / tileheight;

	if (tiley % 2) {
		x -= tilewidth / 2;
	}
	int tilex = x / (tilewidth);

	float y_dot = tiley * tileheight + y_offset + radius;
	int x_dot = tilex * tilewidth+radius;
	std::printf("%d %d\n", tilex,tiley);
	if (tiley % 2) {
		x_dot += tilewidth/2;
	}
	this->setCenter(x_dot, y_dot, 0);


};

void Bubbles::draw() const {
	glPushMatrix();

	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_EMISSION, mtl.getEmission());
	glMaterialfv(GL_FRONT, GL_AMBIENT, mtl.getAmbient());
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mtl.getDiffuse());
	glMaterialfv(GL_FRONT, GL_SPECULAR, mtl.getSpecular());
	glMaterialfv(GL_FRONT, GL_SHININESS, mtl.getShininess());

	glTranslatef(center[0], center[1], center[2]);
	glutSolidSphere(properties[0], properties[1], properties[2]);
	glPopMatrix();
}