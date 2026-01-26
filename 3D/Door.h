#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "DoorState.cpp"

#define _USE_MATH_DEFINES
#include <cmath> // za pi
#include <algorithm> // za max()
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "TileVerticalWide.h"
#include "model.hpp"

class Door {
public:
	float x, y, z;
	float r, g, b;
	float doorScaleX, doorScaleY;
	DoorState state;

	unsigned int VAO, VBO;     // zajednički VAO/VBO
	Shader shaderProgram; // shader koji se koristi


	TileVerticalWide* door;
	float doorWidth, doorHeight;

	Model doorModel;
	float currentAngle;


	Door();
	Door(Shader _shader, float _doorWidth, float _doorHeight);

	void draw();
	void openDoor();
	void closeDoor();
};