#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <unistd.h>
#include <codecvt>
#include <locale>
#include <string>

#include <GL/glew.h>

std::string getCurrentPath();

#ifdef __APPLE__

#include <glm/GLM.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>

#define MAX_PATH 260

std::wstring convertToWide(const char* input);
std::string getCurrentPath();

#else

#include <GLM.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <glfw3.h>

#include <Windows.h>

std::string getCurrentPath();

#endif

#pragma comment (lib, "glfw3dll.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "OpenGL32.lib")

#include "Models/Shader.h"
#include "Models/Model.h"
#include "Models/Camera.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

class SubmarineProgram {
private:
    static SubmarineProgram* instance;
    
    SubmarineProgram() = default;
    
public:
    ~SubmarineProgram();
    
    SubmarineProgram(const SubmarineProgram&) = delete;
    SubmarineProgram& operator=(const SubmarineProgram&) = delete;
    
    static SubmarineProgram* getInstance();

private:
    GLuint ProjMatrixLocation, ViewMatrixLocation, WorldMatrixLocation;
    
    unsigned int windowWidth = SCR_WIDTH;
    unsigned int windowHeight = SCR_HEIGHT;
    
    unsigned int cubeVAO, lightVAO, VBO;
        
    glm::vec3 lightPos;
    
    double deltaTime = 0.0f;
    double lastFrame = 0.0f;
    
    Shader* lightningShader = nullptr;
    Shader* lightningWithTextureShader = nullptr;
    Shader* lampShader = nullptr;
    
    Model* submarineObjModel = nullptr;
    Model* grassLawnObjModel = nullptr;
    
    GLFWwindow* window = nullptr;
    Camera* pCamera = nullptr;
        
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    
    void initializeProgram();
    void loadProgram();
    
    void runMainLoop();
    
    void loadShaders();
    void loadObjects();
    
public:
    void runProgram();
};
