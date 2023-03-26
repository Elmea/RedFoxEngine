#version 450 core

in vec2 TexCoord;

layout (binding=0)             // (from ARB_shading_language_420pack)
uniform sampler2D gPosition;  // texture unit binding 0
layout (binding=1)
uniform sampler2D gNormal;  // texture unit binding 1
layout (binding=2)
uniform sampler2D gAlbedo;  // texture unit binding 2

layout (location=0)
out vec4 o_color;    // output fragment data location 0

struct Light
{
    vec3 position;
    float cutOff;
    vec3 direction;
    float outerCutOff;

    vec3 ambient;
    float constant;
    vec3 diffuse;
    float linear;
    vec3 specular;
    float quadratic;
};

layout(std430, binding = 0) buffer LightBlock {
    Light light[1000];
} u_lightBlock;

void main()
{
    float ambientStrength = 0.1;
    float specularStrength = 0.5;
    float shininess = 32;

    vec3 FragPosition = texture(gPosition, TexCoord).xyz;
    vec3 Normal       = texture(gNormal, TexCoord).xyz;
    vec3 objectColor = texture(gAlbedo, TexCoord).rgb;

    vec3 result = vec3(0, 0, 0);
    for (int i = 0; i < 1000; i++)
    {
        float lightConstant  = u_lightBlock.light[i].constant;
        float lightLinear    = u_lightBlock.light[i].linear;
        float lightQuadratic = u_lightBlock.light[i].quadratic;
        vec3 lightPos   = u_lightBlock.light[i].position;
        vec3 lightColor = u_lightBlock.light[i].diffuse;
        vec3 ambient = ambientStrength * lightColor;
         // diffuse
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPosition);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;

        // specular
        vec3 viewDir = normalize(-FragPosition);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        vec3 specular = specularStrength * spec * lightColor;

        float distance    = length(lightPos - FragPosition);
        float attenuation = 1.0 / (lightConstant + lightLinear * distance +
        lightQuadratic * (distance * distance));

        ambient  *= attenuation;
        diffuse  *= attenuation;
        specular *= attenuation;
        result   += (ambient + diffuse + specular) * objectColor;
    }
    o_color = vec4(result, 1);
}
