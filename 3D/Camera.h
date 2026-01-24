#pragma once
#include <vector>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "WorldSpaceObject.h"
#include <glm/gtc/type_ptr.hpp>         // Za glm::value_ptr
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    std::vector<WorldSpaceObject*> worldSpaceObjects;

    float yaw, pitch, sensitivity;
    float lastX, lastY, cameraSpeed;
    bool firstMouse;
    unsigned int shaderProgram, viewLocation, projectionLocation;

    Camera();
    Camera(glm::vec3 cameraPosition, unsigned int shader);

    void processMouseMovement(float xoffset, float yoffset);
    void proccessKeyInputs(GLFWwindow* window);
    void updateShader(glm::mat4 projectionMatrix);
    void sendToShader(unsigned int shaderProgram, const std::string& viewName, const std::string& projName, glm::mat4 projectionMatrix);
    void addWorldSpaceObject(WorldSpaceObject* object);
    glm::mat4 getViewMatrix();

private:
    void updateCameraVectors();
    void checkCollisionWithObjects();
};