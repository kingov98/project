#include "Bubbles.h"

int radius = 1;
float tileheight = sqrt(3) * radius;
float tilewidth = 2 * radius;
float y_offset = 7.5;

int red[4] = { 1.0, 0.0, 0.0, 1 };
int yellow[4] = { 1.0, 1.0, 0.0, 1 };
int green[4] = { 0.0, 1.0, 0.0, 1 };
int blue[4] = { 0.0, 0.0, 1.0, 1 };


Bubbles::Bubbles() : samecolor(0), check(0),x_dot(0),y_dot(0),x(0),y(0) {
	properties.setXYZ(1, 20, 20);
	setCenter(1.8, 30.5, 0);
	setMTL();
}

Bubbles::Bubbles(const Bubbles& bub) : Shape3D(bub) {
	properties = bub.properties;
	color = bub.color;
	samecolor = bub.samecolor;
	check = bub.check;
}

void Bubbles::setMTL() {
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
	setColor();
}

void Bubbles::setMTLforprediction(int c) {
	switch(c) {
	case 1:
		mtl.setAmbient(red[0]/5.0, red[1]/5.0, red[2]/5.0, 1);
		break;
	case 2:
		mtl.setAmbient(yellow[0]/5.0, yellow[1]/ 5.0, yellow[2]/ 5.0, 1);
		break;
	case 3:
		mtl.setAmbient(green[0]/ 5.0, green[1]/ 5.0, green[2]/ 5.0, 1);
		break;
	case 4:
		mtl.setAmbient(blue[0]/ 5.0, blue[1]/ 5.0, blue[2]/ 5.0, 1);
		break;
	}
}

Vector3 Bubbles::getProperties() const {
	return properties;
}

bool Bubbles::collisionDetection(const Bubbles& bub, float r) {
	// collision detection
	Vector3 c3 = this->getCenter() - bub.getCenter();
	if (sqrt(dotProduct(c3, c3)) <= r)
		return true;
	else
		return false;
}
bool Bubbles::newCollisionDetection(const Bubbles& bub) {
	// collision detection for searching
	Vector3 c3 = this->getCenter() - bub.getCenter();
	if (sqrt(dotProduct(c3, c3)) <= this->getProperties()[0] + bub.getProperties()[0] + 0.05)
		return true;
	else
		return false;
}
void Bubbles::collisionHandling(Bubbles& bub) {
	this->setVelocity(0, 0, 0);
}
void Bubbles::setColor() {
	if (this->getMTL().getAmbient()[0] == red[0] && this->getMTL().getAmbient()[1] == red[1] && this->getMTL().getAmbient()[2] == red[2]) {
		this->color = 1;
	}
	else if(this->getMTL().getAmbient()[0] == yellow[0] && this->getMTL().getAmbient()[1] == yellow[1] && this->getMTL().getAmbient()[2] == yellow[2]) {
		this->color = 2;
	}
	else if(this->getMTL().getAmbient()[0] == green[0] && this->getMTL().getAmbient()[1] == green[1] && this->getMTL().getAmbient()[2] == green[2]) {
		this->color = 3;
	}
	else if (this->getMTL().getAmbient()[0] == blue[0] && this->getMTL().getAmbient()[1] == blue[1] && this->getMTL().getAmbient()[2] == blue[2]) {
		this->color = 4;
	}
	/*else {
		printf("Error");
	}*/
};
int Bubbles::getColor() {
	return this->color;
}
int Bubbles::getTilex() {
	return this->tilex;
}
int Bubbles::getTiley() {
	return this->tiley;
}
void Bubbles::MakeGrid() {
	this-> x = this->getCenter()[0];
	this-> y = this->getCenter()[1];

	this->tiley = (y - y_offset + tileheight / 2) / tileheight;
	if (this->tiley < 0) {
		this->tiley = 0;
	}
	if (tiley % 2 != 0) {
		x -= tilewidth / 2;
	}
	this->tilex = x / tilewidth;

	this-> y_dot = tiley * tileheight + y_offset;
	this-> x_dot = tilex * tilewidth + radius;

	if (tiley % 2 != 0) {
		x_dot += tilewidth / 2;
		if (tilex == 9)
			x_dot -= tilewidth;
	}
	
};
void Bubbles::UpdateGrid() {
	Vector3 v1;
	v1.setXYZ(x, y, 0);
	Vector3 v2;
	v2.setXYZ(x_dot, y_dot, 0);
	float c1 = dotProduct(v1-v2,v1-v2);
	if (x >= x_dot) {//아래쪽 방향을 기준으로 (x_dot,y_dot)->충돌지점(x,y)를 바라보는 각도를 기준으로
		if ((y - y_dot) / sqrt(c1) >= 0.5) {
				x = 2 * x_dot + radius - x;
				y += tileheight;
		}
		else if (-0.5 <= (y - y_dot) / sqrt(c1) < 0.5) {
				x += tilewidth;
		}
		else if ((y - y_dot) / sqrt(c1) < -0.5) {
				x = 2 * x_dot + radius - x;
				if (y_dot == 7.5) {
					y = 15 + tileheight - y;
				}else {
					y -= tileheight;
				}
		}
	}
	else if (x < x_dot) {
		if ((y - y_dot) / sqrt(c1) >= 0.5) {
				x = 2 * x_dot - radius - x;
				y += tileheight;
		}
		else if (-0.5 <= (y - y_dot) / sqrt(c1) < 0.5) {
				x -= tilewidth;
		}
		else if ((y - y_dot) / sqrt(c1) < -0.5) {
				x = 2 * x_dot - radius - x;
				if (y_dot == 7.5) {
					y = 15 + tileheight - y;
				}
				else {
					y -= tileheight;
				}
		}
	}
	this->setCenter(x, y, 0);
};
void Bubbles::decidePosition() {

	this->setCenter(x_dot, y_dot, 0);
}
void Bubbles::setSamecolor(int same) {
	this->samecolor = same;
}
int Bubbles::getSamecolor() {
	return this->samecolor;
}
void Bubbles::setCheck(int star) {
	this->check = star;
}
int Bubbles::getCheck() {
	return this->check;
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
void Bubbles::predictiondraw() const {
	glPushMatrix();
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mtl.getAmbient());

	glTranslatef(center[0], center[1], center[2]);
	glutSolidSphere(properties[0], properties[1], properties[2]);
	glPopMatrix();
}





