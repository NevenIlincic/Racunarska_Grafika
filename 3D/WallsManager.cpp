#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "WallsManager.h"
#include "TileHorizontal.h"
#include "TileVerticalWide.h"
#include "TileVerticalElongated.h"


WallsManager::WallsManager(){}

WallsManager::~WallsManager() {
    for (Tile* tile : this->wallTiles) {
        delete tile;
    }
    this->wallTiles.clear();
}

WallsManager::WallsManager(Shader _shader) : shaderProgram(_shader) {
    std::vector<float> wallTileWideColor = { 0.35f, 0.35f, 0.35f };
    std::vector<float> wallTileElongatedColor = { 0.25f, 0.25f, 0.25f };
    std::vector<float> wallTileHorizontalColor = { 0.3f, 0.3f, 0.3f };
    //Roof
    std::vector<float> roofDownLeftVertex = { -1.2f, 2.0f, 1.0f };
    std::vector<float> roofNormalVector = { 0.0f, -1.0f, 0.0f };
    TileHorizontal* roof = new TileHorizontal(this->shaderProgram, roofDownLeftVertex, roofNormalVector, 2.2f, 3.0f, wallTileHorizontalColor, false);
    this->wallTiles.push_back(roof);

    //Back Wall
    std::vector<float> backWallLeftVertex = { -1.2f, 0.0f, -2.0f };
    std::vector<float> backWallNormalVector = { 0.0f, 0.0f, 1.0f };
    TileVerticalWide* backWall = new TileVerticalWide(this->shaderProgram, backWallLeftVertex, backWallNormalVector, 2.2f, 2.0f, wallTileWideColor, true);
    this->wallTiles.push_back(backWall);

    //Front Wall
    std::vector<float> frontWallDownLeftVertex = { -1.2f, 0.0f, 1.0f };
    std::vector<float> frontWallNormalVector = { 0.0f, 0.0f, -1.0f };
    TileVerticalWide* frontWall = new TileVerticalWide(this->shaderProgram, frontWallDownLeftVertex, frontWallNormalVector, 2.2f, 2.0f, wallTileWideColor, false);
    this->wallTiles.push_back(frontWall);

    //Left Wall
    std::vector<float> leftWallDownLeftVertex = { -1.2f, 0.0f, 1.0f };
    std::vector<float> leftWallNormalVector = { 1.0f, 0.0f, 0.0f };
    TileVerticalElongated* leftWall = new TileVerticalElongated(this->shaderProgram, leftWallDownLeftVertex, leftWallNormalVector, 2.0f, 3.0f, wallTileElongatedColor, true);
    this->wallTiles.push_back(leftWall);

    //Right Wall
    std::vector<float> rightWallDownLeftVertex = { 1.0f, 0.0f, 1.0f };
    std::vector<float> rightWallNormalVector = { -1.0f, 0.0f, 0.0f };
    TileVerticalElongated* rightWall = new TileVerticalElongated(this->shaderProgram, rightWallDownLeftVertex, rightWallNormalVector, 2.0f, 3.0f, wallTileElongatedColor, false);
    this->wallTiles.push_back(rightWall);
}

void WallsManager::draw() {
    for (Tile* tile : this->wallTiles) {
        tile->draw();
    }
}

void WallsManager::checkCameraCollision(Camera& camera) {
    for (Tile* tile : this->wallTiles) {
        tile->checkCameraCollision(camera);
    }
}
