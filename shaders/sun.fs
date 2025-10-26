#version 330
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{
    // Ignore all lighting, just output the texture color.
    // FragColor = texture(texture_diffuse1, TexCoords);
    FragColor = vec4(1.0);
}
