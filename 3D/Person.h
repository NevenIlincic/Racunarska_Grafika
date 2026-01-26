#pragma once
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define _USE_MATH_DEFINES
#include <cmath> // za pi
#include <algorithm> // za max()
#include <iostream>
#include "Seat.h"
#include "model.hpp"

class Person {
public:
	float x, y, z;
	float destinationX, destinationY, destinationZ;
	float startingX, startingY, startingZ;
	bool isSitting, isMovingHorizontaly;
	float speed;
	bool finishedWatching;
	bool hasExited;

	Model personModel;
	float currentAngle;

	Person();
	Person(float _x, float _y, float _z, float _destinationX, float _destinationY, float _destinationZ);
	
	void move();

private:
	void moveVerticalyToSeat();
	void moveHorizontalyToSeat();
	void moveVerticalyFromSeat();
	void moveHorizontalyFromSeat();
};