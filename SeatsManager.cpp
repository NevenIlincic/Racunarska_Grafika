#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define _USE_MATH_DEFINES
#include <cmath> // za pi
#include <algorithm> // za max()
#include <iostream>
#include "Seat.cpp"

class SeatsManager {
public:
    std::vector<Seat> seats;   // lista svih sedišta
    unsigned int VAO, VBO;     // zajednički VAO/VBO
    unsigned int shaderProgram; // shader koji se koristi

    SeatsManager() {};

    SeatsManager(unsigned int shader) : shaderProgram(shader) {
        // Primer: 5x10 sedišta
        int rows = 5;
        int cols = 10;
        float spacingX = 0.12f;
        float spacingY = 0.12f;
        float seatSize = 0.1f;
       

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                seats.push_back(Seat(j * spacingX - 0.6f, i * spacingY - 0.4f));
            }
        }

        // Kreiranje jednog VAO/VBO za kvadrat
        float quadVertices[] = {
           -0.5f, -0.5f,
            0.5f, -0.5f,
            0.5f,  0.5f,
           -0.5f,  0.5f
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

    // Crtanje svih sedišta
    void draw() {
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        for (Seat& seat : seats) {
            glUniform3f(glGetUniformLocation(shaderProgram, "uColor"), seat.r, seat.g, seat.b); // zeleno
         
            glUniform1f(glGetUniformLocation(shaderProgram, "uX"), seat.x);
            glUniform1f(glGetUniformLocation(shaderProgram, "uY"), seat.y);
            glUniform1f(glGetUniformLocation(shaderProgram, "uSx"), 0.1f);
            glUniform1f(glGetUniformLocation(shaderProgram, "uSy"), 0.1f);
            glUniform1f(glGetUniformLocation(shaderProgram, "uAlpha"), 1.0f);

            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        }

        glBindVertexArray(0);
    }

    // Promena stanja sedišta po koordinatama klika
    void click(float mouseX, float mouseY) {
        float halfSize = 0.1f / 2.0f;
        std::cout << seats.size() << std::endl;
        for (Seat& seat : seats) {
            printf("Klik: %.2f %.2f | Seat: %.2f %.2f\n",
                mouseX, mouseY, seat.x, seat.y);
            if (mouseX >= seat.x - halfSize && mouseX <= seat.x + halfSize &&
                mouseY >= seat.y - halfSize && mouseY <= seat.y + halfSize) {
                seat.reserveSeat();
             
                
            }
        }
    }
};
