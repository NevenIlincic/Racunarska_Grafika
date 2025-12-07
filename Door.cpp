#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "DoorState.cpp"

#define _USE_MATH_DEFINES
#include <cmath> // za pi
#include <algorithm> // za max()
#include <iostream>

class Door {
public:
	float x, y;
	float r, g, b;
    float doorScaleX, doorScaleY;
    DoorState state;

	unsigned int VAO, VBO;     // zajednički VAO/VBO
	unsigned int shaderProgram; // shader koji se koristi



	Door() {};

    Door(unsigned int shader): shaderProgram(shader) {

        state = DoorState::Closed;
        x = -1.0f;
        y = 1.0f;
        r = 0.4f;
        g = 0.26f;
        b = 0.13f;

        doorScaleX = 0.25f;
        doorScaleY = 0.15f;
 
        float quadVertices[] = {
            -0.5f, -0.5f,
             0.5f, -0.5f,
             0.5f,  0.25f,
            -0.5f,  0.25f
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }

    void draw() {
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        glUniform3f(glGetUniformLocation(shaderProgram, "uColor"), this->r, this->g, this->b);

        glUniform1f(glGetUniformLocation(shaderProgram, "uX"), this->x);
        glUniform1f(glGetUniformLocation(shaderProgram, "uY"), this->y);
        glUniform1f(glGetUniformLocation(shaderProgram, "uSx"), this->doorScaleX);
        glUniform1f(glGetUniformLocation(shaderProgram, "uSy"), this->doorScaleY);
        glUniform1f(glGetUniformLocation(shaderProgram, "uAlpha"), 1.0f);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glBindVertexArray(0);
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