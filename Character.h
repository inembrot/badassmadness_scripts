#ifndef __CHARACTER_H
#define __CHARACTER_H
#pragma once
#include "Vecteur4D.h"

/*! \class Character
 * \brief class representing the character (here BubbleBall)
 *
 * Class managing interactions with sphere
*/
class Character
{
private:
	float x;		/*!< x coordinate of the sphere.*/
	float y;		/*!< y coordinate of the sphere.*/
	float z;		/*!< z coordinate of the sphere.*/
	float radius;	/*!< Sphere radius.*/
	Vecteur4D speed;/*!< Speed vector of the sphere.*/
	float weight;	/*!< Speed weight.*/
	float fallTime; /*!< Time ellapsed since the sphere is falling.*/
	bool onFlatFace;/*!< Defines if the sphere is on a flat face.*/
public:
	Character(void);
	~Character(void);
	void setWeight(float w);
	float getWeight()const;
	Vecteur4D getSpeed()const;
	void setSpeed(const Vecteur4D speed);
	void move(float x, float y, float z);
	Vecteur4D getPosition();
	void goTo(float x, float y , float z);
	void fall(float fDT);
	void resetFallTime();
	float getFallTime();
	void reset(bool check);
	bool isFalling();
	float getRadius();
	bool isOnFlatFace();
	void setIsOnFlatFace(bool prop);
	float getRotationX()const;
	float getRotationZ()const;
	void Update();
};

#endif