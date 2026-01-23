#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

class TileHorizontal {

public:
    std::vector<float> downLeftVertex;
    float width;
    float length;

	unsigned int VAO, VBO;
	unsigned int shaderProgram;

    TileHorizontal(){}

    TileHorizontal(unsigned int shader, std::vector<float> _downLeftVertex, float _width, float _length):
        shaderProgram(shader), downLeftVertex(_downLeftVertex), width(_width), length(_length)
    {

        float vertices[] = {
            downLeftVertex[0], downLeftVertex[1], downLeftVertex[2], 0.0f, 1.0f, 0.0f, //Donje levo teme
            downLeftVertex[0] + this->width, downLeftVertex[1], downLeftVertex[2], 0.0f, 1.0f, 0.0f, //Donje desno 
            downLeftVertex[0] + this->width, downLeftVertex[1], downLeftVertex[2] - this->length, 0.0f, 1.0f, 0.0f, // Gornje desno
            downLeftVertex[0], downLeftVertex[1], downLeftVertex[2] - this->length, 0.0f, 1.0f, 0.0f, // Gornje levo
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

    void draw() {
        glUseProgram(this->shaderProgram);
        glBindVertexArray(this->VAO);

        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "uM");
        unsigned int colorLoc = glGetUniformLocation(shaderProgram, "uColor");

        glm::mat4 model = glm::mat4(1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glUniform3f(colorLoc, 0.2f, 0.2f, 0.2f);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glBindVertexArray(0);
    }
};