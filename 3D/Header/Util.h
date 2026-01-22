#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
int endProgram(std::string message);
unsigned int createShader(const char* vsSource, const char* fsSource);
unsigned loadImageToTexture(const char* filePath, bool flipImage);
GLFWcursor* loadImageToCursor(const char* filePath);
void preprocessTexture(unsigned& texture, const char* filepath, bool flipImage);
int generate_random_number(int min, int max);
float generate_random_number(float min, float max);