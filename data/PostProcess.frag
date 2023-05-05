#version 450 core

in vec2 TexCoords;

layout (binding = 1) uniform sampler2D screenTexture;

layout(std430, binding = 0) buffer KernelBlock 
{
    mat4 kernels[];
};

layout (location=0)
out vec4 o_color;

void main()
{
    vec3 result = vec3(0.0, 0.0, 0.0);

    for(int i = 0; i < kernels.length(); i++)
    {
        for (int j = 0; j < 9; j++)
        {
            vec3 sampleText = vec3(texture(screenTexture, TexCoords));
            int l = j / 3;
            result += sampleText * kernels[i][j%3][l];
        }
    }

    o_color = vec4(result, 1.0f);
}