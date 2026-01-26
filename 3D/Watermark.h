#pragma once
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

#include "shader.hpp"


class Watermark {
public:
    unsigned int VAO, VBO;     // zajednički VAO/VBO
    Shader shaderProgram;
    unsigned watermarkTexture;

    Watermark();

    Watermark(Shader shader);

    void draw();
};