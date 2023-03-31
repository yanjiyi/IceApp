#include "Camera.h"
#include <algorithm>

Camera::Camera()
{
}

Camera::Camera(float pos_x, float pos_y, float pos_z, float front_x, float front_y, float front_z, float up_x, float up_y, float up_z)
{
    cameraPos = glm::vec3(pos_x, pos_y, pos_z);
    cameraFront = glm::vec3(front_x, front_y, front_z);
    cameraUp = glm::vec3(up_x, up_y, up_z);

    CameraUpdate();
}

Camera::Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up)
{
    cameraPos = pos;
    cameraFront = front;
    cameraUp = up;

    CameraUpdate();
}

Camera::~Camera()
{
}

void Camera::CameraForward(float deltaTime)
{
    float tempSpeed = cameraSpeed * deltaTime;

    cameraPos += cameraSpeed * cameraFront;
}

void Camera::CameraBackward(float deltaTime)
{
    float tempSpeed = cameraSpeed * deltaTime;

    cameraPos -= cameraSpeed * cameraFront;
}

void Camera::CameraWardLeft(float deltaTime)
{
    float tempSpeed = cameraSpeed * deltaTime;

    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * tempSpeed;
}

void Camera::CameraWardRight(float deltaTime)
{
    float tempSpeed = cameraSpeed * deltaTime;

    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * tempSpeed;
}

void Camera::CameraJumpUp(float deltaTime)
{
    float tempSpeed = cameraSpeed * deltaTime;

    cameraPos += cameraUp * tempSpeed;
}

void Camera::SetCameraSpeed(float speed)
{
    cameraSpeed = speed;
}

void Camera::SetCameraDistance(float distance)
{
    this->distance = distance;
    if (distance < 1.0f)
        distance = 1.0f;
}

void Camera::CameraMouseProcess(int xoffset, int yoffset, int preciseY, float deltaTime)
{
    float mxoffset = static_cast<float>(xoffset);
    float myoffset = static_cast<float>(yoffset);

    mxoffset *= sensitiviy;
    myoffset *= sensitiviy;

    yaw += mxoffset;
    pitch += myoffset;

    if (cameraSurround)
    {
        distance += -static_cast<float>(preciseY);

        if (distance < 1.0f)
            distance = 1.0f;

        cameraPos.x = distance * sin(yaw) * cos(pitch);
        cameraPos.y = distance * sin(pitch);
        cameraPos.z = distance * cos(pitch) * cos(yaw);
    }
    else {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        cameraFront = glm::normalize(front);
    }
}

void Camera::SetCameraMouseMode(bool surround)
{
    cameraSurround = surround;
}

glm::vec3 Camera::GetCameraPos(void) const
{
    return cameraPos;
}

void Camera::SetCameraPos(glm::vec3 pos)
{
    cameraPos = pos;
}

glm::vec3 Camera::GetCameraFront(void) const
{
    return cameraFront;
}

void Camera::SetCameraFront(glm::vec3 front)
{
    cameraFront = front;
}

glm::vec3 Camera::GetCameraUp(void) const
{
    return cameraUp;
}

void Camera::SetCameraUp(glm::vec3 up)
{
    cameraUp = up;
}

glm::mat4 Camera::GetCameraMatrix(void) const
{
    return cameraMatrix;
}

void Camera::CameraUpdate(void)
{
    if (cameraSurround)
    {
        if (distance < 1.0f)
            distance = 1.0f;

        cameraPos.x = distance * sin(yaw) * cos(pitch);
        cameraPos.y = distance * sin(pitch);
        cameraPos.z = distance * cos(pitch) * cos(yaw);
    }
    else {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        cameraFront = glm::normalize(front);
    }

    cameraMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}
