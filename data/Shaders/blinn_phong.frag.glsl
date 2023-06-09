#version 450 core
#extension GL_ARB_bindless_texture : enable

struct ShadowParameters
{
    int index;
    int SHADOW_WIDTH, SHADOW_HEIGHT;
    uint depthMap;
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
};

struct Material
{
    vec3 diffuse;
    float Shininess;

    float Opaqueness;
    int diffuseMap;
    int normalMap;
    int _padding;
};

layout (location = 0) uniform vec3 viewPos;

layout(location = 0)in FS_IN
{
    vec3 Position;
    vec3 Normal;
    vec2 TexCoord;
    flat uint materialID;
    vec3 Color;
} fs_in;

layout (location=0)
out vec4 o_color;  // output fragment data location 0

layout(std430, binding = 0) readonly restrict buffer PointLightBlock {
    Light pointLight[];
} u_pointLightBlock;

layout(std430, binding = 1) readonly restrict buffer DirLightBlock {
    Light   dirLight[];
} u_dirLightBlock;

layout(std430, binding = 2) readonly restrict buffer SpotLightBlock {
    Light  spotLight[];
} u_spotLightBlock;

layout(std430, binding = 4) readonly restrict buffer Texture
{
    uvec2 data[];
} sb_Texture;

layout(std430, binding = 5) readonly restrict buffer Materials
{
    Material material[];
} mat;

layout(std430, binding = 6) readonly restrict buffer ShadowMaps
{
    uvec2 data[];
} sb_ShadowMaps;

const float specularFloat = 32;

layout (location = 1) uniform int materialOffset;

float ShadowCalculation(Light light)
{
    vec3 FragPosition = fs_in.Position;
    vec3 Normal       = fs_in.Normal;

    vec4 fragPosLightSpace = light.lightVp * vec4(FragPosition, 1);

    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(sampler2D(sb_ShadowMaps.data[light.shadowParameters.index]), projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = -max(0.00000025 * (1.0 - dot(Normal, vec3(fragPosLightSpace))), 0.0000000025);  

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(sampler2D(sb_ShadowMaps.data[0]), 0);
    for(float x = -1.5; x <= 1.5; ++x)
    {
        for(float y = -1.5; y <= 1.5; ++y)
        {
            float pcfDepth = texture(sampler2D(sb_ShadowMaps.data[0]), projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth + bias > pcfDepth ? 1.0 : 0.0;        
        } 
    }
    shadow /= 16.0;
    return shadow;
}  

vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir, vec3 Color)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 halfwayDir = normalize(lightDir + viewDir);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.material[fs_in.materialID].Shininess);
    // float spec = pow(max(dot(viewDir, halfwayDir), 0.0), mat.material[fs_in.materialID].Shininess);

    vec3 ambient = light.ambient * Color;
    vec3 diffuse = light.diffuse * diff * Color;
    vec3 specular = light.specular * spec * specularFloat;
    float shadow = ShadowCalculation(light);
    return (ambient + (1.0 - shadow) * (diffuse + specular));
}

vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 Color)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), mat.material[fs_in.materialID + materialOffset].Shininess);
    
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = light.ambient * Color;
    vec3 diffuse = light.diffuse * diff * Color;
    vec3 specular = light.specular * spec * specularFloat;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    // float shadow = ShadowCalculation(light);
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 Color)
{
    vec3 lightDir     = normalize(light.position - fragPos);
    float diff        = max(dot(normal, lightDir), 0.0);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), mat.material[fs_in.materialID + materialOffset].Shininess);
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
    float shadow = ShadowCalculation(light);
    return (ambient + (1.0 - shadow) * (diffuse + specular));
}

void main()
{
    vec3 Color = fs_in.Color;
    vec3 FragPosition = fs_in.Position;
    vec3 Normal       = fs_in.Normal;
    vec3 viewDir = normalize(viewPos - FragPosition);
    mat3 TBN = mat3(1.0f);
    if (mat.material[fs_in.materialID + materialOffset].diffuseMap != -1)
        Color = texture(sampler2D(sb_Texture.data[mat.material[fs_in.materialID + materialOffset].diffuseMap]), fs_in.TexCoord).xyz;

    if (mat.material[fs_in.materialID + materialOffset].normalMap != -1)
    {
        Normal = normalize(texture(sampler2D(sb_Texture.data[mat.material[fs_in.materialID + materialOffset].normalMap]), fs_in.TexCoord) * 2.0 - 1.0).xyz;
        vec3 q1 = dFdx(vec3(fs_in.TexCoord, 0.0));
        vec3 q2 = dFdy(vec3(fs_in.TexCoord, 0.0));
        vec3 T = normalize(q1 * q2.y - q2 * q1.y);
        vec3 B = normalize(cross(Normal, T));
        TBN    = transpose(mat3(T, B, Normal));
        FragPosition = TBN * FragPosition;
    }
    vec3 result = vec3(0, 0, 0);
    for (int i = 0; i < u_dirLightBlock.dirLight.length(); i++)
        result += CalcDirLight(u_dirLightBlock.dirLight[i], Normal, viewDir, Color);
    for (int i = 0; i < u_pointLightBlock.pointLight.length(); i++)
    {
        Light light = u_pointLightBlock.pointLight[i];
        light.position = TBN * light.position;
        result += CalcPointLight(light, Normal, FragPosition, viewDir, Color);
    }
    for (int i = 0; i < u_spotLightBlock.spotLight.length(); i++)
    {
        Light light = u_spotLightBlock.spotLight[i];
        light.position = TBN * light.position;
        result += CalcSpotLight(light, Normal, FragPosition, viewDir, Color);
    }
    o_color = vec4(result, 1);
}