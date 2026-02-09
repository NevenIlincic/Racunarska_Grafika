#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#include "TileHorizontal.h"
#include "Header/Util.h"

TileHorizontal::TileHorizontal(){}

TileHorizontal::TileHorizontal(Shader _shader, std::vector<float> _downLeftVertex, std::vector<float> _normalVector, float _width, float _length, std::vector<float> _tileColor, bool _isNormalTowardsPositive):
    shaderProgram(_shader), downLeftVertex(_downLeftVertex), normalVector(_normalVector), width(_width), length(_length), tileColor(_tileColor), isNormalTowardsPositive(_isNormalTowardsPositive)
{
    this->maxCameraDistance = 0.15f;
    
    float x = this->downLeftVertex[0];
    float y = this->downLeftVertex[1];
    float z = this->downLeftVertex[2];

    std::vector<float> vertices;
    if (this->isNormalTowardsPositive) {
        vertices = {
            x, y, z, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, //Donje levo teme
            x + this->width, y, z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, //Donje desno 
            x + this->width, y, z - this->length, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // Gornje desno
            x, y, z - this->length, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f// Gornje levo
        };
    }
    else {
        vertices = {
            x, y, z, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  //Levo prednje
            x, y, z - this->length, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, //Levo zadnje 
            x + this->width, y, z - this->length, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // Desno zadnje
            x + this->width, y, z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f// Desno prednje
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

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    preprocessTexture(this->texture, "Resources/Wall/ground.jpg", true);

}

void TileHorizontal::draw() {
    this->shaderProgram.use();
    glBindVertexArray(this->VAO);

    this->shaderProgram.setBool("useTex", false); ///PROMENI NA TRUE
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture);

    this->shaderProgram.setVec3("uMaterial.kD", this->tileColor[0] * 0.5f, this->tileColor[1] * 0.5f, this->tileColor[2] * 0.5f);
    this->shaderProgram.setVec3("uMaterial.kA", 0.8f, 0.8f, 0.8f);
    this->shaderProgram.setVec3("uMaterial.kS", 0.4f, 0.4f, 0.4f);
    this->shaderProgram.setFloat("uMaterial.shine", 32.0f);

    // Model matrica (ako ne rotiraš, ostaje identity)
    glm::mat4 model = glm::mat4(1.0f);
    this->shaderProgram.setMat4("uM", model);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
    this->shaderProgram.setBool("useTex", false);
}

void TileHorizontal::checkCameraCollision(Camera& camera) {
    float minX = this->downLeftVertex[0];
    float maxX = this->downLeftVertex[0] + this->width;

    float minZ = this->downLeftVertex[2] - this->length;
    float maxZ = this->downLeftVertex[2];

    if (camera.position.x >= minX && camera.position.x <= maxX &&
        camera.position.z >= minZ && camera.position.z <= maxZ) {
        
        if (this->normalVector[1] > 0) {
            if (camera.position.y < this->downLeftVertex[1] + this->maxCameraDistance) {
                camera.position.y = this->downLeftVertex[1] + this->maxCameraDistance;
            }
        }
        else {
            if (camera.position.y > this->downLeftVertex[1] - this->maxCameraDistance) {
                camera.position.y = this->downLeftVertex[1] - this->maxCameraDistance;
            }
        }
    }
}

void TileHorizontal::checkPersonCollision(Person& person){}
