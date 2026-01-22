#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define _USE_MATH_DEFINES
#include <cmath> // za pi
#include <algorithm> // za max()
#include <iostream>
#include "Header/Util.h"


//GLM biblioteke
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;

    float yaw; // Početni ugao gledanja
    float pitch;
    float sensitivity;

    float lastX = 400.0f;
    float lastY = 300.0f;
    float cameraSpeed = 0.01f;
    bool firstMouse = true;

    unsigned int shaderProgram;
    unsigned int viewLocation, projectionLocation;

    Camera(){}

	Camera(glm::vec3 cameraPosition, unsigned int shader): position(cameraPosition), shaderProgram(shader) {
        this->front = glm::vec3(0.0f, 0.0f, -2.0f);
        this->up = glm::vec3(0.0f, 1.0f, 0.0f);
        this->yaw = -90.0f; // Početni ugao gledanja
        this->pitch = 0.0f;
        this->sensitivity = 0.1f;

        viewLocation = glGetUniformLocation(shader, "uV");
        projectionLocation = glGetUniformLocation(shader, "uP");
    }

    void processMouseMovement(float xoffset, float yoffset) {
        xoffset *= this->sensitivity;
        yoffset *= this->sensitivity;

        this->yaw += xoffset;
        this->pitch += yoffset;

        // Ograničenja
        if (this->pitch > 89.0f)  this->pitch = 89.0f;
        if (this->pitch < -89.0f) this->pitch = -89.0f;

        updateCameraVectors();
    }

    void proccessKeyInputs(GLFWwindow* window) {
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            this->position += cameraSpeed * this->front;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            this->position -= cameraSpeed * this->front;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            this->position -= glm::normalize(glm::cross(this->front, this->up)) * cameraSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            this->position += glm::normalize(glm::cross(this->front, this->up)) * cameraSpeed;
        }
    }

    void sendToShader(unsigned int shaderProgram, const std::string& viewName, const std::string& projName, glm::mat4 projectionMatrix) {
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, viewName.c_str());
        unsigned int projLoc = glGetUniformLocation(shaderProgram, projName.c_str());

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(getViewMatrix()));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    }

    void updateShader(glm::mat4 projectionMatrix) {
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(getViewMatrix()));
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    }

private:
    void updateCameraVectors() {
        glm::vec3 direction;
        direction.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        direction.y = sin(glm::radians(this->pitch));
        direction.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        front = glm::normalize(direction);
    }

    glm::mat4 getViewMatrix() {
        return glm::lookAt(position, position + front, up);
    }


};