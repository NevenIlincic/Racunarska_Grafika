#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#include "TileHorizontal.h"

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
            x, y, z, 0.0f, 1.0f, 0.0f, //Donje levo teme
            x + this->width, y, z, 0.0f, 1.0f, 0.0f, //Donje desno 
            x + this->width, y, z - this->length, 0.0f, 1.0f, 0.0f, // Gornje desno
            x, y, z - this->length, 0.0f, 1.0f, 0.0f, // Gornje levo
        };
    }
    else {
        vertices = {
            x, y, z, 0.0f, 1.0f, 0.0f, //Levo prednje
            x, y, z - this->length, 0.0f, 1.0f, 0.0f, //Levo zadnje 
            x + this->width, y, z - this->length, 0.0f, 1.0f, 0.0f, // Desno zadnje
            x + this->width, y, z, 0.0f, 1.0f, 0.0f, // Desno prednje
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

void TileHorizontal::draw() {
   /* glUseProgram(this->shaderProgram);
    glBindVertexArray(this->VAO);

    unsigned int modelLoc = glGetUniformLocation(this->shaderProgram, "uM");
    unsigned int diffLoc = glGetUniformLocation(this->shaderProgram, "uMaterial.kD");
    unsigned int ambLoc = glGetUniformLocation(this->shaderProgram, "uMaterial.kA");
    unsigned int specLoc = glGetUniformLocation(this->shaderProgram, "uMaterial.kS");

    glUniform3f(diffLoc, this->tileColor[0], this->tileColor[1], this->tileColor[2]);
    glUniform3f(ambLoc, this->tileColor[0] * 0.2f, this->tileColor[1] * 0.2f, this->tileColor[2] * 0.2f); // Ambijent je tamnija verzija boje
    glUniform3f(specLoc, 1.0f, 1.0f, 1.0f);

    glm::mat4 model = glm::mat4(1.0f);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

   // glUniform3f(colorLoc, this->tileColor[0], this->tileColor[1], this->tileColor[2]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindVertexArray(0);*/
    /*glUseProgram(this->shaderProgram);*/
    this->shaderProgram.use();
    glBindVertexArray(this->VAO);

    /*unsigned int modelLoc = glGetUniformLocation(this->shaderProgram, "uM");
    unsigned int diffLoc = glGetUniformLocation(this->shaderProgram, "uMaterial.kD");
    unsigned int ambLoc = glGetUniformLocation(this->shaderProgram, "uMaterial.kA");
    unsigned int specLoc = glGetUniformLocation(this->shaderProgram, "uMaterial.kS");
    unsigned int shineLoc = glGetUniformLocation(this->shaderProgram, "uMaterial.shine);"*/


    // Postavi boju pločice u materijal
    //glUniform3f(diffLoc, this->tileColor[0], this->tileColor[1], this->tileColor[2]);
    //glUniform3f(ambLoc, this->tileColor[0] * 0.2f, this->tileColor[1] * 0.2f, this->tileColor[2] * 0.2f);
    //glUniform3f(diffLoc, 0.8f, 0.8f, 0.8f); // Svetlo siva (difuzna)
    //glUniform3f(ambLoc, 0.3f, 0.3f, 0.3f);
    //glUniform3f(specLoc, 0.2f, 0.2f, 0.2f); // Pločice obično nisu previše sjajne
    //glUniform1f(shineLoc, 32.0f);
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
