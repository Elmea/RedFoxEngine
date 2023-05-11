#version 450 core

in vec2 TexCoords;

layout (binding = 1) uniform sampler2D screenTexture;

layout (location=0)
out vec4 o_color;

void main()
{
    vec4 FragColor = texture(screenTexture, TexCoords);
    vec3 unit = vec3(1.0f, 1.0f, 1.0f);
    o_color = vec4(1 - FragColor.xyz, FragColor.w);
}