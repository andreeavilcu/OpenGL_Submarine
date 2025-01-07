#include "SubmarineProgram.h"

SubmarineProgram::SubmarineProgram(GLFWwindow* window) :
    window(window),
    deltaTime(0.0),
    lastFrame(0.0),
    lightPos(0.0f, 2.0f, 1.0f)
{
    camera = new Camera(1920, 1080, glm::vec3(0.f, -50.f, -100.f));
}

SubmarineProgram::~SubmarineProgram() {
    Cleanup();
}

void SubmarineProgram::Initialize() {
    SetupBuffers();
    SetupShaders();
    LoadModels();
    InitializeFish(60);

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
    }
    else {
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
    }
    else {
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
    jellyFishModel = new Model(currentPath + "/Models/JellyFish/jellyfish.obj", false);
    clownFishModel = new Model(currentPath + "/Models/ClownFish/clownfish.obj", false);
    koiFishModel = new Model(currentPath + "/Models/KoiFish/koifish.obj", false);
    angelFishModel = new Model(currentPath + "/Models/AngelFish/angelfish.obj", false);
    sunModel = new Model(currentPath + "/Models/Moon/Moon.obj", false);
    waterModel = new Model(currentPath + "/Models/Water/Water.obj", false);

    waterTextureID = CreateTexture(currentPath + "/Models/Water/image0.png");
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

        lightPos = glm::vec3(0.f, 200.f, -100.f);

        UpdateFish(deltaTime);
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

    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
        day = false;
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
        day = true;

    static bool wasXPressed = false;

    bool xIsPressedNow = (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS);

    if (!xIsPressedNow && wasXPressed) {
        if (camera->getFreeLook()) 
            camera->Set(1920, 1080, subSavedLocation);
        if(camera->thirdPerson())
            camera->changeFreeLook();
    }
    wasXPressed = xIsPressedNow;
    
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && !camera->getFreeLook())
        camera->setCameraMode(1);
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && !camera->getFreeLook())
        camera->setCameraMode(2);
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && !camera->getFreeLook())
        camera->setCameraMode(3);

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
    glViewport(0, 0, 1920, 1080);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    RenderSkyboxAndLight();

    glm::mat4 lightProjection, lightView, lightSpaceMatrix;
    float shadowNear = 0.1f;
    float shadowFar = 50.0f;
    float shadowSize = 25.0f;

    lightProjection = glm::ortho(
        -shadowSize, shadowSize,
        -shadowSize, shadowSize,
        shadowNear, shadowFar
    );

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
    lightingWithTextureShader->SetVec3("objectColor", 1.0f, 1.0f, 1.0f);
    if (day)
        lightingWithTextureShader->SetVec3("lightColor", 1.f, 1.f, 1.f);
    else
        lightingWithTextureShader->SetVec3("lightColor", 0.3f, 0.3f, 0.3f);
    lightingWithTextureShader->SetVec3("lightPos", lightPos);
    lightingWithTextureShader->SetVec3("viewPos", camera->GetPosition());
    lightingWithTextureShader->setInt("texture_diffuse1", 0);
    lightingWithTextureShader->setMat4("projection", camera->GetProjectionMatrix());
    lightingWithTextureShader->setMat4("view", camera->GetViewMatrix());
    lightingWithTextureShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
    lightingWithTextureShader->setInt("shadowMap", 1);
    lightingWithTextureShader->setFloat("opacity", 1);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);

    RenderObjects(lightingWithTextureShader);
}

void SubmarineProgram::InitializeFish(int numFish) {
    glm::vec3 centerPoint = glm::vec3(0.f, -70.f, -100.f);
    const float minDistance = 15.0f;
    const float spawnRadius = 100.0f;

    // Define strict height limits
    const float maxHeight = -40.f; // Upper height limit (higher in world space)
    const float minHeight = -90.f; // Lower height limit
    const float heightRange = minHeight - maxHeight;

    for (int i = 0; i < numFish; ++i) {
        bool validPosition = false;
        glm::vec3 newPosition;

        while (!validPosition) {
            // Generate random height within strict bounds
            float randomHeight = minHeight + static_cast<float>(rand()) / RAND_MAX * (maxHeight - minHeight);

            newPosition = glm::vec3(
                centerPoint.x + (rand() % (int)(2 * spawnRadius) - spawnRadius),
                randomHeight, // Use the calculated height
                centerPoint.z + (rand() % (int)(2 * spawnRadius) - spawnRadius)
            );

            // Check distance from all existing fish
            validPosition = true;
            for (const auto& existingPos : fishPositions) {
                if (glm::length(newPosition - existingPos) < minDistance) {
                    validPosition = false;
                    break;
                }
            }
        }

        fishPositions.push_back(newPosition);

        // Reduce vertical velocity component to minimize height changes
        fishVelocities.push_back(glm::vec3(
            (rand() % 100 - 50) / 300.0f,
            (rand() % 100 - 50) / 500.0f, // Reduced vertical velocity
            (rand() % 100 - 50) / 300.0f
        ));

        fishSwimAnimationTime.push_back(static_cast<float>(rand()) / RAND_MAX * 2.0f * 3.14159f);
        fishVerticalAnimationTime.push_back(static_cast<float>(rand()) / RAND_MAX * 2.0f * 3.14159f);
    }
}


void SubmarineProgram::UpdateFish(float deltaTime) {
    glm::vec3 centerPoint = glm::vec3(0.f, -70.f, -100.f);
    float maxDistance = 150.0f;

    // Use the same height limits as in initialization
    const float maxHeight = -50.f;
    const float minHeight = -80.f;

    const float swimFrequency = 3.0f;
    const float verticalFrequency = 1.2f;

    for (size_t i = 0; i < fishPositions.size(); ++i) {
        fishSwimAnimationTime[i] += deltaTime * swimFrequency;
        fishVerticalAnimationTime[i] += deltaTime * verticalFrequency;

        // Store previous position
        glm::vec3 previousPosition = fishPositions[i];

        if (i % 2 != 0) { // For clownfish
            float verticalOffset = sin(fishVerticalAnimationTime[i]) * 0.01f;
            fishPositions[i].y += verticalOffset;
        }

        fishPositions[i] += fishVelocities[i] * deltaTime;

        // Strict height enforcement
        if (fishPositions[i].y > maxHeight) {
            fishPositions[i].y = maxHeight;
            fishVelocities[i].y *= -1.0f; // Reverse vertical velocity
        }
        else if (fishPositions[i].y < minHeight) {
            fishPositions[i].y = minHeight;
            fishVelocities[i].y *= -1.0f; // Reverse vertical velocity
        }

        // Horizontal boundary check
        glm::vec2 horizontalOffset = glm::vec2(fishPositions[i].x - centerPoint.x,
            fishPositions[i].z - centerPoint.z);
        float horizontalDistance = glm::length(horizontalOffset);

        if (horizontalDistance > maxDistance) {
            // Only adjust horizontal components
            glm::vec2 directionToCenter = glm::normalize(-horizontalOffset);
            fishVelocities[i].x = directionToCenter.x * glm::length(fishVelocities[i]);
            fishVelocities[i].z = directionToCenter.y * glm::length(fishVelocities[i]);
        }

        // Gradually reduce extreme vertical velocities
        if (abs(fishVelocities[i].y) > 0.1f) {
            fishVelocities[i].y *= 0.98f;
        }
    }
}

void SubmarineProgram::DrawJellyFish(const glm::mat4& baseModel, Shader* shader, Model* jellyFishModel)
{
    glm::mat4 model = glm::scale(baseModel, glm::vec3(3.f));
    shader->setMat4("model", model);
    jellyFishModel->Draw(*shader);
}

void SubmarineProgram::DrawClownFish(const glm::mat4& baseModel, Shader* shader, Model* clownFishModel, float animationTime)
{
    glm::mat4 model = baseModel;

    // Tail swishing motion
    float tailSwing = sin(animationTime) * 0.2f;
    model = glm::rotate(model, tailSwing, glm::vec3(0.0f, 1.0f, 0.0f));

    // Slight rolling motion
    float rollAngle = sin(animationTime * 0.5f) * 0.1f;
    model = glm::rotate(model, rollAngle, glm::vec3(0.0f, 0.0f, 1.0f));

    model = glm::scale(model, glm::vec3(70.f));
    shader->setMat4("model", model);
    clownFishModel->Draw(*shader);
}

void SubmarineProgram::DrawAngelFish(const glm::mat4& baseModel, Shader* shader, Model* angelFishModel, float animationTime)
{
    glm::mat4 model = baseModel;

    // Tail swishing motion
    float tailSwing = sin(animationTime) * 0.2f;
    model = glm::rotate(model, tailSwing, glm::vec3(0.0f, 1.0f, 0.0f));

    // Slight rolling motion
    float rollAngle = sin(animationTime * 0.5f) * 0.1f;
    model = glm::rotate(model, rollAngle, glm::vec3(0.0f, 0.0f, 1.0f));

    model = glm::scale(model, glm::vec3(70.f));
    shader->setMat4("model", model);
    angelFishModel->Draw(*shader);
}

void SubmarineProgram::DrawKoiFish(const glm::mat4& baseModel, Shader* shader, Model* koiFishModel, float animationTime)
{
    glm::mat4 model = baseModel;

    // Tail swishing motion
    float tailSwing = sin(animationTime) * 0.2f;
    model = glm::rotate(model, tailSwing, glm::vec3(0.0f, 1.0f, 0.0f));

    // Slight rolling motion
    float rollAngle = sin(animationTime * 0.5f) * 0.1f;
    model = glm::rotate(model, rollAngle, glm::vec3(0.0f, 0.0f, 1.0f));

    model = glm::scale(model, glm::vec3(15.f));
    shader->setMat4("model", model);
    koiFishModel->Draw(*shader);
}

void SubmarineProgram::DrawFish(const std::vector<glm::vec3>& fishPositions, const std::vector<glm::vec3>& fishVelocities, const std::vector<float>& fishSwimAnimationTime, Shader* shader, Model* jellyFishModel, Model* clownFishModel, Model* angelFishModel, Model* koiFishModel)
{
    for (size_t i = 0; i < fishPositions.size(); i++) {
        glm::mat4 baseModel = glm::mat4(1.0f);
        baseModel = glm::translate(baseModel, fishPositions[i]);

        // Orient fish based on velocity
        if (glm::length(fishVelocities[i]) > 0) {
            glm::vec3 direction = glm::normalize(fishVelocities[i]);
            float rotationAngle = atan2(direction.x, direction.z);
            baseModel = glm::rotate(baseModel, rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
        }

        // Determine fish type and draw
        if (i % 2 == 0 && i % 3 != 0 && i % 4 != 0) {
            DrawJellyFish(baseModel, shader, jellyFishModel);
        }
        else if (i % 3 == 0 && i % 4 != 0) {
            DrawClownFish(baseModel, shader, clownFishModel, fishSwimAnimationTime[i]);
        }
        else if (i % 4 == 0) {
            DrawAngelFish(baseModel, shader, angelFishModel, fishSwimAnimationTime[i]);
        }
        else {
            DrawKoiFish(baseModel, shader, koiFishModel, fishSwimAnimationTime[i]);
        }
    }
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
    submarineModelMatrix = glm::scale(submarineModelMatrix, glm::vec3(10.0f)); // Adjust scale

    switch (camera->getCameraMode()) {
        case 1:
            submarineModelMatrix = glm::translate(submarineModelMatrix, glm::vec3(0.0f, -.5f, -1.0f));
            break;
        case 2:
            submarineModelMatrix = glm::translate(submarineModelMatrix, glm::vec3(0.0f, 0.f, -3.0f));
            submarineModelMatrix = glm::rotate(submarineModelMatrix, glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
            break;
        case 3:
            submarineModelMatrix = glm::translate(submarineModelMatrix, glm::vec3(0.0f, 0.05f, 0.35f));
            break;
    }
    

    submarineModelMatrix = glm::rotate(submarineModelMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    submarineModelMatrix = glm::rotate(submarineModelMatrix, glm::radians(-6.0f), glm::vec3(1.0f, 0.0f, .5f));

    shader->setMat4("model", submarineModelMatrix);
    submarineModel->Draw(*shader);

    glm::mat4 terrainMatrix = glm::mat4(1.0f);
    terrainMatrix = glm::translate(terrainMatrix, glm::vec3(0.0f, 0.0f, 0.0f)); // Adjust position
    terrainMatrix = glm::scale(terrainMatrix, glm::vec3(1.0f, 1.0f, 1.0f)); // Adjust scale
    shader->setMat4("model", terrainMatrix);
    terrainModel->Draw(*shader);

    DrawFish(fishPositions, fishVelocities, fishSwimAnimationTime, shader, jellyFishModel, clownFishModel, angelFishModel, koiFishModel);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::mat4 waterMatrix = glm::mat4(1.0f);
    waterMatrix = glm::translate(waterMatrix, glm::vec3(0.0f, -30.0f, -100.0f));
    waterMatrix = glm::scale(waterMatrix, glm::vec3(100.0f));
    shader->setMat4("model", waterMatrix);
    if (shader == lightingWithTextureShader)
        lightingWithTextureShader->setFloat("opacity", 0.5f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, waterTextureID);
    waterModel->Draw(*shader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_BLEND);
}

void SubmarineProgram::RenderSkyboxAndLight() {
    glDisable(GL_DEPTH_TEST);

   // glDepthFunc(GL_LEQUAL);
    glm::vec3 color = day ? glm::vec3(0.69f, 0.87f, 1.0f) : glm::vec3(0.1f, 0.2f, 0.3f);
    skyboxShader->use();
    skyboxShader->SetVec3("color", color);
    skyboxShader->setMat4("view", glm::mat4(glm::mat3(camera->GetViewMatrix())));
    skyboxShader->setMat4("projection", camera->GetProjectionMatrix());
    glBindVertexArray(skyboxVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);

    glDepthFunc(GL_LESS);
    lampShader->use();
    lampShader->setMat4("projection", camera->GetProjectionMatrix());
    lampShader->setMat4("view", camera->GetViewMatrix());
    glm::mat4 lightModel = glm::translate(glm::mat4(1.0), lightPos);
    lightModel = glm::scale(lightModel, glm::vec3(5.f));
    lampShader->setMat4("model", lightModel);
    
    sunModel->Draw(*lampShader);

}

void SubmarineProgram::Cleanup() {
    delete camera;
    delete lightingWithTextureShader;
    delete lampShader;
    delete submarineModel;
    delete jellyFishModel;
    delete clownFishModel;
    delete angelFishModel;
    delete koiFishModel;
    delete sunModel;
    delete waterModel;

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
}
