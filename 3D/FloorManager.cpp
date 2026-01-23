#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include "TileHorizontal.cpp"
#include "TileVertical.cpp"

class FloorManager {

public:
    std::vector<TileHorizontal> floorTilesHorizontal;
    std::vector<TileVertical> floorTilesVertical;
    float stepHeight;

    unsigned int shaderProgram;

    FloorManager(){}

    FloorManager(unsigned int shader) : shaderProgram(shader) {
        stepHeight = 0.1f;
        std::vector<float> downLeftVertex = { -1.2f, 0.0f, 1.0f };
        std::vector<float> roofDownLeftVertex = { -1.2f, 3.0f, 1.0f };

        float floorTileWidth = 2.2f;
        float tileLength = 0.3f;

        //Floor tiles

        TileHorizontal floorTile = TileHorizontal(shader, downLeftVertex, floorTileWidth, 1.15f);   //0.2f za Z razlika
        TileHorizontal roof = TileHorizontal(shader, roofDownLeftVertex, 2.2f, 3.0f);
        floorTilesHorizontal.push_back(floorTile);
 
        float currentZ = 1.0f - 1.15f;
        for (int i = 1; i < 6; i++) {
            std::vector<float> currentPos = { -1.2f, this->stepHeight * i, currentZ };

            TileHorizontal step = TileHorizontal(shader, currentPos, floorTileWidth, tileLength);
            floorTilesHorizontal.push_back(step);

            std::vector<float> currentPosVertical = { -1.2f, this->stepHeight * (i-1), currentZ};
            std::vector<float> verticalTileNormal = { 0.0f, 0.0f, -1.0f };
            TileVertical verticalTile = TileVertical(shader, currentPosVertical, verticalTileNormal, floorTileWidth, this->stepHeight);
            floorTilesVertical.push_back(verticalTile);
            currentZ -= tileLength;
        }
      
    }

    void draw() {
        for (TileHorizontal floorTileHorizontal : floorTilesHorizontal)
        {
            floorTileHorizontal.draw();
        }
        for (TileVertical floorTileVertical : floorTilesVertical) {
            floorTileVertical.draw();
        }
    }
};