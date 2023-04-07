#version 450 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoord;
layout(location = 3) in mat4 worldMatrix;

layout (location = 0) uniform mat4 vp;

out VS_OUT
{
    vec3 Position;
    vec2 TexCoords;
} vs_out;

out gl_PerVertex { vec4 gl_Position; };

void main()
{
    vec4 worldPosition = worldMatrix * vec4(Position, 1);
    vs_out.Position = worldPosition.xyz;
    vs_out.TexCoords = TexCoord;
    gl_Position = vp * worldPosition;
}
