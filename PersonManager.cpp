#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define _USE_MATH_DEFINES
#include <cmath> // za pi
#include <algorithm> // za max()
#include <iostream>
#include "Person.cpp"
#include "Header/Util.h"

class PersonManager {
public:
	std::vector<Person> people;
	unsigned int VAO, VBO;     // zajednički VAO/VBO
	unsigned int shaderTextureProgram;
    Person man;

    unsigned personTexture;

	PersonManager() {};


	PersonManager(unsigned int shader): shaderTextureProgram(shader) {
        preprocessTexture(personTexture, "Resources/stickman.png", true);

        float quadVertices[] = {
            //  X,       Y,       U,     V
            -1.0f,  1.0f,    0.0f, 1.0f, // Gornje levo teme (u gornjem levom uglu prozora)
            -1.0f,  0.8f,    0.0f, 0.0f, // Donje levo teme
            -0.9f,  0.8f,    1.0f, 0.0f, // Donje desno teme
            -0.9f,  1.0f,    1.0f, 1.0f  // Gornje desno teme
        };

        man = Person(0.0f, 0.0f, 50.0f, 100.0f);

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
    };

    void draw() {
        glUseProgram(shaderTextureProgram);

        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(shaderTextureProgram, "uTex"), 0);
        glUniform1f(glGetUniformLocation(shaderTextureProgram, "uAlpha"), 1.0f);
        glBindTexture(GL_TEXTURE_2D, personTexture);


        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindVertexArray(0);
    }

};