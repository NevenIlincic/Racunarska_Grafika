#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

//#include "Camera.h"
#include "Tile.h"

class TileHorizontal: public Tile {
public:
	std::vector<float> downLeftVertex;
	std::vector<float> normalVector;
	std::vector<float> tileColor;
	float width;
	float length;
	float maxCameraDistance;

	unsigned int VAO, VBO;
	unsigned int shaderProgram;

	TileHorizontal();

	TileHorizontal(unsigned int shader, std::vector<float> _downLeftVertex, std::vector<float> _normalVector, float _width, float _length, std::vector<float> _tileColor);

	void draw();

	void checkCameraCollision(Camera& camera);
};