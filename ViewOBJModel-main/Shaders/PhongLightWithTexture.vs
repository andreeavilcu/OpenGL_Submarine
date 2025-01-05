#version 330 core

layout(location = 0) in vec3 aPos;          // Vertex position
layout(location = 1) in vec3 aNormal;       // Vertex normal
layout(location = 2) in vec2 aTexCoords;   // Texture coordinates

out vec3 FragPos;                          // Fragment position in world space
out vec3 Normal;                           // Normal vector
out vec2 TexCoords;                        // Texture coordinates
out vec4 FragPosLightSpace;                // Position in light space
out vec3 ReflectDir;                       // Reflection vector

uniform mat4 model;                        // Model matrix
uniform mat4 view;                         // View matrix
uniform mat4 projection;                   // Projection matrix
uniform mat4 lightSpaceMatrix;             // Light-space matrix (light view * light projection)
uniform vec3 viewPos;                      // Camera position

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0)); // World-space position
    Normal = mat3(transpose(inverse(model))) * aNormal; // Transform normal to world space
    TexCoords = aTexCoords;

    // Calculate light-space position
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);

    // Calculate reflection vector
    vec3 I = normalize(FragPos - viewPos); // Incident vector
    ReflectDir = reflect(I, normalize(Normal));

    // Transform position to clip space
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
