#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "TileVerticalElongated.h"

TileVerticalElongated::TileVerticalElongated() {}

TileVerticalElongated::TileVerticalElongated(unsigned int shader, std::vector<float> _downLeftVertex, std::vector<float> _normalVector, float _height, float _length, std::vector<float> _tileColor):
    shaderProgram(shader), downLeftVertex(_downLeftVertex), normalVector(_normalVector), height(_height), length(_length), tileColor(_tileColor)
{
    this->maxCameraDistance = 0.20f;

    float vertices[] = {
        downLeftVertex[0], downLeftVertex[1], downLeftVertex[2], this->normalVector[0], this->normalVector[1], this->normalVector[2], //Donje prednje
        downLeftVertex[0], downLeftVertex[1], downLeftVertex[2] - this->length, this->normalVector[0], this->normalVector[1], this->normalVector[2], //Donje zadnje
        downLeftVertex[0], downLeftVertex[1] + this->height, downLeftVertex[2] - this->length, this->normalVector[0], this->normalVector[1], this->normalVector[2], // Gornje zadnje
        downLeftVertex[0], downLeftVertex[1] + this->height, downLeftVertex[2], this->normalVector[0], this->normalVector[1], this->normalVector[2], // Gornje prednje
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Pozicije
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Normale (ako tvoj shader koristi location 3 za normale kao u SeatsManager-u)
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(3);

}

void TileVerticalElongated::draw() {
    glUseProgram(this->shaderProgram);
    glBindVertexArray(VAO);

    unsigned int modelLoc = glGetUniformLocation(this->shaderProgram, "uM");
    unsigned int colorLoc = glGetUniformLocation(this->shaderProgram, "uColor");

    glm::mat4 model = glm::mat4(1.0f);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glUniform3f(colorLoc, this->tileColor[0], this->tileColor[1], this->tileColor[2]);

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