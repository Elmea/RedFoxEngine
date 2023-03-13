#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out VS_OUT
{
    vec3 FragPosition;
    vec3 Normal;
    vec2 TexCoord;
} vs_out;

out gl_PerVertex { vec4 gl_Position; };

layout (location=0)
uniform mat4 vp;
layout (location=1)
uniform mat4 model;

void main()
{
    gl_Position            = (vp * model) * vec4(aPos, 1);
    vs_out.FragPosition    = (model * vec4(aPos, 1)).rgb;
    vs_out.Normal          = (transpose(inverse(mat4(model))) * vec4(aNormal, 1)).rgb;
    vs_out.TexCoord        = aTexCoord;
}
