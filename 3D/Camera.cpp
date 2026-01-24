#include "Camera.h"


Camera::Camera(){}

Camera::Camera(glm::vec3 cameraPosition, unsigned int shader): position(cameraPosition), shaderProgram(shader) {
    this->front = glm::vec3(0.0f, 0.0f, -2.0f);
    this->up = glm::vec3(0.0f, 1.0f, 0.0f);
    this->yaw = -90.0f; // Početni ugao gledanja
    this->pitch = 0.0f;
    this->sensitivity = 0.1f;
    this->lastX = 400.0f;
    this->lastY = 300.0f;
    this->cameraSpeed = 0.01f;
    this->firstMouse = true;
       
    this->viewLocation = glGetUniformLocation(shader, "uV");
    this->projectionLocation = glGetUniformLocation(shader, "uP");
}

void Camera::processMouseMovement(float xoffset, float yoffset) {
    xoffset *= this->sensitivity;
    yoffset *= this->sensitivity;

    this->yaw += xoffset;
    this->pitch += yoffset;

    // Ograničenja
    if (this->pitch > 89.0f)  this->pitch = 89.0f;
    if (this->pitch < -89.0f) this->pitch = -89.0f;

    updateCameraVectors();
}

void Camera::proccessKeyInputs(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        this->position += cameraSpeed * this->front;
        this->checkCollisionWithObjects();
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        this->position -= cameraSpeed * this->front;
        this->checkCollisionWithObjects();
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        this->position -= glm::normalize(glm::cross(this->front, this->up)) * cameraSpeed;
        this->checkCollisionWithObjects();
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        this->position += glm::normalize(glm::cross(this->front, this->up)) * cameraSpeed;
        this->checkCollisionWithObjects();
    }
}

void Camera::sendToShader(unsigned int shaderProgram, const std::string& viewName, const std::string& projName, glm::mat4 projectionMatrix) {
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, viewName.c_str());
    unsigned int projLoc = glGetUniformLocation(shaderProgram, projName.c_str());

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(getViewMatrix()));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void Camera::updateShader(glm::mat4 projectionMatrix) {
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(getViewMatrix()));
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void Camera::addWorldSpaceObject(WorldSpaceObject* worldSpaceObject) {
    this->worldSpaceObjects.push_back(worldSpaceObject);
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(this->position, this->position + this->front, this->up);
}

void Camera::updateCameraVectors() {
    glm::vec3 direction;
    direction.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    direction.y = sin(glm::radians(this->pitch));
    direction.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->front = glm::normalize(direction);
}


void Camera::checkCollisionWithObjects() {
    for (WorldSpaceObject* worldSpaceObject : this->worldSpaceObjects) {
        if (worldSpaceObject) {
            worldSpaceObject->checkCameraCollision(*this);
        }
    }
    /*if (this->position.x < -1.17f) { this->position.x = -1.17f; }
    if (this->position.x > 0.97f) { this->position.x = 0.97f; }
    if (this->position.y > 2.97f) { this->position.y = 2.97f; }*/
}
