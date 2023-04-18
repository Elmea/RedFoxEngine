#version 450 core

layout (location = 0) in vec3 vpoint;
layout (location = 0) uniform vec3 sunPos;
layout (location = 1) uniform mat4 mvp;

out vec3 pos;
out vec3 sunPosNorm;

out gl_PerVertex { vec4 gl_Position; };

void main()
{
    gl_Position = mvp * vec4(vpoint, 1);
    pos = vpoint;
    sunPosNorm = normalize(sunPos);
}