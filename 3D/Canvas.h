#pragma once
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define _USE_MATH_DEFINES
#include <cmath> // za pi
#include <algorithm> // za max()
#include <iostream>
#include "Header/Util.h"
#include "TileVerticalWide.h"

class Canvas {
public:
    float x, y, z;
    float r, g, b;
    float canvasScaleX, canvasScaleY;
    unsigned int VAO, VBO;     // zajednički VAO/VBO
    Shader shaderProgram;

    TileVerticalWide* canvas;
    float canvasWidth;
    float canvasHeight;

    float movieTime;
    int frameCounter;

    double startTime;
    bool hasMovieStarted;
    bool isMovieFinished;

    Canvas();

    Canvas(Shader _shader, float _width, float _height);

    void draw();
    void startMovie();
    void checkForMovieFinish();
    void resetCanvas();

};