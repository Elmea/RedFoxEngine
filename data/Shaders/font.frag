
#version 450 core
in vec2 TexCoords;
layout (location=0)
out vec4 color;    // output fragment data location 0

layout (binding=0)
uniform sampler2D text;


void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = vec4(1.0, 1.0, 1.0, 1.0) * sampled;
} 