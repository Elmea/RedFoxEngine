#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in mat4 worldMatrix;

out VS_OUT
{
    vec3 FragPosition;
    vec3 Normal;
    vec2 TexCoord;
} vs_out;

out gl_PerVertex { vec4 gl_Position; };

out vec3 lightPos;
layout (location=0)
uniform mat4 vp;
layout (location=1)
uniform mat4 model;

void main()
{
    lightPos = vec3(0, 4, 0);
    gl_Position            = (vp * model * worldMatrix) * vec4(aPos, 1);
    vs_out.FragPosition    = (model * worldMatrix * vec4(aPos, 1)).rgb;
    vs_out.Normal          = (transpose(inverse(mat4(model * worldMatrix))) * vec4(aNormal, 1)).rgb;
    vs_out.TexCoord        = aTexCoord;
}
