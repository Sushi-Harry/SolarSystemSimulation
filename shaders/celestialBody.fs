#version 330

out vec4 FragColor;
in vec2 TexCoords;
in vec3 Normals;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

// uniform vec3 lightPos;
uniform vec3 viewPos;
// uniform vec3 lightColor;

void main(){

    vec3 lightPos = vec3(0.0, 0.0, 5.0);
    vec3 lightColor = vec3(1.0, 1.0, 1.0);

    // Ambient Lighting
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 color = texture(texture_diffuse1, TexCoords).rgb;
    vec3 norm = normalize(Normals);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * color;

    // Specular
    float shininess = 32.0;
    float specularStrength = texture(texture_specular1, TexCoords).r;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}