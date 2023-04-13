#version 450 core
layout (location = 0) in vec3 aPos;

layout(std430, binding = 0) buffer MatrixBlock 
{
    mat4 worldMatrix[];
};

layout (location = 0) uniform mat4 VP;

out gl_PerVertex { vec4 gl_Position; };

void main()
{
    vec4 worldPosition = worldMatrix[gl_InstanceID] * vec4(aPos, 1);
    gl_Position = worldPosition * VP;
}  