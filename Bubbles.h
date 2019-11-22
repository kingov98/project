#pragma once
#include "Shape3D.h"
#include <vector>

class Bubbles : public Shape3D
{
public:
	Bubbles();
	Bubbles(const Bubbles& bub);
	Vector3 getProperties() const;
	int getTilex();
	int getTiley();
	int getColor();
	int getsamecolor();
	int getcheck();
	bool collisionDetection(const Bubbles& bub);
	void setColor();
	void setsamecolor(int a);
	void setcheck(int a);
	void collisionHandling(Bubbles& bub);
	void decidePosition();
	void setMTL();
	void draw() const;
	void search(Bubbles& bub);
	void checksearch(Bubbles& bub);

private:
	Vector3 properties;
	int tilex;
	int tiley;
	int Color;
	int samecolor;
	int check;
};

