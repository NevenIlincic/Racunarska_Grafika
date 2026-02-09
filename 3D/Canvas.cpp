#include <vector>
#include <GL/glew.h>
#include "Canvas.h"
#include "Header/Util.h"

Canvas::Canvas() {};

Canvas::Canvas(Shader _shader, float _width, float _height): shaderProgram(_shader), canvasWidth(_width), canvasHeight(_height) {

    x = 0.1f;
    y = 0.95f;

    r = 1.0f;
    g = 1.0f;
    b = 1.0f;

    canvasScaleX = 1.4f;
    canvasScaleY = 0.25f;

    movieTime = 2.0f; /// STAVI NA 20.0f
    frameCounter = 0;
    hasMovieStarted = false;
    isMovieFinished = false;

    this->imageIndex = 0;

    std::vector<float> wallTileWideColor = { 1.0f, 1.0f, 1.0f };
    //Front Wall
    std::vector<float> frontWallDownLeftVertex = { -1.0f, 0.5f, 0.99f };
    std::vector<float> frontWallNormalVector = { 0.0f, 0.0f, -1.0f };
    bool isNormalTowardsPositive = false;

    this->canvas =  new TileVerticalWide(this->shaderProgram, frontWallDownLeftVertex, frontWallNormalVector, this->canvasWidth, this->canvasHeight, wallTileWideColor, isNormalTowardsPositive, true);
     
    for (int i = 0; i <= 19; i++) {
        std::string imagePath = "Resources/Movie/frame_" + std::to_string(i + 1) + ".jpg";
        unsigned int texID;
        preprocessTexture(texID, imagePath.c_str(), true);
        this->images.push_back(texID);
    }

}

void Canvas::draw() {
    this->shaderProgram.use();
    if (this->hasMovieStarted && !this->isMovieFinished) {
        this->shaderProgram.setBool("useTex", true);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->images[this->imageIndex]);
    }
    else {
        this->shaderProgram.setBool("useTex", false);
    }
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
        this->shaderProgram.setBool("useTex", false);
    }else{
        if (glfwGetTime() - this->startTime > movieTime) {
            this->isMovieFinished = true;
            this->hasMovieStarted = false;
            this->r = 1.0f;
            this->g = 1.0f;
            this->b = 1.0f;
            this->shaderProgram.setBool("useTex", false);
        }
        else {
            this->shaderProgram.setBool("useTex", true);
            this->frameCounter++;
            if (this->frameCounter > 20) {
                this->frameCounter = 0;
                this->imageIndex = (this->imageIndex + 1) % 20;
            }

        }
        
    }
}

void Canvas::resetCanvas() {
    this->r = 1.0f;
    this->g = 1.0f;
    this->b = 1.0f;
    frameCounter = 0;
    this->imageIndex = 0;
    hasMovieStarted = false;
    isMovieFinished = false;
}
    
