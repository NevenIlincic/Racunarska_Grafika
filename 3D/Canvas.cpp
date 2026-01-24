#include <vector>
#include <GL/glew.h>
#include "Canvas.h"

Canvas::Canvas() {};

Canvas::Canvas(Shader _shader, float _width, float _height): shaderProgram(_shader), canvasWidth(_width), canvasHeight(_height) {

    x = 0.1f;
    y = 0.95f;

    r = 1.0f;
    g = 1.0f;
    b = 1.0f;

    canvasScaleX = 1.4f;
    canvasScaleY = 0.25f;

    movieTime = 20.0f;
    frameCounter = 0;
    hasMovieStarted = false;
    isMovieFinished = false;

    std::vector<float> wallTileWideColor = { 1.0f, 1.0f, 1.0f };
    //Front Wall
    std::vector<float> frontWallDownLeftVertex = { -1.0f, 0.5f, 0.99f };
    std::vector<float> frontWallNormalVector = { 0.0f, 0.0f, -1.0f };
    bool isNormalTowardsPositive = false;
    this->canvas =  new TileVerticalWide(this->shaderProgram, frontWallDownLeftVertex, frontWallNormalVector, this->canvasWidth, this->canvasHeight, wallTileWideColor, isNormalTowardsPositive);
        
}

void Canvas::draw() {
    this->canvas->draw();

    checkForMovieFinish();
}

void Canvas::startMovie() {
    this->hasMovieStarted = true;
    this->isMovieFinished = false;
    this->startTime = glfwGetTime();
}

void Canvas::checkForMovieFinish() {
    if (!this->hasMovieStarted || this->isMovieFinished) {
        this->r = 1.0f;
        this->g = 1.0f;
        this->b = 1.0f;
        this->frameCounter = 0;
    }else{
        if (glfwGetTime() - this->startTime > movieTime) {
            this->isMovieFinished = true;
            this->hasMovieStarted = false;
            this->r = 1.0f;
            this->g = 1.0f;
            this->b = 1.0f;
        }
        else {
            this->frameCounter++;
            if (this->frameCounter > 20) {
                this->r = generate_random_number(0.0f, 1.0f);
                this->g = generate_random_number(0.0f, 1.0f);
                this->b = generate_random_number(0.0f, 1.0f);
                this->frameCounter = 0;
            }
        }
        
    }
}

void Canvas::resetCanvas() {
    this->r = 1.0f;
    this->g = 1.0f;
    this->b = 1.0f;
    frameCounter = 0;
    hasMovieStarted = false;
    isMovieFinished = false;
}
    
