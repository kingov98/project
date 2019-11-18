#pragma once

class Vector3
{
public:
	Vector3();
	void setXYZ(float x, float y, float z);
	const float* getXYZ() const;

	Vector3& operator=(const Vector3& vec3);
	
	float& operator[](const int i);
	float operator[](const int i) const;
	
private:
	float xyz[3];
};

Vector3 operator+(const Vector3& v1, const Vector3& v2);
Vector3 operator-(const Vector3& v1, const Vector3& v2);
Vector3 operator-(const Vector3& v);
Vector3 operator*(const float s, const Vector3& vec3);
float dotProduct(const Vector3& v1, const Vector3& v2);