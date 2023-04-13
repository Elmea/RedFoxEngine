#version 450 core
#extension GL_ARB_bindless_texture : enable

struct ShadowParameters
{
    unsigned int depthMapFBO;
    int SHADOW_WIDTH, SHADOW_HEIGHT;
    unsigned int depthMap;
};

struct Light {
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

    mat4 lightVp;
    ShadowParameters shadowParameters;

    int index;
    int padding0;
    int padding1;
    int padding2;
};
struct Material
{
    vec3 ambient;
    float Opaqueness;

    vec3 diffuse;
    float Shininess;

    vec3 specular;
    int diffuseMap;

    vec3 emissive;
    int normalMap;
};

in FS_IN
{
    vec3 Position;
    vec3 Normal;
    vec2 TexCoord;
    flat unsigned int materialID;
} fs_in;


layout (location=0)
out vec4 o_color;  // output fragment data location 0


layout(std430, binding = 0) buffer PointLightBlock {
    readonly Light pointLight[];
} u_pointLightBlock;

layout(std430, binding = 1) buffer DirLightBlock {
    readonly Light   dirLight[];
} u_dirLightBlock;

layout(std430, binding = 2) buffer SpotLightBlock {
    readonly Light  spotLight[];
} u_spotLightBlock;


layout(std430, binding = 4) buffer Texture
{
    readonly uvec2 data[];
} sb_Texture;

layout(std430, binding = 5) buffer Materials
{
    readonly Material material[];
} mat;

const float specularFloat = 32;

vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir, vec3 Color)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 halfwayDir = normalize(lightDir + viewDir);

    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), mat.material[fs_in.materialID].Shininess);

    vec3 ambient = light.ambient * Color;
    vec3 diffuse = light.diffuse * diff * Color;
    vec3 specular = light.specular * spec * specularFloat;
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 Color)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), mat.material[fs_in.materialID].Shininess);
    
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = light.ambient * Color;
    vec3 diffuse = light.diffuse * diff * Color;
    vec3 specular = light.specular * spec * specularFloat;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 Color)
{
    vec3 lightDir     = normalize(light.position - fragPos);
    float diff        = max(dot(normal, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), mat.material[fs_in.materialID].Shininess);
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    float theta       = dot(lightDir, normalize(-light.direction)); 
    float epsilon     = light.cutOff - light.outerCutOff;
    float intensity   = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient = light.ambient * Color;
    vec3 diffuse = light.diffuse * diff * Color;
    vec3 specular = light.specular * spec * specularFloat;
    ambient  *= attenuation * intensity;
    diffuse  *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

void main()
{
    vec3 Color;
    vec3 FragPosition = fs_in.Position;
    vec3 Normal       = fs_in.Normal;

    mat3 TBN = mat3(1.0f);

    if (mat.material[fs_in.materialID].diffuseMap == -1)
    {
        Color = mat.material[fs_in.materialID].diffuse;
        // o_color = vec4(Color, 1);
        // return;
    }
    else
    {
        sampler2D diffuse = sampler2D(sb_Texture.data[mat.material[fs_in.materialID].diffuseMap]);
        Color = texture(diffuse, fs_in.TexCoord).xyz;
    }
    if (mat.material[fs_in.materialID].normalMap != -1)
    {
        sampler2D normal = sampler2D(sb_Texture.data[mat.material[fs_in.materialID].normalMap]);
        Normal = normalize(texture(normal, fs_in.TexCoord) * 2.0 - 1.0).xyz;
        vec3 q1 = dFdx(vec3(fs_in.TexCoord, 0.0));
        vec3 q2 = dFdy(vec3(fs_in.TexCoord, 0.0));
        vec3 T = normalize(q1 * q2.y - q2 * q1.y);
        vec3 B = normalize(cross(Normal, T));
        TBN    = transpose(mat3(T, B, Normal));
        FragPosition = TBN * FragPosition;
    }
    vec3 result = vec3(0, 0, 0);
    for (int i = 0; i < u_dirLightBlock.dirLight.length(); i++)
        result += CalcDirLight(u_dirLightBlock.dirLight[i], Normal, vec3(0, 0, 0), Color);
    for (int i = 0; i < u_pointLightBlock.pointLight.length(); i++)
    {
        Light light = u_pointLightBlock.pointLight[i];
        light.position = TBN * light.position;
        result += CalcPointLight(light, Normal, FragPosition, vec3(0, 0, 0), Color);
    }
    for (int i = 0; i < u_spotLightBlock.spotLight.length(); i++)
    {
        Light light = u_spotLightBlock.spotLight[i];
        light.position = TBN * light.position;
        result += CalcSpotLight(light, Normal, FragPosition, vec3(0, 0, 0), Color);
    }
    o_color = vec4(result, 1);
    // o_color = vec4(Color, 1);
}