#version 330
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct PointLight{
    bool enabled;
    vec3 position;
    vec3 color;
    
    // Attribute Strength
    float specular;
    float diffuse;
    float ambient;

    // Attenuation
    float constant, linear, quadratic;
};

uniform PointLight pntLight;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform float shininess;

uniform vec3 viewPos;

vec3 CalcPointLight(PointLight pointLight, vec3 normal, vec3 viewDir, vec2 texCoords){
    vec3 lightDir = normalize(pointLight.position - FragPos);
    float dist = length(pointLight.position - FragPos);
    float attenuation = 1.0 / (pointLight.constant + (pointLight.linear * dist) + (pointLight.quadratic * dist * dist));
    vec3 reflectDir = reflect(-lightDir, normal);

    // ambient
    vec3 ambient = pointLight.color * pointLight.ambient * texture(texture_diffuse1, texCoords).rgb;

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = pointLight.color * pointLight.diffuse * diff * texture(texture_diffuse1, texCoords).rgb;

    // Specular
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = pointLight.color * spec * pointLight.specular * texture(texture_specular1, texCoords).rgb;

    if(pointLight.enabled)
        return (ambient + specular + diffuse) * attenuation;
    else
        return ambient;
}

void main()
{    
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0);

    result += CalcPointLight(pntLight, norm, viewDir, TexCoords);

    FragColor = vec4(result, 1.0);
    // FragColor = texture(texture_diffuse1, TexCoords);
    // FragColor = vec4(1.0);
    // FragColor = vec4(pntLight.linear, 0.0, 0.0, 1.0);
}




// #version 330 core
// out vec4 FragColor;

// // We still need the inputs from the vertex shader
// in vec3 Normal;
// in vec3 FragPos;
// in vec2 TexCoords;

// // We still declare the uniforms so we can test them
// struct PointLight {
//     vec3 position;
//     vec3 color;
//     float ambient;
//     float constant, linear, quadratic;
// };

// uniform PointLight pntLight;
// uniform sampler2D texture_diffuse1;
// uniform vec3 viewPos;

// void main()
// {
//     //-------------------------------------------------------------------------
//     // Instructions: Uncomment ONE test at a time to check a specific uniform.
//     //-------------------------------------------------------------------------

//     // --- TEST 1: Are the Normals Correct? ---
//     // Normals are in the range [-1, 1]. We remap them to [0, 1] to see them as colors.
//     // vec3 norm = normalize(Normal);
//     // FragColor = vec4(norm * 0.5 + 0.5, 1.0);
    
//     // --- TEST 2: Is the Diffuse Texture Loaded and Correctly Mapped? ---
//     // If this works, your texture loader and TexCoords are correct.
//     // FragColor = texture(texture_diffuse1, TexCoords);

//     // --- TEST 3: Is the Light Direction Correct? ---
//     // This calculates basic diffuse lighting. If you see this, your light's
//     // position and the normals are working together correctly.
//     // vec3 norm = normalize(Normal);
//     // vec3 lightDir = normalize(pntLight.position - FragPos);
//     // float diff = max(dot(norm, lightDir), 0.0);
//     // FragColor = vec4(diff, diff, diff, 1.0);

//     // --- TEST 4: Is the Light Color Being Passed Correctly? ---
//     // The entire sphere should be a solid, bright color (e.g., white).
//     FragColor = vec4(pntLight.color, 1.0);

//     // --- TEST 5: Is Attenuation Working? ---
//     // The part of the sphere closer to the sun should be bright white, fading to black.
//     // float dist = length(pntLight.position - FragPos);
//     // float attenuation = 1.0 / (pntLight.constant + (pntLight.linear * dist) + (pntLight.quadratic * dist * dist));
//     // FragColor = vec4(vec3(attenuation), 1.0);
// }
