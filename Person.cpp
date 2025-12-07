#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define _USE_MATH_DEFINES
#include <cmath> // za pi
#include <algorithm> // za max()
#include <iostream>
#include "Seat.cpp"

class Person {
public:
	float x, y;
	float destinationX, destinationY;
	float startingX, startingY;
	bool isSitting, isMovingHorizontaly;
	float speed;
	bool finishedWatching;
	bool hasExited;


	Person() {};

	Person(float _x, float _y, float _destinationX, float _destinationY) : 
		x(_x), y(_y), destinationX(_destinationX), destinationY(_destinationY), 
		isSitting(false), isMovingHorizontaly(false), speed(0.01f), finishedWatching(false), hasExited(false) {

		this->startingX = _x;
		this->startingY = _y;
	};


	void move() {
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
private:
	void moveVerticalyToSeat() {
		if(this->y > destinationY) {
			this->y -= this->speed;
		}
		if (this->y <= destinationY) {
			this->y = destinationY;
			this->isMovingHorizontaly = true;
		}
	}

	void moveHorizontalyToSeat() {
		if (this->x < destinationX) {
			this->x += this->speed;
		}
		if (this->x >= destinationX) {
			this->x = destinationX;
			this->isSitting = true;
		}
	}

	void moveVerticalyFromSeat() {
		if (this->y < startingY) {
			this->y += this->speed;
		}
		if (this->y >= startingY) {
			this->y = startingY;
			this->hasExited = true;
		}
	}

	void moveHorizontalyFromSeat() {
		if (this->x > startingX) {
			this->x -= this->speed;
		}
		if (this->x <= startingX) {
			this->x = startingX;
			this->isMovingHorizontaly = false;
		}
	}
};