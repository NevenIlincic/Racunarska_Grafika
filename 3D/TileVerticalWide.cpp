#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "TileVerticalWide.h"



TileVerticalWide::TileVerticalWide() {}

TileVerticalWide::TileVerticalWide(Shader _shader, std::vector<float> _downLeftVertex, std::vector<float> _normalVector, float _width, float _height, std::vector<float> _tileColor, bool _isNormalTowardsPositive, bool _isTileCanvas) :
    shaderProgram(_shader), downLeftVertex(_downLeftVertex), normalVector(_normalVector), width(_width), height(_height), tileColor(_tileColor), isNormalTowardsPositive(_isNormalTowardsPositive), isTileCanvas(_isTileCanvas)
{
    this->maxCameraDistance = 0.20f;

    std::vector<float> vertices;
    float x = this->downLeftVertex[0];
    float y = this->downLeftVertex[1];
    float z = this->downLeftVertex[2];

    this->drawTexture = false;

    if (this->isNormalTowardsPositive) {
        vertices = {
            x, y, z, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, //Donje levo teme
            x + this->width, y, z, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, //Donje desno 
            x + this->width, y + this->height, z, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Gornje desno
            x, y + this->height, z, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f // Gornje levo
        };
    }
    else {
        vertices = { //Gledano u pravcu normale
            x, y, z, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,   ///Donje desno
            x, y + this->height, z, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,///Gornje desno
            x + this->width, y + this->height, z, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,/// Gornje levo
            x + this->width, y, z, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f/// Donje levo
        };
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Pozicije
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Normale (ako tvoj shader koristi location 3 za normale kao u SeatsManager-u)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Tekstura
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    preprocessTexture(this->texture, "Resources/Wall/wall.jpg", true);

}

void TileVerticalWide::draw() {
    this->shaderProgram.use();
    glBindVertexArray(this->VAO);
    if (!this->isTileCanvas) {
        this->shaderProgram.setBool("useTex", true);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->texture);
    }

    this->shaderProgram.setVec3("uMaterial.kD", this->tileColor[0] * 0.9f, this->tileColor[1] * 0.9f, this->tileColor[2] * 0.9f);
    this->shaderProgram.setVec3("uMaterial.kA", 0.8f, 0.8f, 0.8f);
    this->shaderProgram.setVec3("uMaterial.kS", 0.4f, 0.4f, 0.4f);
    this->shaderProgram.setFloat("uMaterial.shine", 64.0f);

    glm::mat4 model = glm::mat4(1.0f);
    this->shaderProgram.setMat4("uM", model);
    

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
    this->shaderProgram.setBool("useTex", false);
        
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
    else {
        ///SPUSTANJE
        if (person.z > this->downLeftVertex[2] && person.y + person.yOffset > maxY - 0.01f) {
            person.y = person.y - stepHeight;
        }
    }
    

}