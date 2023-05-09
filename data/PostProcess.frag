#version 450 core

in vec2 TexCoords;

layout (binding = 1) uniform sampler2D screenTexture;

layout(std430, binding = 0) buffer KernelBlock 
{
    mat4 kernels[];
};

layout (location=0)
out vec4 o_color;

const float offset = 1.0 / 300.0;  

const vec2 offsets[9] = vec2[](
    vec2(-offset,  offset), // top-left
    vec2( 0.0f,    offset), // top-center
    vec2( offset,  offset), // top-right
    vec2(-offset,  0.0f),   // center-left
    vec2( 0.0f,    0.0f),   // center-center
    vec2( offset,  0.0f),   // center-right
    vec2(-offset, -offset), // bottom-left
    vec2( 0.0f,   -offset), // bottom-center
    vec2( offset, -offset)  // bottom-right    
);

void main()
{
    if (kernels.length() == 0)
    {
        o_color = texture(screenTexture, TexCoords);
        return;
    }

    vec3 result = vec3(0.0, 0.0, 0.0);

    for(int i = 0; i < kernels.length(); i++)
    {
        vec3 sampleTex[9];
        for(int i = 0; i < 9; i++)
        {
            sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
        }
        
        int sampleIt = 0;
        for (int j = 0; j < 3; j++)
        {
            for (int l = 0; l < 3; l++)
            {
                result += sampleTex[sampleIt] * kernels[i][j][l];
                sampleIt++;
            }
        }
    }

    o_color = vec4(result, 1.0f);
}