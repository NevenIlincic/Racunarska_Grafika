#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define _USE_MATH_DEFINES
#include <cmath> // za pi
#include <algorithm> // za max()
#include <iostream>

class DarkRect {
public:
	float x, y;
	float r, g, b;
	float canvasScaleX, canvasScaleY;
    float alpha;
	unsigned int VAO, VBO;     // zajednički VAO/VBO
	unsigned int shaderProgram;

	DarkRect() {};

	DarkRect(unsigned int shader) : shaderProgram(shader) {
        x = -0.0f;
        y = 0.8f;

        r = 0.3f;
        g = 0.3f;
        b = 0.3f;

        canvasScaleX = 1.5f;
        canvasScaleY = 1.5f;

        alpha = 0.5f;

        // Kreiranje jednog VAO/VBO za kvadrat
        float quadVertices[] = {
            -1.0f, -1.0f, // Donji levi ugao
             1.0f, -1.0f, // Donji desni ugao
             1.0f,  1.0f, // Gornji desni ugao
            -1.0f,  1.0f  // Gornji levi ugao
        };
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
	};

    void draw() {
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        glUniform3f(glGetUniformLocation(shaderProgram, "uColor"), this->r, this->g, this->b);

        glUniform1f(glGetUniformLocation(shaderProgram, "uX"), 0.0f);
        glUniform1f(glGetUniformLocation(shaderProgram, "uY"), 0.0f);
        glUniform1f(glGetUniformLocation(shaderProgram, "uSx"), this->canvasScaleX);
        glUniform1f(glGetUniformLocation(shaderProgram, "uSy"), this->canvasScaleY);
        glUniform1f(glGetUniformLocation(shaderProgram, "uAlpha"), this->alpha);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glBindVertexArray(0);
    }

    void changeTransparency() {
        if (alpha == 0.5f) {
            alpha = 0.0f;
        }
        else {
            alpha = 0.5f;
        }
    }

};