#pragma once
#include "Shape3D.h"

class Bubbles : public Shape3D
{
public:
	Bubbles();
	Bubbles(const Bubbles& bub);
	Vector3 getProperties() const;
	bool collisionDetection(const Bubbles& bub);
	void collisionHandling(Bubbles& bub);
	void decidePosition(Bubbles& bub);
	void setMTL();
	void draw() const;

private:
	Vector3 properties;
};

