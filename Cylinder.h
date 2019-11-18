#pragma once
#include "Shape3D.h"

class Cylinder : public Shape3D
{
public:
	Cylinder();
	void setHeight(float x);
	void draw() const;

private:
	float properties[4];
};

