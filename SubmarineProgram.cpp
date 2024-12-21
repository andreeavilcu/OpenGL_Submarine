#include "SubmarineProgram.h"

#ifdef __APPLE__

std::wstring convertToWide(const char* input) {
    size_t length = std::strlen(input) + 1;
    std::wstring wideString(length, L'\0');
    std::mbstowcs(&wideString[0], input, length);
    return wideString;
}

std::string getCurrentPath() {
    wchar_t buffer[MAX_PATH];
    char charBuf[MAX_PATH];
    
    if (getcwd(charBuf, sizeof(charBuf)) != nullptr) {
        std::wstring widePath = convertToWide(charBuf);
        std::wcsncpy(buffer, widePath.c_str(), MAX_PATH);

        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.to_bytes(buffer);
    } else {
        perror("Eroare la obținerea directorului curent");
        return "";
    }
}

#else

std::string getCurrentPath() {
    wchar_t buffer[MAX_PATH];
    char charBuf[MAX_PATH];

    GetCurrentDirectoryW(MAX_PATH, buffer);

    std::wstring executablePath(buffer);
    std::wstring wscurrentPath = executablePath.substr(0, executablePath.find_last_of(L"\\/"));

    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string currentPath = converter.to_bytes(wscurrentPath);

    return currentPath;
}

#endif


SubmarineProgram* SubmarineProgram::instance = nullptr;

SubmarineProgram* SubmarineProgram::getInstance() {
    if (instance == nullptr) instance = new SubmarineProgram();
    return instance;
}

SubmarineProgram::~SubmarineProgram() {
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
    
    glfwTerminate();
        
    delete instance->pCamera;
    
    delete instance->lightningShader;
    delete instance->lightningWithTextureShader;
    delete instance->lampShader;

    delete instance->submarineObjModel;
    delete instance->grassLawnObjModel;
}

void SubmarineProgram::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    instance->pCamera->Reshape(width, height);
}

void SubmarineProgram::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    instance->pCamera->MouseControl((float)xpos, (float)ypos);
}

void SubmarineProgram::scroll_callback(GLFWwindow* window, double xoffset, double yOffset)
{
    instance->pCamera->ProcessMouseScroll((float)yOffset);
}

void SubmarineProgram::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        instance->pCamera->ProcessKeyboard(FORWARD, (float)instance->deltaTime);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        instance->pCamera->ProcessKeyboard(BACKWARD, (float)instance->deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        instance->pCamera->ProcessKeyboard(LEFT, (float)instance->deltaTime);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        instance->pCamera->ProcessKeyboard(RIGHT, (float)instance->deltaTime);
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        instance->pCamera->ProcessKeyboard(UP, (float)instance->deltaTime);
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        instance->pCamera->ProcessKeyboard(DOWN, (float)instance->deltaTime);
    
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        instance->pCamera->Reset(width, height);
    }
}

void SubmarineProgram::initializeProgram() {
    if (instance->window != nullptr) {
        std::cout << "Window already initialized!" << std::endl;
        return;
    }
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow* window = glfwCreateWindow(instance->windowWidth, instance->windowHeight, "OpenGL_Submarine", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    
    instance->window = window;
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    
    glewInit();
    
    glEnable(GL_DEPTH_TEST);
}

void SubmarineProgram::loadProgram() {
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    
    glGenVertexArrays(1, &instance->cubeVAO);
    glGenBuffers(1, &instance->VBO);

    glBindBuffer(GL_ARRAY_BUFFER, instance->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(instance->cubeVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenVertexArrays(1, &instance->lightVAO);
    glBindVertexArray(instance->lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, instance->VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    instance->pCamera = new Camera(instance->windowWidth, instance->windowHeight, glm::vec3(0.0, 0.0, 3.0));
    
    instance->lightPos = glm::vec3(0.0f, 2.0f, 1.0f);
    
    instance->loadShaders();
    instance->loadObjects();
}

void SubmarineProgram::runMainLoop() {
    while (!glfwWindowShouldClose(instance->window)) {
        double currentFrame = glfwGetTime();
        instance->deltaTime = currentFrame - instance->lastFrame;
        instance->lastFrame = currentFrame;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        instance->lightPos.x = 2.5 * cos(glfwGetTime());
        instance->lightPos.z = 2.5 * sin(glfwGetTime());

        lightningShader->use();
        lightningShader->SetVec3("objectColor", 0.5f, 1.0f, 0.31f);
        lightningShader->SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightningShader->SetVec3("lightPos", lightPos);
        lightningShader->SetVec3("viewPos", pCamera->GetPosition());
        lightningShader->setMat4("projection", pCamera->GetProjectionMatrix());
        lightningShader->setMat4("view", pCamera->GetViewMatrix());

        lightningWithTextureShader->use();
        lightningWithTextureShader->SetVec3("objectColor", 0.5f, 1.0f, 0.31f);
        lightningWithTextureShader->SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightningWithTextureShader->SetVec3("lightPos", instance->lightPos);
        lightningWithTextureShader->SetVec3("viewPos", instance->pCamera->GetPosition());
        lightningWithTextureShader->setInt("texture_diffuse1", 0);
        lightningWithTextureShader->setMat4("projection", instance->pCamera->GetProjectionMatrix());
        lightningWithTextureShader->setMat4("view", instance->pCamera->GetViewMatrix());

        glm::mat4 grassLawnModelMatrix = glm::scale(glm::mat4(1.f), glm::vec3(1000, 1, 1000));
        lightningWithTextureShader->setMat4("model", grassLawnModelMatrix);
        grassLawnObjModel->Draw(*(instance->lightningWithTextureShader));
        
        glm::mat4 submarineModelMatrix = glm::mat4(1.f);
        lightningWithTextureShader->setMat4("model", submarineModelMatrix);
        submarineObjModel->Draw(*(instance->lightningWithTextureShader));

        lampShader->use();
        lampShader->setMat4("projection", instance->pCamera->GetProjectionMatrix());
        lampShader->setMat4("view", instance->pCamera->GetViewMatrix());
        glm::mat4 lightModel = glm::translate(glm::mat4(1.0), instance->lightPos);
        lightModel = glm::scale(lightModel, glm::vec3(0.05f));
        lampShader->setMat4("model", lightModel);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(instance->window);
        glfwPollEvents();
    }
}

void SubmarineProgram::loadShaders() {
    std::string currentPath = getCurrentPath();
    
    instance->lightningShader = new Shader((currentPath + "/Shaders/PhongLight.vs").c_str(),
                          (currentPath + "/Shaders/PhongLight.fs").c_str());
    
    instance->lightningWithTextureShader = new Shader((currentPath + "/Shaders/PhongLightWithTexture.vs").c_str(),
                                     (currentPath + "/Shaders/PhongLightWithTexture.fs").c_str());
    
    instance->lampShader = new Shader((currentPath + "/Shaders/Lamp.vs").c_str(),
                      (currentPath + "/Shaders/Lamp.fs").c_str());
}

void SubmarineProgram::loadObjects() {
    std::string currentPath = getCurrentPath();
    
    std::string submarineObjFileName = (currentPath + "/Objects/Submarine.obj");
    instance->submarineObjModel = new Model(submarineObjFileName, false);

    std::string grassLawnObjFileName = (currentPath + "/Objects/GrassLawn/GrassLawn.obj");
    instance->grassLawnObjModel = new Model(grassLawnObjFileName, false);
}

void SubmarineProgram::runProgram() {
    instance->initializeProgram();
    instance->loadProgram();
    
    instance->runMainLoop();
}
