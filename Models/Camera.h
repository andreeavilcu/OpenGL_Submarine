#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>

enum ECameraMovementType
{
    UNKNOWN,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera
{
public:
    Camera(const int width, const int height, const glm::vec3& position);

    void Set(const int width, const int height, const glm::vec3& position);
    void Reset(const int width, const int height);
    void Reshape(int windowWidth, int windowHeight);

    const glm::mat4 GetViewMatrix() const;
    const glm::vec3 GetPosition() const;
    const glm::mat4 GetProjectionMatrix() const;

    void ProcessKeyboard(ECameraMovementType direction, float deltaTime);
    void MouseControl(float xPos, float yPos);
    void ProcessMouseScroll(float yOffset);

private:
    void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
    void UpdateCameraVectors();

private:
    const float zNEAR = 0.1f;
    const float zFAR = 500.f;
    const float YAW = -90.0f;
    const float PITCH = 0.0f;
    const float FOV = 45.0f;
    const float cameraSpeedFactor = 250.0f;
    const float mouseSensitivity = 0.1f;

    float zNear;
    float zFar;
    float FoVy;
    int width;
    int height;
    bool isPerspective;

    glm::vec3 position;
    glm::vec3 forward;
    glm::vec3 right;
    glm::vec3 up;
    glm::vec3 worldUp;
    glm::vec3 startPosition;

    float yaw;
    float pitch;

    bool bFirstMouseMove = true;
    float lastX = 0.f, lastY = 0.f;
};
