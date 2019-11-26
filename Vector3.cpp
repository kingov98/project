#include "Vector3.h"

Vector3::Vector3() {}

void Vector3::setXYZ(float x, float y, float z) {
	xyz[0] = x;
	xyz[1] = y;
	xyz[2] = z;
}

const float* Vector3::getXYZ() const {
	return xyz;
}

Vector3& Vector3::operator=(const Vector3& vec3) {
	xyz[0] = vec3.xyz[0];
	xyz[1] = vec3.xyz[1];
	xyz[2] = vec3.xyz[2];
	return (*this);
}

float& Vector3::operator[](const int i) {
	return xyz[i];
}

float Vector3::operator[](const int i) const {
	return xyz[i];
}

Vector3 operator+(const Vector3& v1, const Vector3& v2) {
	Vector3 v3;
	v3.setXYZ(v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]);
	return v3;
}

Vector3 operator-(const Vector3& v1, const Vector3& v2) {
	Vector3 v3;
	v3.setXYZ(v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]);
	return v3;
}

Vector3 operator-(const Vector3& v) {
	Vector3 v3;
	v3.setXYZ(-v[0], -v[1], -v[2]);
	return v3;
}

bool operator!=(const Vector3& v1, const Vector3& v2) {
	if (v1[0] == v2[0] && v1[1] == v2[1] && v1[2] == v2[2])
		return false;
	else
		return true;
}

Vector3 operator*(const float s, const Vector3& vec3) {
	Vector3 v3;
	v3.setXYZ(vec3[0] * s, vec3[1] * s, vec3[2] * s);
	return v3;
}

float dotProduct(const Vector3& v1, const Vector3& v2) {
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}
