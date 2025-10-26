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
