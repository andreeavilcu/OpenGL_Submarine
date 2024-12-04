#define MAX_PATH 260
#include <unistd.h>

#include <locale>
#include <codecvt>

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <math.h>

#include <GL/glew.h>

#define GLM_FORCE_CTOR_INIT
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#pragma comment (lib, "glfw3dll.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "OpenGL32.lib")

int main(int argc, const char * argv[]) {
    std::cout << "Hello, World!\n";
    return 0;
}
