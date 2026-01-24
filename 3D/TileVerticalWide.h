#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "Tile.h"

class TileVerticalWide : public Tile {

public:
    std::vector<float> downLeftVertex;
    float width;
    float height;
    float maxCameraDistance;
    std::vector<float> tileColor;

    std::vector<float> normalVector;

    unsigned int VAO, VBO;
    unsigned int shaderProgram;

    TileVerticalWide();

    TileVerticalWide(unsigned int shader, std::vector<float> _downLeftVertex, std::vector<float> _normalVector, float _width, float _height, std::vector<float> color);

    void draw();

    void checkCameraCollision(Camera& camera);
};