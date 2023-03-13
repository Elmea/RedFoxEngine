#version 450 core

in VS_IN
{
    vec3 FragPosition;
    vec3 Normal;
    vec2 TexCoord;
} vs_in;

layout (binding=0)             // (from ARB_shading_language_420pack)
uniform sampler2D diffuseMap;  // texture unit binding 0

layout (location=0)
out vec4 o_color;              // output fragment data location 0

void main()
{
    o_color = texture(diffuseMap, vs_in.TexCoord);
}
