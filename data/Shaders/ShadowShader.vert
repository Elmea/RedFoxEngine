#version 450 core
layout (location = 0) in vec3 aPos;

layout (location = 1) uniform mat4 VP;
layout (location = 2) uniform mat4 model;

out gl_PerVertex { vec4 gl_Position; };

void main()
{
    gl_Position = VP * model * vec4(aPos, 1.0);
}  