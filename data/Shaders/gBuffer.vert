#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in unsigned int aMaterialID;

layout(std430, binding = 0) buffer MatrixBlock 
{
    mat4 worldMatrix[];
};

layout (location = 0) uniform mat4 vp;

out VS_OUT
{
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
    flat unsigned int materialID;
} vs_out;

out gl_PerVertex { vec4 gl_Position; };

void main()
{
    vec4 worldPosition = worldMatrix[gl_InstanceID] * vec4(aPos, 1);
    vs_out.Position = worldPosition.xyz;
    vs_out.TexCoords = aTexCoord;
    vs_out.Normal   = (transpose(inverse(worldMatrix[gl_InstanceID])) *
                                 vec4(aNormal, 1)).rgb;
    vs_out.materialID = aMaterialID;
    gl_Position = vp * worldPosition;
}
