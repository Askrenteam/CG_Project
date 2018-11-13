#pragma once

#include "glm/vec3.hpp"
#include "glm/detail/type_mat4x4.hpp"

class Camera {
public:
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraTarget;
    glm::vec3 cameraDirection;
    glm::vec3 up;
    glm::vec3 cameraRight;
    glm::vec3 cameraUp;
    glm::mat4 view;

    float cameraSpeed;
    float sensitivity;
    float lastX;
    float lastY;

    float yaw;
    float pitch;

    void moveCamera (float x, float y, float z);

    void rotateCamera (float x, float y);

    Camera (glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraTarget, float cameraSpeed, float sensitivity);

private:
    void updateView();
};


