#pragma once
#ifdef __APPLE__
#include <GLM/glm.hpp>
#include <GLFW/glfw3.h>
#else
#include <glm.hpp>
#include <glfw3.h>
#endif
#include <iostream>

class Camera {
public:
	Camera(const int width, const int height, const glm::vec3& position);
	void Set(const int width, const int height, const glm::vec3& position);
	void Reset(const int width, const int height);
	void Reshape(int windowWidth, int windowHeight);
	const glm::mat4 GetViewMatrix() const;
	const glm::vec3 GetPosition() const;
	const glm::mat4 GetProjectionMatrix() const;
	void ProcessKeyboard(int direction, float deltaTime);
	void MouseControl(float xPos, float yPos);
	void ProcessMouseScroll(float yOffset);
	void LockMouseToCenter(bool locked) { mouseLocked = locked; }
    
    glm::vec3 GetForward() {
        if (!freeLook) return glm::normalize(forward);
        
        glm::vec3 fwd;
        fwd.x = cos(glm::radians(yawCopy)) * cos(glm::radians(pitchCopy));
        fwd.y = sin(glm::radians(pitchCopy));
        fwd.z = sin(glm::radians(yawCopy)) * cos(glm::radians(pitchCopy));
        fwd = glm::normalize(fwd);
        
        right = glm::normalize(glm::cross(fwd, worldUp));
        up = glm::normalize(glm::cross(right, fwd));
        
        return fwd;
    }
    
    glm::vec3 GetUp() { return glm::normalize(up); }
    
    void changeFreeLook() {
        if (freeLook) {
            yaw = yawCopy;
            pitch = pitchCopy;
        }
        
        else {
            yawCopy = yaw;
            pitchCopy = pitch;
        }

        UpdateCameraVectors();
        freeLook = !freeLook;
    }
    
    bool getFreeLook() { return freeLook; }
    
    unsigned int cameraMode = 1;
    
private:
	void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
	void UpdateCameraVectors();
    
    bool freeLook = false;

	const float cameraSpeedFactor = 10.f;
	const float mouseSensitivity = 0.1f;
	const float zNEAR = 0.1f;
	const float zFAR = 500.f;
	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float FOV = 90.0f;

	float zNear;
	float zFar;
	float FoVy;
	int width;
	int height;
	bool isPerspective;
    
    float yawCopy;
    float pitchCopy;

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
	bool mouseLocked = false;
};
