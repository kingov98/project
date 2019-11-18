#include "Material.h"

void Material::setMTL(const Material& mtl) {
	setEmission(mtl.getEmission());
	setAmbient(mtl.getAmbient());
	setDiffuse(mtl.getDiffuse());
	setSpecular(mtl.getSpecular());
	setShininess(mtl.getShininess());
}

void Material::setEmission(float r, float g, float b, float a) {
	emission[0] = r;
	emission[1] = g;
	emission[2] = b;
	emission[3] = a;
}

void Material::setEmission(const float* rgba) {
	setEmission(rgba[0], rgba[1], rgba[2], rgba[3]);
}

void Material::setAmbient(float r, float g, float b, float a) {
	ambient[0] = r;
	ambient[1] = g;
	ambient[2] = b;
	ambient[3] = a;
}

void Material::setAmbient(const float* rgba) {
	setAmbient(rgba[0], rgba[1], rgba[2], rgba[3]);
}

void Material::setDiffuse(float r, float g, float b, float a) {
	diffuse[0] = r;
	diffuse[1] = g;
	diffuse[2] = b;
	diffuse[3] = a;
}

void Material::setDiffuse(const float* rgba) {
	setDiffuse(rgba[0], rgba[1], rgba[2], rgba[3]);
}

void Material::setSpecular(float r, float g, float b, float a) {
	specular[0] = r;
	specular[1] = g;
	specular[2] = b;
	specular[3] = a;
}

void Material::setSpecular(const float* rgba) {
	setSpecular(rgba[0], rgba[1], rgba[2], rgba[3]);
}

void Material::setShininess(float sh) {
	shininess[0] = sh;
}

void Material::setShininess(const float* sh) {
	setShininess(sh[0]);
}

const float* Material::getEmission() const {
	return emission;
}

const float* Material::getAmbient() const {
	return ambient;
}

const float* Material::getDiffuse() const {
	return diffuse;
}

const float* Material::getSpecular() const {
	return specular;
}

const float* Material::getShininess() const {
	return shininess;
}
