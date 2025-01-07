#version 330 core
layout (location = 0) in vec3 aPos;

in vec3 color;
out vec3 Color;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww; // Skybox-ul rămâne infinit
    Color = color;
}

