#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "DoorState.cpp"

#define _USE_MATH_DEFINES
#include <cmath> // za pi
#include <algorithm> // za max()
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "TileVerticalWide.h"

class Door {
public:
	float x, y, z;
	float r, g, b;
    float doorScaleX, doorScaleY;
    DoorState state;

	unsigned int VAO, VBO;     // zajednički VAO/VBO
	unsigned int shaderProgram; // shader koji se koristi

    
    TileVerticalWide* door;
    float doorWidth, doorHeight;


	Door() {};

    Door(unsigned int shader, float _doorWidth, float _doorHeight): shaderProgram(shader), doorWidth(_doorWidth), doorHeight(_doorHeight) {

        state = DoorState::Closed;
        x = -1.0f;
        y = 1.0f;
        r = 0.4f;
        g = 0.26f;
        b = 0.13f;

        doorScaleX = 0.25f;
        doorScaleY = 0.15f;

        std::vector<float> wallTileWideColor = { 0.0f, 1.0f, 1.0f };
        //Front Wall
        std::vector<float> frontWallDownLeftVertex = { 0.5f, 0.0f, 0.99f };
        std::vector<float> frontWallNormalVector = { 0.0f, 0.0f, -1.0f };

        bool isNormalTowardsPositive = false;
        this->door = new TileVerticalWide(this->shaderProgram, frontWallDownLeftVertex, frontWallNormalVector, this->doorWidth, this->doorHeight, wallTileWideColor, false);
 
        //float vertices[] = {
        //    -0.5f, -0.5f,
        //     0.5f, -0.5f,
        //     0.5f,  0.25f,
        //    -0.5f,  0.25f
        //};

        //glGenVertexArrays(1, &VAO);
        //glGenBuffers(1, &VBO);

        //glBindVertexArray(VAO);
        //glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        //// Pozicije
        //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        //glEnableVertexAttribArray(0);
        //// Normale (ako tvoj shader koristi location 3 za normale kao u SeatsManager-u)
        //glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        //glEnableVertexAttribArray(3);
    }

    void draw() {
        unsigned int modelLoc = glGetUniformLocation(this->shaderProgram, "uM");

        float ugao = glm::radians(45.0f); // Rotacija od 45 stepeni
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, ugao, glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        this->door->draw();
        model = glm::mat4(1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        

        
       /* glUseProgram(this->shaderProgram);
        glBindVertexArray(VAO);

        unsigned int modelLoc = glGetUniformLocation(this->shaderProgram, "uM");
        unsigned int colorLoc = glGetUniformLocation(this->shaderProgram, "uColor");

        glm::mat4 model = glm::mat4(1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glUniform3f(colorLoc, this->r, this->g, this->b);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glBindVertexArray(0);*/
    }

    void openDoor() {
        this->state = DoorState::Opened;
        this->x = -0.85f;
        this->doorScaleX = 0.05f;
        this->doorScaleY = 0.6f;
    }

    void closeDoor() {
        this->state = DoorState::Closed;
        this->x = -1.0f;
        this->doorScaleX = 0.25f;
        this->doorScaleY = 0.15f;
    }

};