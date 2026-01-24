#include <vector>
#include "Door.h"

Door::Door() {};

Door::Door(Shader _shader, float _doorWidth, float _doorHeight): shaderProgram(_shader), doorWidth(_doorWidth), doorHeight(_doorHeight) {

    this->state = DoorState::Closed;

    this->x = 0.75f;
    this->y = 0.0f;
    this->z = 0.99f;

    this->r = 0.4f;
    this->g = 0.26f;
    this->b = 0.13f;

    doorScaleX = 0.25f;
    doorScaleY = 0.15f;

    std::vector<float> wallTileWideColor = { 0.0f, 1.0f, 1.0f };
    //Front Wall
    std::vector<float> frontWallDownLeftVertex = { 0.5f, 0.0f, 0.99f };
    std::vector<float> frontWallNormalVector = { 0.0f, 0.0f, -1.0f };

    bool isNormalTowardsPositive = false;
    this->door = new TileVerticalWide(this->shaderProgram, frontWallDownLeftVertex, frontWallNormalVector, this->doorWidth, this->doorHeight, wallTileWideColor, false);
        
    this->doorModel = Model("Resources/Door/door.obj");

    //float vertices[] = {
    //    -0.5f, -0.5f,
    //     0.5f, -0.5f,
    //     0.5f,  0.25f,
    //    -0.5f,  0.25f
    //};

    //glGenVertexArrays(1, &VAO);
    //glGenBuffers(1, &VBO);

    //glBindVertexArray(VAO);
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //// Pozicije
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);
    //// Normale (ako tvoj shader koristi location 3 za normale kao u SeatsManager-u)
    //glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(3);
}

void Door::draw() {
    this->shaderProgram.use();

    // 1. Aktivacija tekstura i useTex zastavice
    this->shaderProgram.setBool("useTex", false);

    glm::mat4 modelMat = glm::mat4(1.0f);

    // 2. Transformacije
    // Pomeri model ispred kamere da ga sigurno vidis (npr. z = -2.0f ako je kamera na 0.5)
    modelMat = glm::translate(modelMat, glm::vec3(this->x, this->y, this->z));
    modelMat = glm::rotate(modelMat, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // Proveri skalu - ako je model sitan, 0.01f ga cini nevidljivim. 
    // Za probu stavi 1.0f ili 0.1f
    modelMat = glm::scale(modelMat, glm::vec3(0.001f, 0.001f, 0.001f));

    this->shaderProgram.setMat4("uM", modelMat);

    // 3. Crtanje
    // Pre crtanja iskljuci Face Culling jer su neki modeli "rupičasti"
    glDisable(GL_CULL_FACE);
    this->doorModel.Draw(this->shaderProgram);
    glEnable(GL_CULL_FACE);

    this->shaderProgram.setBool("useTex", false);
        
    /* glUseProgram(this->shaderProgram);
    glBindVertexArray(VAO);

    unsigned int modelLoc = glGetUniformLocation(this->shaderProgram, "uM");
    unsigned int colorLoc = glGetUniformLocation(this->shaderProgram, "uColor");

    glm::mat4 model = glm::mat4(1.0f);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glUniform3f(colorLoc, this->r, this->g, this->b);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindVertexArray(0);*/
}

void Door::openDoor() {
    this->state = DoorState::Opened;
    this->x = -0.85f;
    this->doorScaleX = 0.05f;
    this->doorScaleY = 0.6f;
}

void Door::closeDoor() {
    this->state = DoorState::Closed;
    this->x = -1.0f;
    this->doorScaleX = 0.25f;
    this->doorScaleY = 0.15f;
}

