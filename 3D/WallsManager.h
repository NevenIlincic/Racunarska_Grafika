#pragma once

#include "WorldSpaceObject.h"
#include "Tile.h"

class WallsManager: public WorldSpaceObject {
public:
    std::vector<Tile*> wallTiles;

    unsigned int VAO, VBO;
    unsigned int shaderProgram;

    WallsManager();

    ~WallsManager();

    WallsManager(unsigned int shader);

    void draw();

    void checkCameraCollision(Camera& camera);
};