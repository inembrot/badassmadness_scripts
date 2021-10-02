#include "StdAfx.h"
#include "Vecteur4D.h"
#include "Math.h"


Vecteur4D::Vecteur4D()
{
	this->x=0;
	this->y=0;
	this->z=0;
	this->w=0;
}

Vecteur4D::Vecteur4D(float x, float y, float z, float w)
{
	this->x=x;
	this->y=y;
	this->z=z;
	this->w=w;
}

Vecteur4D::~Vecteur4D(void)
{
}

float Vecteur4D::getX()const
{
	return x;
}

float Vecteur4D::getY()const
{
	return y;
}

float Vecteur4D::getZ()const
{
	return z;
}

float Vecteur4D::getW()const
{
	return w;
}

void Vecteur4D::setX(float x){
	this->x=x;
}

void Vecteur4D::setY(float y){
	this->y=y;
}

void Vecteur4D::setZ(float z){
	this->z=z;
}

void Vecteur4D::setW(float w){
	this->w=w;
}

Vecteur4D Vecteur4D::operator+(const Vecteur4D v)
{
	Vecteur4D vTemp;
	vTemp.x=x+v.x;
	vTemp.y=y+v.y;
	vTemp.z=z+v.z;
	vTemp.w=w;
	return vTemp;
}

Vecteur4D Vecteur4D::operator-(const Vecteur4D v)
{
	Vecteur4D vTemp;
	vTemp.x=x-v.x;
	vTemp.y=y-v.y;
	vTemp.z=z-v.z;
	vTemp.w=w;
	return vTemp;
}

Vecteur4D Vecteur4D::operator*(const float s)
{
	Vecteur4D vTemp;
	vTemp.x=x*s;
	vTemp.y=y*s;
	vTemp.z=z*s;
	vTemp.w=w;
	return vTemp;
}

float Vecteur4D::norme()
{
	return sqrt((x*x)+(y*y)+(z*z));
}

Vecteur4D Vecteur4D::normalisation()
{
	Vecteur4D vTemp;
	vTemp.x=x/norme();
	vTemp.y=y/norme();
	vTemp.z=z/norme();
	vTemp.w=w;
	return vTemp;
}

Vecteur4D Vecteur4D::produitVectoriel(const Vecteur4D v)
{
	Vecteur4D vTemp;
	vTemp.x=(y*v.z) - (v.y*z);
	vTemp.y=(v.x*z) - (x*v.z);
	vTemp.z=(x*v.y) - (v.x*y);
	vTemp.w=w;
	return vTemp;
}

float Vecteur4D::produitScalaire(Vecteur4D v)
{
	float denominateur = norme()*v.norme();
	float nominateur = (x*v.x) + (y*v.y) + (z*v.z);
	return nominateur;
	//return nominateur/denominateur;
}

Vecteur4D Vecteur4D::interpolation(Vecteur4D v, float f)
{
	Vecteur4D vTemp;
	vTemp.x = (v.x-x)*f + x;
	vTemp.y = (v.y-y)*f + y;
	vTemp.z = (v.z-z)*f + z;
	vTemp.w=w;
	return vTemp;
}

void Vecteur4D::affiche(ostream& os)const
{
	os << "[" << x << "," << y << "," << z << "," << w << "]" << endl; 
}

ostream& operator<<(ostream& os, const Vecteur4D v){
	v.affiche(os);
	return os;
}

void Vecteur4D::operator()(float x, float y, float z, float w){
	this->x=x;
	this->y=y;
	this->z=z;
	this->w=w;
}

float& Vecteur4D::operator[](const int i){
	switch(i){
		case 0:
			return x;
		break;
		case 1:
			return y;
		break;
		case 2:
			return z;
		break;
		case 3:
			return w;
		break;
		default:
			cout << "Error Vecteur4D::operator[], trying to access a non existent var" << endl;
			exit(-1);
		break;
	}
}
