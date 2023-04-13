#version 450 core

layout (location = 0) in vec3 vpoint;
layout (location = 1) in vec3 vnormal;

layout (location = 0)  uniform vec3 sunPos;
layout (location = 1)  uniform mat4 mvp;
layout (location = 2)  uniform mat3 starsRot;

out vec3 pos;
out vec3 sunNorm;
out vec3 starPos;
out gl_PerVertex { vec4 gl_Position; };

void main()
{
    gl_Position = mvp * vec4(vpoint, 1.0);
    pos = vpoint;
    sunNorm = normalize(sunPos);
    starPos = starsRot * normalize(pos);
}