#pragma once
#include "Cylinder.h"

Cylinder::Cylinder() {
	properties[0] = 0.5;
	properties[1] = 2.4;
	properties[2] = 21;
	properties[3] = 21;
	setMTL();
}

void Cylinder::setHeight(float h) {
	properties[1] = h;
}

void Cylinder::draw() const {
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);

	/*glShadeModel(GL_SMOOTH);*/
	glMaterialfv(GL_FRONT, GL_EMISSION, mtl.getEmission());
	glMaterialfv(GL_FRONT, GL_AMBIENT, mtl.getAmbient());
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mtl.getDiffuse());
	glMaterialfv(GL_FRONT, GL_SPECULAR, mtl.getSpecular());
	glMaterialfv(GL_FRONT, GL_SHININESS, mtl.getShininess());

	gluCylinder(qobj, properties[0], properties[0], properties[1], properties[2], properties[3]);
}