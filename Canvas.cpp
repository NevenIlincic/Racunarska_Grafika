#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define _USE_MATH_DEFINES
#include <cmath> // za pi
#include <algorithm> // za max()
#include <iostream>
#include "Header/Util.h"

class Canvas {
public:
    float x, y;
    float r, g, b;
    float canvasScaleX, canvasScaleY;
    unsigned int VAO, VBO;     // zajednički VAO/VBO
    unsigned int shaderProgram;

    float movieTime;
    int frameCounter;

    double startTime;
    bool hasMovieStarted;
    bool isMovieFinished;

    Canvas() {};

	Canvas(unsigned int shader): shaderProgram(shader) {

        x = 0.1f;
        y = 0.95f;

        r = 1.0f;
        g = 1.0f;
        b = 1.0f;

        canvasScaleX = 1.4f;
        canvasScaleY = 0.25f;

        movieTime = 20.0f;
        frameCounter = 0;
        hasMovieStarted = false;
        isMovieFinished = false;

        // Kreiranje jednog VAO/VBO za kvadrat
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
        glUniform1f(glGetUniformLocation(shaderProgram, "uSx"), this->canvasScaleX);
        glUniform1f(glGetUniformLocation(shaderProgram, "uSy"), this->canvasScaleY);
        glUniform1f(glGetUniformLocation(shaderProgram, "uAlpha"), 1.0f);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
 
        glBindVertexArray(0);

        checkForMovieFinish();
    }

    void startMovie() {
        this->hasMovieStarted = true;
        this->isMovieFinished = false;
        this->startTime = glfwGetTime();
    }

    void checkForMovieFinish() {
        if (!this->hasMovieStarted || this->isMovieFinished) {
            this->r = 1.0f;
            this->g = 1.0f;
            this->b = 1.0f;
            this->frameCounter = 0;
        }else{
            if (glfwGetTime() - this->startTime > movieTime) {
                this->isMovieFinished = true;
                this->hasMovieStarted = false;
                this->r = 1.0f;
                this->g = 1.0f;
                this->b = 1.0f;
            }
            else {
                this->frameCounter++;
                if (this->frameCounter > 20) {
                    this->r = generate_random_number(0.0f, 1.0f);
                    this->g = generate_random_number(0.0f, 1.0f);
                    this->b = generate_random_number(0.0f, 1.0f);
                    this->frameCounter = 0;
                }
            }
        
        }
    }

    void resetCanvas() {
        this->r = 1.0f;
        this->g = 1.0f;
        this->b = 1.0f;
        frameCounter = 0;
        hasMovieStarted = false;
        isMovieFinished = false;
    }
    
};