#pragma once
#include <GL/glew.h>
#ifdef __APPLE__
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#else
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#endif
#include "Shader.h"
#include "Model.h"
#include "Camera.h"

#include <iostream>
#ifdef __APPLE__
#define MAX_PATH 260
#include <unistd.h>
#else
#include <Windows.h>
#endif
#include <locale>
#include <codecvt>

class SubmarineProgram {
public:
	SubmarineProgram(GLFWwindow* window);
	~SubmarineProgram();

	void UpdateCameraFollowSubmarine(const glm::vec3& submarinePosition);

	void Initialize();
	void Run();
	void Cleanup();

	bool day = true;

private:
	GLFWwindow* window;
	Camera* camera;

	const unsigned int SHADOW_SIZE = 1024;

	unsigned int VBO, cubeVAO, lightVAO, skyboxVAO, depthMapFBO, depthMap;
	Shader* lightingWithTextureShader, * lampShader, * skyboxShader, * shadowShader;
	Model* submarineModel, * terrainModel, * jellyFishModel, * clownFishModel, * sunModel, * angelFishModel, * koiFishModel;

	std::vector<glm::vec3> fishPositions; // Pozițiile peștilor
	std::vector<glm::vec3> fishVelocities; // Direcția și viteza fiecărui pește
	std::vector<float> fishSwimAnimationTime; // Time tracker for each fish's swimming animation
	std::vector<float> fishVerticalAnimationTime; // Time tracker for vertical oscillation

	glm::vec3 lightPos;
	glm::vec3 cubePos;

	glm::vec3 subSavedLocation;

	double deltaTime;
	double lastFrame;

	void ProcessInput();
	void RenderScene();
	void RenderSkyboxAndLight();
	void InitializeFish(int numFish);
	void UpdateFish(float deltaTime);

	void DrawJellyFish(const glm::mat4& baseModel, Shader* shader, Model* jellyFishModel);
	void DrawClownFish(const glm::mat4& baseModel, Shader* shader, Model* clownFishModel, float animationTime);
	void DrawAngelFish(const glm::mat4& baseModel, Shader* shader, Model* angelFishModel, float animationTime);
	void DrawKoiFish(const glm::mat4& baseModel, Shader* shader, Model* koiFishModel, float animationTime);
	void DrawFish(const std::vector<glm::vec3>& fishPositions,
		const std::vector<glm::vec3>& fishVelocities,
		const std::vector<float>& fishSwimAnimationTime,
		Shader* shader,
		Model* jellyFishModel,
		Model* clownFishModel,
		Model* angelFishModel,
		Model* koiFishModel);

	void RenderObjects(Shader* shader);
	void SetupBuffers();
	void SetupShaders();
	void LoadModels();

	void MouseCallback(double xpos, double ypos);
};
