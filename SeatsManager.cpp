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
    bool canManipulateSeats;
    float seatSize;

    int takenSeats;

    SeatsManager() {};

    SeatsManager(unsigned int shader) : shaderProgram(shader) {
        // Primer: 5x10 sedišta
        int rows = 5;
        int cols = 10;
        float spacingX = 0.15f;
        float spacingY = 0.2f;
        seatSize = 0.1f;

        canManipulateSeats = true;

        takenSeats = 0;
       

        for (int i = rows - 1; i >= 0; i--) {
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
            glUniform1f(glGetUniformLocation(shaderProgram, "uSx"), seatSize);
            glUniform1f(glGetUniformLocation(shaderProgram, "uSy"), seatSize);
            glUniform1f(glGetUniformLocation(shaderProgram, "uAlpha"), 1.0f);

            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        }

        glBindVertexArray(0);
    }

    // Promena stanja sedišta po koordinatama klika
    void reserve(float mouseX, float mouseY) {
        float halfSize = seatSize / 2.0f;
        if (canManipulateSeats) {
            for (Seat& seat : seats) {

                if (mouseX >= seat.x - halfSize && mouseX <= seat.x + halfSize &&
                    mouseY >= seat.y - halfSize && mouseY <= seat.y + halfSize) {
                    seat.reserveSeat();

                    if (seat.state == State::Free) {
                        takenSeats--;
                    }
                    else {
                        takenSeats++;
                    }
                }
            }
        }
    }

    void buySeats(int numSeats) {
        if (numSeats > seats.size() - takenSeats) {
            return;
        }
        bool canTakeSeats = false;
        int startingSeatIndex = seats.size() - 1;
        int checkedFreeSeats = 0;

        if (this->canManipulateSeats) {
            //Provera da li postoji numSeats uzastopnih sedista
            for (int i = seats.size() - 1; i >= 0; i--) {
                Seat& startingSeat = seats[i];
                if (startingSeat.state == State::Free) {
                    startingSeatIndex = i;
                    checkedFreeSeats++;
                    for (int z = 1; z < numSeats; z++) {
                        Seat adjacentSeat = seats[i - z];
                        if (adjacentSeat.state != State::Free) {
                            checkedFreeSeats = 0;
                            break;
                        }
                        checkedFreeSeats++;
                    }
                }

                //Ako potoji N uzastopnih sedista, moze da se kupuje
                if (checkedFreeSeats == numSeats) {
                    canTakeSeats = true;
                    break;
                }
            }

            //Kupovina sedista
            if (canTakeSeats) {
                for (int z = 0; z < numSeats; z++) {
                    Seat& currentSeat = seats[startingSeatIndex - z];
                    currentSeat.buySeat();
                    takenSeats++;
                }
            }
        }
    }

};
