#pragma once
#include "Shape3D.h"

class Cylinder : public Shape3D
{
public:
	Cylinder();
	void setProperties(float a0, float a1, float a2, float a3, float a4);
	void draw() const;

private:
	float properties[5];
};

