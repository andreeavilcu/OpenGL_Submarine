#pragma once
#include <GL/glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Model.h"
#include "FlyingCube.h"
#include "Camera.h"

class SubmarineProgram {
public:
	SubmarineProgram(GLFWwindow* window);
	~SubmarineProgram();

	void Initialize();
	void Run();
	void Cleanup();

private:
	GLFWwindow* window;
	Camera* camera;

	unsigned int VBO, cubeVAO, lightVAO;
	Shader* lightingShader, * lightingWithTextureShader, * lampShader;
	FlyingCube* flyingCubeModel;
	Model* piratModel, * grassLawnModel, * helicopterModel;

	glm::vec3 lightPos;
	glm::vec3 cubePos;

	double deltaTime;
	double lastFrame;

	void ProcessInput();
	void RenderScene();
	void SetupBuffers();
	void SetupShaders();
	void LoadModels();
};

