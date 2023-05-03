
#version 450 core
in vec2 TexCoords;
layout (location=0)
out vec4 color;    // output fragment data location 0

layout (binding=0)
uniform sampler2D text;
layout (location=0)
uniform vec3 u_color;
layout (location=2)
uniform vec3 u_selectedColor;
layout (location=1)
uniform bool isText;


void main()
{    
    if (isText)
    {
        vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
        color = vec4(u_color, 1.0) * sampled;
    }
    else
    {
        color.xyz = u_selectedColor;
        color.w = 1;
    }
} 