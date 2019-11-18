#include "Shape3D.h"

Shape3D::Shape3D() {
	center.setXYZ(0, 0, 0);
	velocity.setXYZ(0, 0, 0);

	setMTL();
}

Shape3D::Shape3D(const Shape3D& sh3d) {
	setCenter(sh3d.center);
	setVelocity(sh3d.velocity);
	
	mtl.setMTL(sh3d.mtl);
}

void Shape3D::setCenter(float x, float y, float z) {
	center[0] = x;
	center[1] = y;
	center[2] = z;
}

void Shape3D::setX(float x) {
	center[0] = x;
}

void Shape3D::setY(float y) {
	center[1] = y;
}

void Shape3D::setCenter(const Vector3& c) {
	center = c;
}

Vector3 Shape3D::getCenter() const {
	return center;
}

void Shape3D::setVelocity(float x, float y, float z) {
	velocity[0] = x;
	velocity[1] = y;
	velocity[2] = z;
}

void Shape3D::setVelocity(const Vector3& v) {
	velocity = v;
}

Vector3 Shape3D::getVelocity() const {
	return velocity;
}

void Shape3D::move() {
	center = center + velocity;
}

void Shape3D::setMTL() {
	mtl.setEmission(0.1, 0.1, 0.1, 1);
	mtl.setAmbient(0.3, 0.3, 0.3, 1);
	mtl.setDiffuse(0.5, 0.5, 0.5, 1);
	mtl.setSpecular(1.0, 1.0, 1.0, 1);
	mtl.setShininess(40);
}