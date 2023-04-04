#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in mat4 worldMatrix;

layout (location = 0) uniform mat4 vp;

out VS_OUT
{
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
    // mat3 TBN;
} vs_out;

out gl_PerVertex { vec4 gl_Position; };

void main()
{
    vec4 worldPosition = worldMatrix * vec4(aPos, 1);
    vs_out.Position = worldPosition.xyz;
    vs_out.TexCoords = aTexCoord;
    vs_out.Normal   = (transpose(inverse(worldMatrix)) *
                                 vec4(aNormal, 1)).rgb;
    // mat3 normalMatrix = transpose(inverse(mat3(worldMatrix)));
    // vec3 normal = normalMatrix * aNormal;
    // vec3 tangent = normalMatrix * aTangent;
    // vec3 bitangent = cross(normal, tangent);

    // vs_out.TBN = transpose(mat3(t, b, n));
    gl_Position = vp * worldPosition;
}
