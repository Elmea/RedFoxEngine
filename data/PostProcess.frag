#version 450 core

in vec2 TexCoords;

uniform sampler2D screenTexture;

layout(std430, binding = 0) buffer KernelBlock 
{
    mat4 kernels[];
};

void main()
{
    vec3 color = vec3(0.0, 0.0, 0.0);

    for(int i = 0; i < kernels.length(); i++)
    {
        for (int j = 0; j < 9; j++)
        {
            vec3 sampleText = vec3(texture(screenTexture, TexCoords));
            int l = j / 3;
            color += sampleText * kernels[i][j%3][l];
        }
    }
}