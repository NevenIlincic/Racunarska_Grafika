#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

class TileVertical {

public:
    std::vector<float> downLeftVertex;
    float width;
    float height;

    std::vector<float> normalVector;

    unsigned int VAO, VBO;
    unsigned int shaderProgram;

    TileVertical() {}

    TileVertical(unsigned int shader, std::vector<float> _downLeftVertex, std::vector<float> _normalVector, float _width, float _height) :
        shaderProgram(shader), downLeftVertex(_downLeftVertex), normalVector(_normalVector), width(_width), height(_height)
    {

        float vertices[] = {
            downLeftVertex[0], downLeftVertex[1], downLeftVertex[2], this->normalVector[0], this->normalVector[1], this->normalVector[2], //Donje levo teme
            downLeftVertex[0] + this->width, downLeftVertex[1], downLeftVertex[2], this->normalVector[0], this->normalVector[1], this->normalVector[2], //Donje desno 
            downLeftVertex[0] + this->width, downLeftVertex[1] + this->height, downLeftVertex[2], this->normalVector[0], this->normalVector[1], this->normalVector[2], // Gornje desno
            downLeftVertex[0], downLeftVertex[1] + this->height, downLeftVertex[2], this->normalVector[0], this->normalVector[1], this->normalVector[2], // Gornje levo
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
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "uM");
        unsigned int colorLoc = glGetUniformLocation(shaderProgram, "uColor");

        glm::mat4 model = glm::mat4(1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glUniform3f(colorLoc, 0.3f, 0.3f, 0.3f);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        glBindVertexArray(0);
        
    }
};