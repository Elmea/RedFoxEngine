#version 450 core
layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 tex;
out vec2 TexCoords;

out gl_PerVertex { vec4 gl_Position; };
void main()
{
    gl_Position = vec4(vertex.xy, 0.0, 1.0);
    TexCoords = tex.xy;
}