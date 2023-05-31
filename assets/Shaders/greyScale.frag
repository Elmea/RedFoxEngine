#version 450 core

in vec2 TexCoords;

layout (binding = 1) uniform sampler2D screenTexture;

layout (location=0)
out vec4 o_color;

void main()
{
    vec4 FragColor = texture(screenTexture, TexCoords);
    float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
    FragColor = vec4(average, average, average, 1.0);
    o_color = FragColor;
}

