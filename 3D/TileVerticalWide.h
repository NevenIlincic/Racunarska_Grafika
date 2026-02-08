#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "Tile.h"
#include "Person.h"
#include "Header/Util.h"

class TileVerticalWide : public Tile {

public:
    std::vector<float> downLeftVertex;
    std::vector<float> tileColor;
    std::vector<float> normalVector;
    float width;
    float height;
    float maxCameraDistance;

    bool isNormalTowardsPositive;
    bool drawTexture;
    bool isTileCanvas;

    unsigned int VAO, VBO;
    Shader shaderProgram;
    unsigned int texture;

    TileVerticalWide();

    TileVerticalWide(Shader _shader, std::vector<float> _downLeftVertex, std::vector<float> _normalVector, float _width, float _height, std::vector<float> color, bool _isNormalTowardsPositive, bool isTileCanvas);

    void draw();

    void checkCameraCollision(Camera& camera);

    void checkPersonCollision(Person& person, float stepHeight);
};