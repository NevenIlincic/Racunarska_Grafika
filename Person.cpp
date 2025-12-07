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

	Person() {};

	Person(float _x, float _y, float _destinationX, float _destinationY) : 
		x(_x), y(_y), destinationX(_destinationX), destinationY(_destinationY) {};
};