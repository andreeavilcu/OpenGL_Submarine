#include "SubmarineProgram.h"

SubmarineProgram::SubmarineProgram(GLFWwindow* window) :
    window(window),
    deltaTime(0.0),
    lastFrame(0.0),
    lightPos(0.0f, 2.0f, 1.0f)
{
    camera = new Camera(800, 600, glm::vec3(0.0, 0.0, 3.0));
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
    std::wstring wscurrentPath = executablePath.substr(0, executablePath.find_last_of(L"//"));
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

    std::string submarineObjFileName = currentPath + "/Models/Submarine/Submarine.obj";
    submarineModel = new Model(submarineObjFileName, false);

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
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->ProcessKeyboard(3, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->ProcessKeyboard(4, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        camera->ProcessKeyboard(5, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        camera->ProcessKeyboard(6, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera->ProcessKeyboard(7, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera->ProcessKeyboard(8, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        camera->Reset(width, height);
    }
}

void SubmarineProgram::RenderScene() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    lightPos.x = 2.5f * cos(glfwGetTime());
    lightPos.z = 2.5f * sin(glfwGetTime());
    
    glDepthFunc(GL_LEQUAL);
    skyboxShader->use();
    skyboxShader->setMat4("view", glm::mat4(glm::mat3(camera->GetViewMatrix())));
    skyboxShader->setMat4("projection", camera->GetProjectionMatrix());
    glBindVertexArray(skyboxVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    
    glDepthFunc(GL_LESS);
    lightingWithTextureShader->use();
    lightingWithTextureShader->setFloat("ambient", 0.6);
    lightingWithTextureShader->SetVec3("objectColor", 0.5f, 1.0f, 0.31f);
    lightingWithTextureShader->SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
    lightingWithTextureShader->SetVec3("lightPos", lightPos);
    lightingWithTextureShader->SetVec3("viewPos", camera->GetPosition());
    lightingWithTextureShader->setInt("texture_diffuse1", 0);
    lightingWithTextureShader->setMat4("projection", camera->GetProjectionMatrix());
    lightingWithTextureShader->setMat4("view", camera->GetViewMatrix());
    
    glm::mat4 submarineModelMatrix = glm::translate(glm::mat4(1.f), camera->GetPosition());
    submarineModelMatrix = glm::translate(submarineModelMatrix, glm::vec3(0.0f, -.5f, -2.0f));
    submarineModelMatrix = glm::rotate(submarineModelMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    submarineModelMatrix = glm::rotate(submarineModelMatrix, glm::radians(-3.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    lightingWithTextureShader->setMat4("model", submarineModelMatrix);
    submarineModel->Draw(*lightingWithTextureShader);

    lampShader->use();
    lampShader->setMat4("projection", camera->GetProjectionMatrix());
    lampShader->setMat4("view", camera->GetViewMatrix());
    glm::mat4 lightModel = glm::translate(glm::mat4(1.0), lightPos);
    lightModel = glm::scale(lightModel, glm::vec3(0.05f));
    lampShader->setMat4("model", lightModel);

    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
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
