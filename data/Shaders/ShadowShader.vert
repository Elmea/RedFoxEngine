#version 450 core
layout (location = 0) in vec3 aPos;
layout(location = 3) in mat4 worldMatrix;

layout (location = 1) uniform mat4 VP;

out gl_PerVertex { vec4 gl_Position; };

void main()
{
    vec4 worldPosition = worldMatrix * vec4(aPos, 1);
    gl_Position = worldPosition * VP;
}  