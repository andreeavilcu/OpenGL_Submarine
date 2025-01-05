#include "SubmarineProgram.h"

SubmarineProgram::SubmarineProgram(GLFWwindow* window) :
window(window),
deltaTime(0.0),
lastFrame(0.0),
lightPos(0.0f, 2.0f, 1.0f)
{
    camera = new Camera(1920, 1080, glm::vec3(0.0, 0.0, 3.0));
}

SubmarineProgram::~SubmarineProgram() {
    Cleanup();
}

void SubmarineProgram::Initialize() {
    SetupBuffers();
    SetupShaders();
    LoadModels();
    
    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, [](GLFWwindow* w, double x, double y) {
        static_cast<SubmarineProgram*>(glfwGetWindowUserPointer(w))->MouseCallback(x, y);
    });
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
}

void SubmarineProgram::SetupBuffers() {
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
    
    // skybox
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindVertexArray(cubeVAO);
    
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // light VAO
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glGenVertexArrays(1, &skyboxVAO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    
    glGenFramebuffers(1, &depthMapFBO);
    
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_SIZE, SHADOW_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SubmarineProgram::SetupShaders() {
#ifdef __APPLE__
    char currentDir[PATH_MAX];
    std::string currentPath;
    
    if (getcwd(currentDir, sizeof(currentDir)) != nullptr) {
        currentPath = std::string(currentDir);
    } else {
        perror("Eroare la obținerea directorului curent");
    }
#else
    wchar_t buffer[MAX_PATH];
    GetCurrentDirectoryW(MAX_PATH, buffer);
    std::wstring executablePath(buffer);
    std::wstring wscurrentPath = executablePath.substr(0, executablePath.find_last_of(L"\\/"));
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string currentPath = converter.to_bytes(wscurrentPath);
#endif
    
    lightingWithTextureShader = new Shader(
                                           (currentPath + "/Shaders/PhongLightWithTexture.vs").c_str(),
                                           (currentPath + "/Shaders/PhongLightWithTexture.fs").c_str()
                                           );
    
    lampShader = new Shader(
                            (currentPath + "/Shaders/Lamp.vs").c_str(),
                            (currentPath + "/Shaders/Lamp.fs").c_str()
                            );
    
    skyboxShader = new Shader(
                              (currentPath + "/Shaders/SkyboxSingleColor.vs").c_str(),
                              (currentPath + "/Shaders/SkyboxSingleColor.fs").c_str()
                              );
    
    shadowShader = new Shader(
                              (currentPath + "/Shaders/ShadowMapping.vs").c_str(),
                              (currentPath + "/Shaders/ShadowMapping.fs").c_str()
                              );
}

void SubmarineProgram::LoadModels() {
#ifdef __APPLE__
    char currentDir[PATH_MAX];
    std::string currentPath;
    
    if (getcwd(currentDir, sizeof(currentDir)) != nullptr) {
        currentPath = std::string(currentDir);
    } else {
        perror("Eroare la obținerea directorului curent");
    }
#else
    wchar_t buffer[MAX_PATH];
    GetCurrentDirectoryW(MAX_PATH, buffer);
    std::wstring executablePath(buffer);
    std::wstring wscurrentPath = executablePath.substr(0, executablePath.find_last_of(L"\\/"));
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string currentPath = converter.to_bytes(wscurrentPath);
#endif
    
    submarineModel = new Model(currentPath + "/Models/Submarine/Submarine.obj", false);
    terrainModel = new Model(currentPath + "/Models/terrain/terrain.obj", false);
}

void SubmarineProgram::MouseCallback(double xpos, double ypos)
{
    camera->MouseControl(xpos, ypos);
}

void SubmarineProgram::Run() {
    while (!glfwWindowShouldClose(window)) {
        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        lightPos.x = 2.5f * cos(glfwGetTime());
        lightPos.z = 2.5f * sin(glfwGetTime());
        
        ProcessInput();
        RenderScene();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void SubmarineProgram::ProcessInput() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->ProcessKeyboard(1, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->ProcessKeyboard(2, deltaTime);
    
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        if (camera->getFreeLook()) camera->ProcessKeyboard(3, deltaTime);
        else camera->ProcessKeyboard(7, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        if (camera->getFreeLook()) camera->ProcessKeyboard(4, deltaTime);
        else camera->ProcessKeyboard(8, deltaTime);
    }
    
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera->ProcessKeyboard(5, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera->ProcessKeyboard(6, deltaTime);
    
    static bool wasXPressed = false;
    
    bool xIsPressedNow = (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS);
    
    if (!xIsPressedNow && wasXPressed) {
        if (camera->getFreeLook()) camera->Set(1920, 1080, subSavedLocation);
        camera->changeFreeLook();
    }
    wasXPressed = xIsPressedNow;
    
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        if (camera->getFreeLook()) {
            camera->Set(1920, 1080, subSavedLocation);
            camera->changeFreeLook();
        }
        
        camera->Reset(1920, 1080);
    }
}

void SubmarineProgram::RenderScene() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glm::mat4 lightProjection, lightView, lightSpaceMatrix;
    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
    lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    lightSpaceMatrix = lightProjection * lightView;
    
    shadowShader->use();
    shadowShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
    
    glViewport(0, 0, SHADOW_SIZE, SHADOW_SIZE);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    RenderObjects(shadowShader);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    glViewport(0, 0, 1920, 1080);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    lightingWithTextureShader->use();
    lightingWithTextureShader->setFloat("ambient", 0.6);
    lightingWithTextureShader->SetVec3("objectColor", 1.0f, 1.0f, 1.0f);
    lightingWithTextureShader->SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
    lightingWithTextureShader->SetVec3("lightPos", lightPos);
    lightingWithTextureShader->SetVec3("viewPos", camera->GetPosition());
    lightingWithTextureShader->setInt("texture_diffuse1", 0);
    lightingWithTextureShader->setMat4("projection", camera->GetProjectionMatrix());
    lightingWithTextureShader->setMat4("view", camera->GetViewMatrix());
    lightingWithTextureShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
    lightingWithTextureShader->setInt("shadowMap", 1);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    
    RenderObjects(lightingWithTextureShader);
    
    glDepthFunc(GL_LEQUAL);
    skyboxShader->use();
    skyboxShader->setMat4("view", glm::mat4(glm::mat3(camera->GetViewMatrix())));
    skyboxShader->setMat4("projection", camera->GetProjectionMatrix());
    glBindVertexArray(skyboxVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    
    glDepthFunc(GL_LESS);
    lampShader->use();
    lampShader->setMat4("projection", camera->GetProjectionMatrix());
    lampShader->setMat4("view", camera->GetViewMatrix());
    glm::mat4 lightModel = glm::translate(glm::mat4(1.0), lightPos);
    lightModel = glm::scale(lightModel, glm::vec3(0.05f));
    lampShader->setMat4("model", lightModel);
    
    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void SubmarineProgram::RenderObjects(Shader* shader) {
    glm::mat4 submarineModelMatrix = glm::mat4(1.f);
    
    if (!camera->getFreeLook()) {
        subSavedLocation = camera->GetPosition();
    }
    
    submarineModelMatrix = glm::translate(glm::mat4(1.f), subSavedLocation);
    
    glm::vec3 forward = camera->GetForward();
    glm::vec3 up = camera->GetUp();
    glm::mat4 rotationMatrix = glm::inverse(glm::lookAt(glm::vec3(0.0f), forward, up));
    submarineModelMatrix *= rotationMatrix;
    
    submarineModelMatrix = glm::translate(submarineModelMatrix, glm::vec3(0.0f, -.5f, -1.0f));
    submarineModelMatrix = glm::rotate(submarineModelMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    submarineModelMatrix = glm::rotate(submarineModelMatrix, glm::radians(-6.0f), glm::vec3(1.0f, 0.0f, .5f));
    
    shader->setMat4("model", submarineModelMatrix);
    submarineModel->Draw(*shader);
    
    glm::mat4 terrainMatrix = glm::translate(glm::mat4(1.f), glm::vec3(0.0f, 5.0f, 15.f));
    terrainMatrix = glm::scale(terrainMatrix, glm::vec3(.1f, .1f, .1f));
    shader->setMat4("model", terrainMatrix);
    terrainModel->Draw(*shader);
}

void SubmarineProgram::Cleanup() {
    delete camera;
    delete lightingWithTextureShader;
    delete lampShader;
    delete submarineModel;
    
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
}
