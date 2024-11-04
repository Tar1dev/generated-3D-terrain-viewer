#include "Camera.h"
#include <iostream>

Camera::Camera(int width, int height) {
    pos = glm::vec3(0.0f, 0.0f, 5.0f);
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f,  0.0f);
    view = glm::lookAt(pos, pos + front, up);
    lastX = width / 2;
    lastY = height / 2;
    yaw = -90.f;
}

glm::vec3 Camera::getPos() { return pos; }

glm::vec3 Camera::getTarget() { return target; }

glm::vec3 Camera::getDirection() { return direction; }

glm::vec3 Camera::getRight() { return right; }

glm::vec3 Camera::getUp() { return up; }

glm::vec3 Camera::getFront() { return front; }

glm::mat4 Camera::getView() { return view; }

void Camera::setFront(glm::vec3 new_front) {
    front = new_front;
}

void Camera::setPos(glm::vec3 new_pos) { pos = new_pos; }

void Camera::update(GLFWwindow* window, float dt) {
    float cameraSpeed = 5.5f * dt; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        pos += cameraSpeed * front;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        pos -= cameraSpeed * front;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        pos -= glm::normalize(glm::cross(front, up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        pos += glm::normalize(glm::cross(front, up)) * cameraSpeed;
    view = glm::lookAt(pos, pos + front, up);
}

void Camera::updateEuler(double xpos, double ypos) {
    if (firstMouse) // initially set to true
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(direction);
}

void Camera::updateFOV(double xoffset, double yoffset) {
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f; 
}

float Camera::getFOV() { return fov; }
