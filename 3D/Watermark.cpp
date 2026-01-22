#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define _USE_MATH_DEFINES
#include <cmath> // za pi
#include <algorithm> // za max()
#include "Header/Util.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>



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
            -1.0f, -0.75f,    0.0f, 0.0f, // 1. Donje levo 
            -0.5f, -0.75f,    1.0f, 0.0f, // 2. Donje desno
            -0.5f, -0.85f,    1.0f, 1.0f, // 3. Gornje desno
            -1.0f, -0.85f,    0.0f, 1.0f
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
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);

	}

    void draw() {
        glUseProgram(shaderTextureProgram);
        GLboolean wasDepthTestEnabled = glIsEnabled(GL_DEPTH_TEST);
        GLboolean wasCullFaceEnabled = glIsEnabled(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);

        // 2. Isključi Cull Face tako da se vidi čak i ako je naopako
        glDisable(GL_CULL_FACE);
       
        // Koristi Identity matrice da ignorišeš kameru
        glm::mat4 identity = glm::mat4(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(shaderTextureProgram, "uM"), 1, GL_FALSE, glm::value_ptr(identity));
        glUniformMatrix4fv(glGetUniformLocation(shaderTextureProgram, "uV"), 1, GL_FALSE, glm::value_ptr(identity));
        glUniformMatrix4fv(glGetUniformLocation(shaderTextureProgram, "uP"), 1, GL_FALSE, glm::value_ptr(identity));

        glUniform1i(glGetUniformLocation(shaderTextureProgram, "useTex"), 1);
        glUniform1f(glGetUniformLocation(shaderTextureProgram, "uAlpha"), 1.0f);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, watermarkTexture);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindVertexArray(0);

        if (wasDepthTestEnabled) glEnable(GL_DEPTH_TEST);
        else glDisable(GL_DEPTH_TEST);

        if (wasCullFaceEnabled) glEnable(GL_CULL_FACE);
        else glDisable(GL_CULL_FACE);

  
    }

};