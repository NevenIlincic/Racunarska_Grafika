#pragma once

#include "Camera.h"
#include "Person.h"
#include <vector>

class Tile {
public:
	std::vector<float> tileColor;

	virtual ~Tile() {}
	virtual void draw() = 0;
	virtual void checkCameraCollision(Camera& camera) = 0;
	virtual void checkPersonCollision(Person& person, float stepHeight) {}
};