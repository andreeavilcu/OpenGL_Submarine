#include "Camera.h"
#ifdef __APPLE__
#include <GLM/gtc/matrix_transform.hpp>
#else
#include <gtc/matrix_transform.hpp>
#endif

Camera::Camera(const int width, const int height, const glm::vec3& position) {
    startPosition = position;
    Set(width, height, position);
}

void Camera::Set(const int width, const int height, const glm::vec3& position) {
    isPerspective = true;
    yaw = YAW;
    pitch = PITCH;
    FoVy = FOV;
    this->width = width;
    this->height = height;
    zNear = zNEAR;
    zFar = zFAR;
    worldUp = glm::vec3(0, 1, 0);
    this->position = position;
    lastX = width / 2.0f;
    lastY = height / 2.0f;
    bFirstMouseMove = true;
    UpdateCameraVectors();
}

void Camera::Reset(const int width, const int height) {
    Set(width, height, startPosition);
}

void Camera::Reshape(int windowWidth, int windowHeight) {
    width = windowWidth;
    height = windowHeight;
    glViewport(0, 0, windowWidth, windowHeight);
}

const glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(position, position + forward, up);
}

const glm::vec3 Camera::GetPosition() const {
    return position;
}

const glm::mat4 Camera::GetProjectionMatrix() const {
    glm::mat4 Proj = glm::mat4(1);
    if (isPerspective) {
        float aspectRatio = ((float)(width)) / height;
        Proj = glm::perspective(glm::radians(FoVy), aspectRatio, zNear, zFar);
    }
    else {
        float scaleFactor = 2000.f;
        Proj = glm::ortho<float>(-width / scaleFactor, width / scaleFactor,
            -height / scaleFactor, height / scaleFactor, -zFar, zFar);
    }
    return Proj;
}

void Camera::ProcessKeyboard(int direction, float deltaTime) {
    float velocity = cameraSpeedFactor * deltaTime;
    UpdateCameraVectors();
    
    switch (direction) {
        case 1: // FORWARD
            if (cameraMode == 1 || freeLook) position += forward * velocity;
            else if (cameraMode == 2) position += up * velocity;
            else if (cameraMode == 3) position += (up + forward) * velocity;
            break;
        case 2: // BACKWARD
            if (cameraMode == 1 || freeLook) position -= forward * velocity;
            else if (cameraMode == 2) position -= up * velocity;
            else if (cameraMode == 3) position -= (up + forward) * velocity;
            break;
        case 3: // LEFT
            position -= right * velocity;
            break;
        case 4: // RIGHT
            position += right * velocity;
            break;
        case 5: // UP
            if (cameraMode == 1 || freeLook) position += worldUp * velocity;
            else if (cameraMode == 2) position -= forward * velocity;
            else if (cameraMode == 3) position += (worldUp - forward) * velocity;
            break;
        case 6: // DOWN
            if (cameraMode == 1 || freeLook) position -= worldUp * velocity;
            else if (cameraMode == 2) position += forward * velocity;
            else if (cameraMode == 3) position -= (worldUp - forward) * velocity;
            break;
        case 7: {
            if (freeLook) break;
            
            float dist = cameraMode == 1 ? -10.f : cameraMode == 2 ? 0.f : 5.f;
            glm::vec3 pivot = position + forward * dist;

            position -= pivot;

            float angle = glm::radians(-45.0f) * velocity;
            glm::mat4 rotMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0));

            position = glm::vec3(rotMatrix * glm::vec4(position, 1.0f));

            position += pivot;

            yaw += glm::degrees(angle);

            UpdateCameraVectors();
            break;
        }
            
        case 8: {
            if (freeLook) break;
            
            float dist = cameraMode == 1 ? -10.f : cameraMode == 2 ? 0.f : 5.f;
            glm::vec3 pivot = position + forward * dist;

            position -= pivot;
            
            float angle = glm::radians(45.0f) * velocity;
            glm::mat4 rotMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0));

            position = glm::vec3(rotMatrix * glm::vec4(position, 1.0f));

            position += pivot;

            yaw += glm::degrees(angle);

            UpdateCameraVectors();
            break;
        }
    }
}

void Camera::MouseControl(float xPos, float yPos) {
    if (!freeLook) return;
    
    if (bFirstMouseMove) {
        lastX = xPos;
        lastY = yPos;
        bFirstMouseMove = false;
    }

    float xChange = xPos - lastX;
    float yChange = lastY - yPos;
    lastX = xPos;
    lastY = yPos;

    if (fabs(xChange) <= 1e-6 && fabs(yChange) <= 1e-6) {
        return;
    }
    
    xChange *= mouseSensitivity;
    yChange *= mouseSensitivity;

    ProcessMouseMovement(xChange, yChange);
}

void Camera::ProcessMouseScroll(float yOffset) {
    return;
    
//    if (FoVy >= 1.0f && FoVy <= 90.0f) {
//        FoVy -= yOffset;
//    }
//    FoVy = FoVy <= 1.0f ? 1.0f : FoVy;
//    FoVy = FoVy >= 90.0f ? 90.0f : FoVy;
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch) {
    yaw += xOffset;
    pitch += yOffset;
    
    if (constrainPitch) {
        pitch = pitch > 89.0f ? 89.0f : pitch;
        pitch = pitch < -89.0f ? -89.0f : pitch;
    }
    
    UpdateCameraVectors();
}

void Camera::UpdateCameraVectors() {
    forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward.y = sin(glm::radians(pitch));
    forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward = glm::normalize(forward);
    right = glm::normalize(glm::cross(forward, worldUp));
    up = glm::normalize(glm::cross(right, forward));
}
