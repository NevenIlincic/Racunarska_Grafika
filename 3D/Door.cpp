#include <vector>
#include "Door.h"

Door::Door() {};

Door::Door(Shader _shader, float _doorWidth, float _doorHeight): shaderProgram(_shader), doorWidth(_doorWidth), doorHeight(_doorHeight) {

    this->state = DoorState::Closed;

    this->x = 0.75f;
    this->y = 0.0f;
    this->z = 0.99f;

    this->r = 0.4f;
    this->g = 0.26f;
    this->b = 0.13f;

    doorScaleX = 0.25f;
    doorScaleY = 0.15f;

    std::vector<float> wallTileWideColor = { 0.0f, 1.0f, 1.0f };
    //Front Wall
    std::vector<float> frontWallDownLeftVertex = { 0.5f, 0.0f, 0.99f };
    std::vector<float> frontWallNormalVector = { 0.0f, 0.0f, -1.0f };

    bool isNormalTowardsPositive = false;
    this->door = new TileVerticalWide(this->shaderProgram, frontWallDownLeftVertex, frontWallNormalVector, this->doorWidth, this->doorHeight, wallTileWideColor, false);
        
    this->doorModel = Model("Resources/Door/door.obj");
    this->currentAngle = 90.0f;
}

void Door::draw() {
    this->shaderProgram.use();

    this->shaderProgram.setBool("useTex", false);

    glm::mat4 modelMat = glm::mat4(1.0f);

    modelMat = glm::translate(modelMat, glm::vec3(this->x, this->y, this->z));
    modelMat = glm::rotate(modelMat, glm::radians(this->currentAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMat = glm::scale(modelMat, glm::vec3(0.001f, 0.001f, 0.001f));

    this->shaderProgram.setMat4("uM", modelMat);
    /*glDisable(GL_CULL_FACE);*/
    this->doorModel.Draw(this->shaderProgram);
    /*glEnable(GL_CULL_FACE);*/

    this->shaderProgram.setBool("useTex", false);

}

void Door::openDoor() {
    this->state = DoorState::Opened;
    this->currentAngle = 0.0f;
    /*this->x = -0.85f;
    this->doorScaleX = 0.05f;
    this->doorScaleY = 0.6f;*/
}

void Door::closeDoor() {
    this->state = DoorState::Closed;
    this->currentAngle = 90.0f;
}

