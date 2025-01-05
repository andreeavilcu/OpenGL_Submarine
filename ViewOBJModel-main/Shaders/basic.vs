#version 330 core

layout (location = 0) in vec3 aPos;  // Vertex position
layout (location = 1) in vec3 aNormal; // Normal (ignored here)

uniform mat4 model;      // Model transformation
uniform mat4 view;       // View transformation
uniform mat4 projection; // Projection transformation

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
