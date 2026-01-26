#include "Person.h"



Person::Person() {};

Person::Person(float _x, float _y, float _z, float _destinationX, float _destinationY, float _destinationZ) : 
	x(_x), y(_y), z(_z), destinationX(_destinationX), destinationY(_destinationY), destinationZ(_destinationZ),
	isSitting(false), isMovingHorizontaly(false), speed(0.005f), finishedWatching(false), hasExited(false) {

	this->startingX = _x;
	this->startingY = _y;
	this->startingZ = _z;

	this->personModel = Model("Resources/Person/characterlowpoly2.obj");
	this->currentAngle = 180.0f;
};


void Person::move() {
	if (!isSitting) {
		if (!finishedWatching) {
			if (isMovingHorizontaly) {
				moveHorizontalyToSeat();
			}
			else {
				moveVerticalyToSeat();
			}
		}
		else {
			if (isMovingHorizontaly) {
				moveHorizontalyFromSeat();
			}
			else {
				moveVerticalyFromSeat();
			}
		}
	}
}
void Person::moveVerticalyToSeat() {
	if(this->z > destinationZ) {
		this->z -= this->speed;
	}
	if (this->z <= destinationZ) {
		this->z = destinationZ;
		this->isMovingHorizontaly = true;
	}
}

void Person::moveHorizontalyToSeat() {
	if (this->x > destinationX) {
		this->x -= this->speed;
	}
	if (this->x <= destinationX) {
		this->x = destinationX;
		this->isSitting = true;
	}
}

void Person::moveVerticalyFromSeat() {
	if (this->z < startingZ) {
		this->z += this->speed;
	}
	if (this->z >= startingZ) {
		this->z = startingZ;
		this->hasExited = true;
	}
}

void Person::moveHorizontalyFromSeat() {
	if (this->x > startingX) {
		this->x -= this->speed;
	}
	if (this->x <= startingX) {
		this->x = startingX;
		this->isMovingHorizontaly = false;
	}
}
