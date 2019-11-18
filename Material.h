#pragma once

#include <stdlib.h>
#include <GL/glut.h>

class Material
{
public:
	void setMTL(const Material& mtl);
	void setEmission(float r, float g, float b, float a);
	void setEmission(const float* rgba);
	void setAmbient(float r, float g, float b, float a);
	void setAmbient(const float* rgba);
	void setDiffuse(float r, float g, float b, float a);
	void setDiffuse(const float* rgba);
	void setSpecular(float r, float g, float b, float a);
	void setSpecular(const float* rgba);
	void setShininess(float sh);
	void setShininess(const float* sh);

	const float* getEmission() const;
	const float* getAmbient() const;
	const float* getDiffuse() const;
	const float* getSpecular() const;
	const float* getShininess() const;

private:
	float emission[4];
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float shininess[1];
};

