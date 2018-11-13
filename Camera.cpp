#include <iostream>
#include "Camera.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


void Camera::moveCamera(float x, float y, float z) {
    cameraPos += x * cameraSpeed * cameraFront;
    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * y * cameraSpeed;
    cameraPos += z * cameraSpeed * cameraUp;
    updateView();
}

void Camera::rotateCamera(float x, float y){
    float xoffset = (x - lastX) * sensitivity;
    float yoffset = (lastY - y) * sensitivity; // reversed since y-coordinates range from bottom to top
    lastX = x;
    lastY = y;
    yaw = std::fmod((yaw + xoffset),360.f);
    pitch += yoffset;
    if(pitch > 89.0f)
        pitch =  89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;
    updateView();
//    std::cout<<"yaw : "<<yaw<<std::endl<<"pitch : "<<pitch<<std::endl;
}

void Camera::updateView() {
    glm::vec3 front;
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    cameraFront = glm::normalize(front);
    view = glm::lookAt(cameraPos,
                       cameraPos+cameraFront,
                       cameraUp);
}

Camera::Camera(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraTarget, float cameraSpeed, float sensitivity) {
    this->cameraPos = cameraPos;
    this->cameraFront = cameraFront;
    this->cameraTarget = cameraTarget;
    this->cameraDirection = glm::normalize(cameraPos - cameraTarget);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    this->cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    this->cameraUp = glm::cross(cameraDirection, cameraRight);

    this->view = glm::lookAt(cameraPos,
                             cameraPos + cameraFront,
                             cameraUp);

    this->cameraSpeed = cameraSpeed;
    this->sensitivity=sensitivity;
    this->yaw = 0;
    this->pitch = 0;
    this->lastX = 400;
    this->lastY = 300;
}
