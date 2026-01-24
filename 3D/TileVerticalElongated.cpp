#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "TileVerticalElongated.h"

TileVerticalElongated::TileVerticalElongated() {}

TileVerticalElongated::TileVerticalElongated(Shader _shader, std::vector<float> _downLeftVertex, std::vector<float> _normalVector, float _height, float _length, std::vector<float> _tileColor, bool _isNormalTowardsPositive):
    shaderProgram(_shader), downLeftVertex(_downLeftVertex), normalVector(_normalVector), height(_height), length(_length), tileColor(_tileColor), isNormalTowardsPositive(_isNormalTowardsPositive)
{
    this->maxCameraDistance = 0.20f;
    std::vector<float> vertices;
    float x = this->downLeftVertex[0];
    float y = this->downLeftVertex[1];
    float z = this->downLeftVertex[2];
    if (this->isNormalTowardsPositive) { //Posmatrano iz smera pozitivne normale CCW
        vertices = {
            x, y, z, 1.0f, 0.0f, 0.0f, //Donje prednje
            x, y, z - this->length, 1.0f, 0.0f, 0.0f, //Donje zadnje
            x, y + this->height, z - this->length, 1.0f, 0.0f, 0.0f, // Gornje zadnje
            x, y + this->height, z, 1.0f, 0.0f, 0.0f, // Gornje prednje
        };
    }
    else {
        vertices = {
            x, y, z, -1.0f, 0.0f, 0.0f, //Donje prednje
            x, y + this->height, z, -1.0f, 0.0f, 0.0f, //Gornje prednje
            x, y + this->height, z - this->length, -1.0f, 0.0f, 0.0f, // Gornje zadnje
            x, y, z - this->length, -1.0f, 0.0f, 0.0f, // Donje zadnje
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

void TileVerticalElongated::draw() {
    /*glUseProgram(this->shaderProgram);
    glBindVertexArray(VAO);

    unsigned int modelLoc = glGetUniformLocation(this->shaderProgram, "uM");
    unsigned int colorLoc = glGetUniformLocation(this->shaderProgram, "uColor");

    glm::mat4 model = glm::mat4(1.0f);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glUniform3f(colorLoc, this->tileColor[0], this->tileColor[1], this->tileColor[2]);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindVertexArray(0);*/

    //glUseProgram(this->shaderProgram);
    this->shaderProgram.use();
    glBindVertexArray(this->VAO);

   /* unsigned int modelLoc = glGetUniformLocation(this->shaderProgram, "uM");
    unsigned int diffLoc = glGetUniformLocation(this->shaderProgram, "uMaterial.kD");
    unsigned int ambLoc = glGetUniformLocation(this->shaderProgram, "uMaterial.kA");
    unsigned int specLoc = glGetUniformLocation(this->shaderProgram, "uMaterial.kS");
    unsigned int shineLoc = glGetUniformLocation(this->shaderProgram, "uMaterial.shine");*/

    // Postavi boju pločice u materijal
    //glUniform3f(diffLoc, this->tileColor[0], this->tileColor[1], this->tileColor[2]);
    //glUniform3f(ambLoc, this->tileColor[0] * 0.2f, this->tileColor[1] * 0.2f, this->tileColor[2] * 0.2f);
    //glUniform3f(specLoc, 0.2f, 0.2f, 0.2f); // Pločice obično nisu previše sjajne
    //glUniform1f(shineLoc, 32.0f);
    this->shaderProgram.setVec3("uMaterial.kD", this->tileColor[0], this->tileColor[1], this->tileColor[2]);
    this->shaderProgram.setVec3("uMaterial.kA", 0.2f, 0.2f, 0.2f);
    this->shaderProgram.setVec3("uMaterial.kS", 0.2f, 0.2f, 0.2f);
    this->shaderProgram.setFloat("uMaterial.shine", 32.0f);

    // Model matrica (ako ne rotiraš, ostaje identity)
    glm::mat4 model = glm::mat4(1.0f);
    /*glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));*/
    this->shaderProgram.setMat4("uM", model);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);

}

void TileVerticalElongated::checkCameraCollision(Camera& camera) {
    float minY = this->downLeftVertex[1];
    float maxY = this->downLeftVertex[1] + this->height;

    float minZ = this->downLeftVertex[2] - this->length;
    float maxZ = this->downLeftVertex[2];

    if (camera.position.y >= minY && camera.position.y <= maxY &&
        camera.position.z >= minZ && camera.position.z <= maxZ) {

        if (this->normalVector[0] > 0) {
            if (camera.position.x < this->downLeftVertex[0] + this->maxCameraDistance) {
                camera.position.x = this->downLeftVertex[0] + this->maxCameraDistance;
            }
        }
        else {
            if (camera.position.x > this->downLeftVertex[0] - this->maxCameraDistance) {
                camera.position.x = this->downLeftVertex[0] - this->maxCameraDistance;
            }
        }
    }
}