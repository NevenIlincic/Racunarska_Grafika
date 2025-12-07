#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define _USE_MATH_DEFINES
#include <cmath> // za pi
#include <algorithm> // za max()
#include "Header/Util.h"



class Watermark {
public:
    unsigned int VAO, VBO;     // zajednički VAO/VBO
    unsigned int shaderTextureProgram;
    unsigned watermarkTexture;

	Watermark() {};

	Watermark(unsigned int shader): shaderTextureProgram(shader) {

        preprocessTexture(watermarkTexture, "Resources/watermark.png", false);

        float quadVertices[] = {
            //  X,     Y,     U,    V
         -1.0f,  -0.85f,    0.0f, 1.0f,// gornje levo teme
         -1.0f, -0.75f,    0.0f, 0.0f,// donje levo teme
         -0.5f, -0.75f,    1.0f, 0.0f, // donje desno teme
         -0.5f,  -0.85f,    1.0f, 1.0f// gornje desno teme
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

        // pozicija
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // tekstura
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

	}

    void draw() {
        glUseProgram(shaderTextureProgram);

        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(shaderTextureProgram, "uTex"), 0);
        glUniform1f(glGetUniformLocation(shaderTextureProgram, "uAlpha"), 0.5f);
        glUniform1f(glGetUniformLocation(shaderTextureProgram, "uX"), 0.0f);
        glUniform1f(glGetUniformLocation(shaderTextureProgram, "uY"), 0.0f);
        glUniform1f(glGetUniformLocation(shaderTextureProgram, "uSx"), 1.0f);
        glUniform1f(glGetUniformLocation(shaderTextureProgram, "uSy"), 1.0f);
        glBindTexture(GL_TEXTURE_2D, watermarkTexture);


        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindVertexArray(0);
    }

};