#pragma once

#include "Material.h"
#include "Vector3.h"

class Shape3D
{
public:
	Shape3D();
	Shape3D(const Shape3D& sh3d);

	void setCenter(float x, float y, float z);
	void setCenter(const Vector3& c);
	void setX(float X);
	void setY(float y);
	Vector3 getCenter() const;
	void setVelocity(float x, float y, float z);
	void setVelocity(const Vector3& v);
	Vector3 getVelocity() const;
	void move();
	virtual void setMTL();
	virtual void draw() const = 0;

protected:
	Vector3 center;
	Vector3 velocity;
	Material mtl;
};

