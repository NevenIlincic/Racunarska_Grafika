#pragma once

#include "WorldSpaceObject.h"
#include "Tile.h"
#include "shader.hpp"

class WallsManager: public WorldSpaceObject {
public:
    std::vector<Tile*> wallTiles;

    unsigned int VAO, VBO;
    /*unsigned int shaderProgram;*/
    Shader shaderProgram;

    WallsManager();

    ~WallsManager();

    WallsManager(Shader _shader);

    void draw();

    void checkCameraCollision(Camera& camera);
};