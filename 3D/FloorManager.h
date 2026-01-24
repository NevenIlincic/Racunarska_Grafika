#pragma once
#include "WorldSpaceObject.h"
#include <vector>
//#include "TileHorizontal.h"
//#include "TileVerticalWide.h"
#include "Tile.h"

class FloorManager : public WorldSpaceObject {

public:
    std::vector<Tile*> floorTiles;
  /*  std::vector<Tile> floorTilesVertical;*/
    float stepHeight;

    unsigned int shaderProgram;

    FloorManager();

    ~FloorManager();

    FloorManager(unsigned int shader);

    void draw();

    void checkCameraCollision(Camera& camera);
};