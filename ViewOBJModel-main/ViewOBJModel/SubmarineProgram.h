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
#else
#include <Windows.h>
#endif
#include <locale>
#include <codecvt>

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
	Model* submarineModel;

	glm::vec3 lightPos;
	glm::vec3 cubePos;

	double deltaTime;
	double lastFrame;

	void ProcessInput();
	void RenderScene();
	void SetupBuffers();
	void SetupShaders();
	void LoadModels();

	void MouseCallback(double xpos, double ypos);
};

