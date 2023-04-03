#version 450 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

out gl_PerVertex { vec4 gl_Position; };

void main()
{
    gl_Position = vec4(aPos.xy, 0, 1);
    TexCoord    = aTexCoord;
}