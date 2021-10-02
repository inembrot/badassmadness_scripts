#pragma once
#include "stdafx.h"
using namespace std;
class Vecteur4D
{

private:
	float x;
	float y;
	float z;
	float w;

public:
	Vecteur4D();
	Vecteur4D(float x, float y, float z, float w);
	~Vecteur4D();
	float getX()const;
	float getY()const;
	float getZ()const;
	float getW()const;
	void setX(float x);
	void setY(float y);
	void setZ(float z);
	void setW(float w);
	Vecteur4D operator+(const Vecteur4D v);
	Vecteur4D operator-(const Vecteur4D v);
	Vecteur4D operator*(const float s);
	float norme();
	Vecteur4D normalisation();
	Vecteur4D produitVectoriel(const Vecteur4D v);
	float produitScalaire(Vecteur4D v);
	Vecteur4D interpolation(Vecteur4D v, float f);
	void affiche(ostream& os)const;
	void operator()(float x, float y, float z, float w);
	float& operator[](const int i);
};

ostream& operator<<(ostream& os, const Vecteur4D v);