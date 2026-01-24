#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "Tile.h"

class TileVerticalElongated : public Tile {

public:
    std::vector<float> downLeftVertex;
    std::vector<float> normalVector;
    std::vector<float> tileColor;
    float height;
    float length;
    float maxCameraDistance;

    bool isNormalTowardsPositive;  //positive normal --->     negative normal <-----
    unsigned int VAO, VBO;
    unsigned int shaderProgram;

    TileVerticalElongated();

    TileVerticalElongated(unsigned int shader, std::vector<float> _downLeftVertex, std::vector<float> _normalVector, float _height, float _length, std::vector<float> _tileColor, bool _isNormalTowardsPositive);

    void draw();

    void checkCameraCollision(Camera& camera);
};