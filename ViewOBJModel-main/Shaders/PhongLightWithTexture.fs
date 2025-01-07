#version 330 core

in vec3 FragPos;                // Fragment position in world space
in vec3 Normal;                 // Normal vector
in vec2 TexCoords;              // Texture coordinates
in vec4 FragPosLightSpace;      // Position in light space
in vec3 ReflectDir;             // Reflection vector
in float Opacity;

out vec4 FragColor;

uniform sampler2D texture_diffuse1; // Diffuse texture
uniform samplerCube cubeMap;        // Cube map for reflections
uniform sampler2D shadowMap;        // Shadow map

uniform vec3 lightPos;              // Light position
uniform vec3 lightColor;            // Light color
uniform vec3 viewPos;               // Camera position

float ShadowCalculation(vec4 fragPosLightSpace) {
    // Transform from clip space to normalized device coordinates
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5; // Transform to [0,1] range

    // Get the closest depth value from the shadow map
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    // Bias to prevent shadow acne
    float bias = max(0.05 * (1.0 - dot(Normal, normalize(lightPos - FragPos))), 0.005);

    // Shadow determination
    float shadow = (currentDepth - bias > closestDepth) ? 1.0 : 0.0;

    // Return shadow factor
    return shadow;
}

void main() {
    // Texture color
    vec3 diffuseColor = texture(texture_diffuse1, TexCoords).rgb;

    // Ambient lighting
    vec3 ambient = 0.3 * lightColor * diffuseColor;

    // Diffuse lighting
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * diffuseColor;

    // Specular lighting
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, Normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = spec * lightColor * vec3(0.5);

    // Shadow calculation
    float shadow = ShadowCalculation(FragPosLightSpace) * 0.5;

    // Reflection
    vec3 reflection = texture(cubeMap, ReflectDir).rgb;

    // Combine results
    vec3 result = ambient + (1.0 - shadow * 0.7) * (diffuse + specular);
    FragColor = vec4(result, Opacity);
}
