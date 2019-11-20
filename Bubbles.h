#pragma once
#include "Shape3D.h"

class Bubbles : public Shape3D
{
public:
	Bubbles();
	Bubbles(const Bubbles& bub);
	Vector3 getProperties() const;
	int getTilex();
	int getTiley();
	int getC();
	bool collisionDetection(const Bubbles& bub);
	void collisionHandling(Bubbles& bub);
	void decidePosition();
	void setMTL();
	void draw() const;

private:
	Vector3 properties;
	int tilex;
	int tiley;
	int c;
};

