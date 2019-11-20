#include "Bubbles.h"
#include <vector>
#include <iostream>
#include <ctime>
#include <math.h>

int radius = 1;
float tileheight = sqrt(3) * radius;
float tilewidth = 2 * radius;
float y_offset = 7.5;

int red[4] = { 1.0, 0.0, 0.0, 1 };
int yellow[4] = { 1.0, 1.0, 0.0, 1 };
int green[4] = { 0.0, 1.0, 0.0, 1 };
int blue[4] = { 0.0, 0.0, 1.0, 1 };

std::vector<int> colorarr;

Bubbles::Bubbles() {
	properties.setXYZ(1, 100, 100);
	setCenter(1.8, 30.5, 0);
	setMTL();
}

Bubbles::Bubbles(const Bubbles& bub) : Shape3D(bub) {
	properties = bub.properties;
}

void Bubbles::setMTL() {	
	srand((unsigned int)time(NULL));
	const int color = rand() % 4+1;

	switch (color) {
	case 1:
		mtl.setAmbient(red[0], red[1], red[2], red[3]);
		break;
	case 2:
		mtl.setAmbient(yellow[0], yellow[1], yellow[2], yellow[3]);
		break;
	case 3:
		mtl.setAmbient(green[0], green[1], green[2], green[3]);
		break;
	case 4:
		mtl.setAmbient(blue[0], blue[1], blue[2], blue[3]);
		break;
	}
	colorarr.push_back(color);

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

int Bubbles::getTilex(){
	return this->tilex;
}
int Bubbles::getTiley() {
	return this ->tiley;
}
int Bubbles::getC() {
	return this ->c;
}

void Bubbles::decidePosition() {
	float x = this->getCenter()[0];
	float y = this->getCenter()[1];

	this-> tiley = (y - y_offset + tileheight / 2) / tileheight;

	if (tiley % 2 != 0)
		x -= tilewidth / 2;

	this-> tilex = x / tilewidth;

	float y_dot = tiley * tileheight + y_offset;
	int x_dot = tilex * tilewidth + radius;

	if (tiley % 2 != 0) {
		x_dot += tilewidth / 2;
		if (tilex == 9)
			x_dot -= tilewidth;
	}

	this->setCenter(x_dot, y_dot, 0);
}

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