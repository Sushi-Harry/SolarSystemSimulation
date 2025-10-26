#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal; // Normal in model space
layout (location = 2) in vec2 aTexCoords;

// Outputs for the fragment shader
out vec3 FragPos;   // Fragment position in world space
out vec3 Normal;    // Normal in world space
out vec2 TexCoords;

// Uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Calculate world position and pass to fragment shader
    FragPos = vec3(model * vec4(aPos, 1.0));
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    Normal = normalize(normalMatrix * aNormal); 
    // Pass texture coordinates
    TexCoords = aTexCoords;
    // Calculate final clip space position
    gl_Position = projection * view * vec4(FragPos, 1.0);
}