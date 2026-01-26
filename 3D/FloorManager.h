#pragma once
#include "WorldSpaceObject.h"
#include <vector>
//#include "TileHorizontal.h"
//#include "TileVerticalWide.h"
#include "Tile.h"

class FloorManager : public WorldSpaceObject {

public:
    std::vector<Tile*> floorTiles;
    std::vector<Tile*> floorTilesVertical;
    float stepHeight;

    Shader shaderProgram;

    FloorManager();

    ~FloorManager();

    FloorManager(Shader shader);

    void draw();

    void checkCameraCollision(Camera& camera);
    void checkPersonCollision(Person& person);
};