#version 330 core

layout(location = 0) in vec3 aPos; // Vertex position
layout(location = 1) in vec3 aNormal; // Vertex normal (optional, not used here)

uniform mat4 model;            // Model matrix
uniform mat4 lightSpaceMatrix; // Light's projection * view matrix

void main() {
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}
