#include "Character.h"

/*!\brief Character default constructor*/
Character::Character(void)
{
	this->x = 0.f;
	this->y = 9.f;
	this->z = 0.f;
	this->radius = 0.3f;
	this->speed =  Vecteur4D(0,0,0,0);
	this->weight = 0.03f;
	this->fallTime = 0.f;
	this->onFlatFace = false;
}

/*!\brief Character destructor*/
Character::~Character(void)
{
}

/*!
	* \brief A method that translate the sphere from the values x, y and z.
	*
	* \param x : translation from the value x.
	* \param y : translation from the value t.
	* \param z : translation from the value z.
*/
void Character::move(float x, float y, float z) {
	this->x += x;
	this->y += y;
	this->z += z;
}

/*!
	* \brief Position Getter.
	*
	* Method returning the position vector.
	* \return position vector.
*/
Vecteur4D Character::getPosition() {
	Vecteur4D v(x, y, z, 0.f);
	return v;
}

/*!
	* \brief A method that translate the sphere to the values x, y and z.
	*
	* \param x : translation to the value x.
	* \param y : translation to the value t.
	* \param z : translation to the value z.
*/
void Character::goTo(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

float Character::getRadius() {
	return this->radius;
}

/*!
	* \brief Speed Setter.
	*
	* Method for setting a new value for the speed vector.
	* \param speed : the speed vector value.
*/
void Character::setSpeed(Vecteur4D speed) {
	this->speed = speed;
}

/*!
	* \brief Weight Setter.
	*
	* Method for setting a new value for the weight, that will be applied to the sphere speed.
	* \param w : the new weight.
*/
void Character::setWeight(float s) {
	this->weight = s;
}

/*!
	* \brief Speed Getter.
	*
	* Method returning the value of the speed vector.
	* \return speed vector.
*/
Vecteur4D Character::getSpeed()const {
	return this->speed;
}

/*!
	* \brief Weight Getter.
	*
	* Method returning the value of the speed weight.
	* \return speed weight value.
*/
float Character::getWeight()const {
	return this->weight;
}

void Character::Update() {
	float decreasingWeight = 1.f/700;
	//Applying the speed weight
	this->x += speed[0]*weight;
	this->y += speed[1]*weight;
	this->z += speed[2]*weight;
	//Decreasing the speed in time
	if(speed[0] > 0)
		speed[0] -= decreasingWeight;
	if(speed[2] > 0)
		speed[2] -= decreasingWeight;
	if(speed[0] < 0)
		speed[0] += decreasingWeight;
	if(speed[2] < 0){
		speed[2] += decreasingWeight;
	}
}

/*!
	* \brief Makes the sphere fall.
	*
	* Method making the sphere falling, and updating the fallTime variable.
	* \param fDT : the time ellapsed since the last call.
*/
void Character::fall(float fDT) {
	fallTime += fDT;
	this->y -=0.05f;
}

//0.017 When ball is on inclined surface
/*!
	* \brief Tells if the sphere is falling.
	*
	* \return true if the sphere is falling,
	* false otherwise.
*/
bool Character::isFalling() {
	if (fallTime > 0.020f) {
		return true;
	}
	return false;
}

/*!
	* \brief Sets the fall time to 0.
*/
void Character::resetFallTime() {
	fallTime = 0.f;
}

/*!
	* \brief Reset the sphere attributes.
	*
	* Method resetting the sphere attributes : position, fallTime and speed.
	* \param check : a boolean that defines if the checkpoint has been reached.
*/
void Character::reset(bool check) {
	fallTime = 0.f;
	if (!check) {		//Sets the sphere to the beginning
		x = 0.f;
		y = 9.f;
		z = 0.f;
	} else {			//Set the sphere to the checkpoint
		x = 8;
		y = 5.8;
		z = 35.0;
		//x = -2.5;
		//y = -4;
		//z = 66.0;
	}
	speed[0] = 0.f;
	speed[1] = 0.f;
	speed[2] = 0.f;
}

/*!
	* \brief FallTime Getter.
	*
	* Method returning the time ellapsed since the sphere began to fall.
	* \return the falling time.
*/
float Character::getFallTime() {
	return fallTime;
}

/*!
	* \brief Tells if the sphere is on a flat face.
	* \return true if the sphere is on a flat face,
	* false otherwise.
*/
bool Character::isOnFlatFace() {
	return onFlatFace;
}

/*!
	* \brief onFLatFace setter.
	* \param prop : the property that tells if the sphere is on a flat face.
*/
void Character::setIsOnFlatFace(bool prop) {
	onFlatFace = prop;
}

/*!
	* \brief Returns the x rotation of the sphere.
	* \return the x rotation.
*/
float Character::getRotationX()const{
	//Applying a weight, so that the sphere seems to be rolling.
	return this->x * 50;
}
	
/*!
	* \brief Returns the z rotation of the sphere.
	* \return the z rotation.
*/
float Character::getRotationZ()const{
	return this->z * 50;
}