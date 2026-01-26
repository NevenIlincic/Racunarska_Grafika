#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "TileVerticalWide.h"



TileVerticalWide::TileVerticalWide() {}

TileVerticalWide::TileVerticalWide(Shader _shader, std::vector<float> _downLeftVertex, std::vector<float> _normalVector, float _width, float _height, std::vector<float> _tileColor, bool _isNormalTowardsPositive) :
    shaderProgram(_shader), downLeftVertex(_downLeftVertex), normalVector(_normalVector), width(_width), height(_height), tileColor(_tileColor), isNormalTowardsPositive(_isNormalTowardsPositive)
{
    this->maxCameraDistance = 0.20f;

    std::vector<float> vertices;
    float x = this->downLeftVertex[0];
    float y = this->downLeftVertex[1];
    float z = this->downLeftVertex[2];

    if (this->isNormalTowardsPositive) {
        vertices = {
            x, y, z, 0.0f, 0.0f, 1.0f, //Donje levo teme
            x + this->width, y, z, 0.0f, 0.0f, 1.0f, //Donje desno 
            x + this->width, y + this->height, z, 0.0f, 0.0f, 1.0f, // Gornje desno
            x, y + this->height, z, 0.0f, 0.0f, 1.0f, // Gornje levo
        };
    }
    else {
        vertices = { //Gledano u pravcu normale
            x, y, z, 0.0f, 0.0f, -1.0f,   ///Donje desno
            x, y + this->height, z, 0.0f, 0.0f, -1.0f,///Gornje desno
            x + this->width, y + this->height, z, 0.0f, 0.0f, -1.0f,/// Gornje levo
            x + this->width, y, z, 0.0f, 0.0f, -1.0f/// Donje levo
        };
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Pozicije
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Normale (ako tvoj shader koristi location 3 za normale kao u SeatsManager-u)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

}

void TileVerticalWide::draw() {
    this->shaderProgram.use();
    glBindVertexArray(this->VAO);

    this->shaderProgram.setVec3("uMaterial.kD", this->tileColor[0], this->tileColor[1], this->tileColor[2]);
    this->shaderProgram.setVec3("uMaterial.kA", 0.3f, 0.3f, 0.3f);
    this->shaderProgram.setVec3("uMaterial.kS", 0.2f, 0.2f, 0.2f);
    this->shaderProgram.setFloat("uMaterial.shine", 32.0f);

    // Model matrica (ako ne rotiraš, ostaje identity)
    glm::mat4 model = glm::mat4(1.0f);
    /*glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));*/
    this->shaderProgram.setMat4("uM", model);
    

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
        
}

void TileVerticalWide::checkCameraCollision(Camera& camera) {
    float minX = this->downLeftVertex[0];
    float maxX = this->downLeftVertex[0] + this->width;

    float minY = this->downLeftVertex[1];
    float maxY = this->downLeftVertex[1] + this->height;

    if (camera.position.x >= minX && camera.position.x <= maxX &&
        camera.position.y >= minY && camera.position.y <= maxY) {

        if (this->normalVector[2] > 0) {
            if (camera.position.z < this->downLeftVertex[2] + this->maxCameraDistance) {
                camera.position.z = this->downLeftVertex[2] + this->maxCameraDistance;
            }
        }
        else {
            if (camera.position.z > this->downLeftVertex[2] - this->maxCameraDistance) {
                camera.position.z = this->downLeftVertex[2] - this->maxCameraDistance;
            }
        }
    }
}

void TileVerticalWide::checkPersonCollision(Person& person, float stepHeight) {
    float minX = this->downLeftVertex[0];
    float maxX = this->downLeftVertex[0] + this->width;

    float minY = this->downLeftVertex[1];
    float maxY = this->downLeftVertex[1] + this->height;

    if (person.x >= minX && person.x <= maxX &&
        person.y >= minY && person.y <= maxY) {

        if (this->normalVector[2] > 0) {
            if (person.z < this->downLeftVertex[2] + 0.35f) {
                person.y = person.y + stepHeight;
            }
        }
        else {
            if (person.z > this->downLeftVertex[2] - this->maxCameraDistance) {
                person.z = this->downLeftVertex[2] - this->maxCameraDistance;
            }
        }
    }
}