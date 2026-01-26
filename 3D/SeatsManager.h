#pragma once
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define _USE_MATH_DEFINES
#include <cmath> // za pi
#include <algorithm> // za max()
#include <iostream>
#include "Seat.h"
#include "Camera.h"
#include "shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class SeatsManager {
public:
    std::vector<Seat> seats;   // lista svih sedišta
    std::vector<Seat> usedSeats;
    unsigned int VAO, VBO;     // zajednički VAO/VBO
    Shader shaderProgram; // shader koji se koristi
    bool canManipulateSeats;
    float seatSize;

    int takenSeats;
    int firstFreeSeatIndex;

    float stepHeight;

    int rows, cols;

    SeatsManager();

    SeatsManager(int _rows, int _cols, Shader shader);

    void draw();
    void reserve(Camera& camera);
    void buySeats(int numSeats);
    std::vector<Seat> fillUsedSeats();
    void resetSeats();

private:
    bool isCameraLookingAt(Camera& camera, Seat& seat, float maxDistance);
};