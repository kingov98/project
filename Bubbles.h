#pragma once
#include "Shape3D.h"
#include <ctime>
#include <math.h>

class Bubbles : public Shape3D
{
public:
	Bubbles();
	Bubbles(const Bubbles& bub);
	Vector3 getProperties() const;
	bool collisionDetection(const Bubbles& bub,float r);
	bool newCollisionDetection(const Bubbles& bub);
	void collisionHandling(Bubbles& bub);
	void MakeGrid();
	void UpdateGrid();
	void decidePosition();
	int getTilex();
	int getTiley();
	void setColor();
	int getColor();
	void setSamecolor(int same);
	int getSamecolor();
	void setCheck(int star);
	int getCheck();
	void setMTL();
	void setMTLforprediction(int c);
	void draw() const;
	void predictiondraw() const;
private:
	Vector3 properties;
	int tilex;
	int tiley;
	int color;
	int samecolor;
	int check;
	float x_dot;
	float y_dot;
	float x;
	float y;
};



