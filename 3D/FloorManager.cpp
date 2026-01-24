#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "FloorManager.h"
#include "Camera.h"
#include "TileHorizontal.h"
#include "TileVerticalWide.h"


FloorManager::FloorManager(){}

FloorManager::~FloorManager() {
    for (Tile* tile : this->floorTiles) {
        delete tile;
    }
    this->floorTiles.clear();
}

FloorManager::FloorManager(unsigned int shader) : shaderProgram(shader) {
    this->stepHeight = 0.1f;
    std::vector<float> downLeftVertex = { -1.2f, 0.0f, 1.0f };
    std::vector<float> roofDownLeftVertex = { -1.2f, 3.0f, 1.0f };

    float floorTileWidth = 2.2f;
    float tileLength = 0.3f;

    //Floor tiles
    std::vector<float> normalVectorFloor = { 0.0f, 1.0f, 0.0f };
    std::vector<float> horizontalTileColor = { 0.2f, 0.2f, 0.2f };
    TileHorizontal* floorTile = new TileHorizontal(this->shaderProgram, downLeftVertex, normalVectorFloor, floorTileWidth, 1.15f, horizontalTileColor);   //0.2f za Z razlika
    this->floorTiles.push_back(floorTile);
  
 
    float currentZ = 1.0f - 1.15f;
    for (int i = 1; i < 6; i++) {
        std::vector<float> currentPos = { -1.2f, this->stepHeight * i, currentZ };
        if (i != 5) {
            TileHorizontal* step = new TileHorizontal(this->shaderProgram, currentPos, normalVectorFloor, floorTileWidth, tileLength, horizontalTileColor);
            this->floorTiles.push_back(step);
        }
        else {
            TileHorizontal* step = new TileHorizontal(this->shaderProgram, currentPos, normalVectorFloor, floorTileWidth, tileLength + 0.4f, horizontalTileColor);
            this->floorTiles.push_back(step);
        }

        std::vector<float> currentPosVertical = { -1.2f, this->stepHeight * (i-1), currentZ};
        std::vector<float> verticalTileNormal = { 0.0f, 0.0f, 1.0f };
        std::vector<float> verticalTileColor = { 0.3f, 0.3f, 0.3f };
        TileVerticalWide* verticalTile = new TileVerticalWide(this->shaderProgram, currentPosVertical, verticalTileNormal, floorTileWidth, this->stepHeight, verticalTileColor);
        this->floorTiles.push_back(verticalTile);
        currentZ -= tileLength;
    }
      
}

void FloorManager::draw() {
    for (Tile* tile : this->floorTiles) {
        tile->draw();
    }
    /*for (TileHorizontal floorTileHorizontal : this->floorTilesHorizontal)
    {
        floorTileHorizontal.draw();
    }
    for (TileVerticalWide floorTileVertical : this->floorTilesVertical) {
        floorTileVertical.draw();
    }*/
}

void FloorManager::checkCameraCollision(Camera& camera) {
    for (Tile* tile : this->floorTiles) {
        tile->checkCameraCollision(camera);
    }
}
