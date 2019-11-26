#pragma once
#include "Cylinder.h"

Cylinder::Cylinder() {
	properties[0] = 0.5;
	properties[1] = 0.5;
	properties[2] = 2.4;
	properties[3] = 20;
	properties[4] = 20;
	setMTL();
}

void Cylinder::setProperties(float a0, float a1, float a2, float a3, float a4) {
	properties[0] = a0;
	properties[1] = a1;
	properties[2] = a2;
	properties[3] = a3;
	properties[4] = a4;
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

	gluCylinder(qobj, properties[0], properties[1], properties[2], properties[3], properties[4]);
}