#include "Light.hpp"
#include "OpenGLGraphics.hpp"
#include "Engine.hpp"

#define MEMORY_IMPLEMENTATION
#include "MyMemory.hpp"


namespace RedFoxEngine
{
Light::Light(LightType lightType, int _index)
{
    lightInfo.index = _index;
    type = lightType;
    glCreateFramebuffers(1, &lightInfo.shadowParameters.depthMapFBO);

    glGenTextures(1, &lightInfo.shadowParameters.depthMap);
    glBindTexture(GL_TEXTURE_2D, lightInfo.shadowParameters.depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        lightInfo.shadowParameters.SHADOW_WIDTH, lightInfo.shadowParameters.SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, lightInfo.shadowParameters.depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, lightInfo.shadowParameters.depthMap, 0);

    u64 handle = glGetTextureHandleARB(lightInfo.shadowParameters.depthMap);
    glMakeTextureHandleResidentARB(handle);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Light* LightStorage::CreateLight(LightType type)
{
    for (int i = 0 ; i < lightCount; i++)
    {
        if (lights[i].GetType() == LightType::NONE)
        {
            lights[i].SetType(type);
            return &lights[i];
        }
    }

    Light newLight { type , lightCount };
    newLight.lightInfo.index = lightCount;
    newLight.SetType(type);

    lights[lightCount] = newLight;
    shadowMaps[lightCount] = lights[lightCount].lightInfo.shadowParameters.depthMap;
    switch(type)
    {
        case POINT: pointLightCount++;break;
        case SPOT: spotLightCount++;break;
        case DIRECTIONAL: dirLightCount++;break;
        case NONE: break;
    }
    lightCount++;

    return &lights[lightCount-1];
}

void LightStorage::RemoveLight(int lightIndex)
{
    switch(lights[lightIndex].GetType())
    {
        case POINT: pointLightCount--;break;
        case SPOT: spotLightCount--;break;
        case DIRECTIONAL: dirLightCount--;break;
        case NONE: break;
    }
    lights[lightIndex].SetType(LightType::NONE);
}

void Light::SetProjection(LightType type)
{
    switch (type)
    {
    case (LightType::DIRECTIONAL):
        projection = RedFoxMaths::Mat4::GetOrthographicMatrix(-100, 100, -100, 100, 100.0001, 0.0001);
        break;

    case (LightType::POINT):
        projection = RedFoxMaths::Mat4::GetPerspectiveMatrix(1, 80, 1000, 0.0001);
        break;

    case (LightType::SPOT):
        projection = RedFoxMaths::Mat4::GetPerspectiveMatrix(1, 120, 1000, 0.0001);
        break;

    default:
        break;
    }
}

void Light::SetType(LightType _type)
{
    type = _type;
    SetProjection(type);
}

RedFoxMaths::Mat4 Light::GetProjection()
{
    return projection;
}

LightType Light::GetType()
{
    return type;
}

void Light::operator=(Light& light)
{
    memcpy(this, &light, sizeof(Light));
}

void LightInfo::operator=(LightInfo& light)
{
    memcpy(this, &light, sizeof(LightInfo));
}

void Graphics::InitLights()
{
    //NOTE: The allocation of pointLightCount to all of the lights is not a mistake.
    //     We should think about a max value, but that is the intention, all lights are
    // allocated to their max value first, then we can increase in size or decrease at runtime
    // the amount we actually use without reallocating.

    const int maxLightCount = 100;
    glCreateBuffers(1, &lightStorage.pointLightSSBO);
    glNamedBufferStorage(lightStorage.pointLightSSBO, maxLightCount * sizeof(Light), nullptr,
        GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
    glCreateBuffers(1, &lightStorage.dirLightSSBO);
    glNamedBufferStorage(lightStorage.dirLightSSBO, maxLightCount * sizeof(Light), nullptr,
        GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);

    glCreateBuffers(1, &lightStorage.spotLightSSBO);
    glNamedBufferStorage(lightStorage.spotLightSSBO, maxLightCount * sizeof(Light), nullptr,
        GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
}

void Engine::UpdateLights(LightStorage* lightStorage) //TODO: This function or something like this could be in game or in physics
{
    int dirCount = 0, pointCount = 0, spotCount = 0;
    LightInfo* dirLights = (LightInfo*)MyMalloc(&m_tempAllocator, sizeof(LightInfo) * lightStorage->dirLightCount);
    LightInfo* pointLights = (LightInfo*)MyMalloc(&m_tempAllocator, sizeof(LightInfo) * lightStorage->pointLightCount);
    LightInfo* spotLights = (LightInfo*)MyMalloc(&m_tempAllocator, sizeof(LightInfo) * lightStorage->spotLightCount);

    for (int i = 0; i < lightStorage->lightCount; i++)
    {
        Light* current = &lightStorage->lights[i];
        bool update = true;
        switch (current->GetType())
        {
        case (LightType::DIRECTIONAL):
            dirLights[dirCount] = current->lightInfo;
            dirCount++;
            break;

        case (LightType::POINT):
            pointLights[pointCount] = current->lightInfo;
            pointCount++;
            break;

        case (LightType::SPOT):
            spotLights[spotCount] = current->lightInfo;
            spotCount++;
            break;

        default:
            update = false;
            break;
        }

        if (update)
        {

            RedFoxMaths::Float3 rotation = RedFoxMaths::Float3::DirToEuler(current->lightInfo.direction, { 0.0f, 1.0f, 0.0f });

            RedFoxMaths::Mat4 lightView = RedFoxMaths::Mat4::GetTranslation(current->lightInfo.position) * RedFoxMaths::Mat4::GetRotationY(rotation.y) *
                RedFoxMaths::Mat4::GetRotationX(rotation.x) * RedFoxMaths::Mat4::GetRotationZ(rotation.z);

            current->lightInfo.VP = (lightStorage->lights[i].GetProjection() * lightView.GetInverseMatrix()).GetTransposedMatrix();
        }
    }
    glNamedBufferSubData(lightStorage->pointLightSSBO, 0, lightStorage->pointLightCount * sizeof(LightInfo), pointLights);
    glNamedBufferSubData(lightStorage->dirLightSSBO, 0, lightStorage->dirLightCount * sizeof(LightInfo), dirLights);
    glNamedBufferSubData(lightStorage->spotLightSSBO, 0, lightStorage->spotLightCount * sizeof(LightInfo), spotLights);
}

void Graphics::BindLights()
{
    GLuint bindingPoint = 0;
    if (lightStorage.pointLightCount)
    {
        glBindBufferRange(GL_SHADER_STORAGE_BUFFER, bindingPoint, lightStorage.pointLightSSBO, 0, lightStorage.pointLightCount);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, lightStorage.pointLightSSBO);
    }
    else
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, 0);
    GLuint bindingDir = 1;
    if (lightStorage.dirLightCount)
    {
        glBindBufferRange(GL_SHADER_STORAGE_BUFFER, bindingDir, lightStorage.dirLightSSBO, 0, lightStorage.dirLightCount);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingDir, lightStorage.dirLightSSBO);
    }
    else
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingDir, 0);
    GLuint bindingSpot = 2;
    if (lightStorage.spotLightCount)
    {
        glBindBufferRange(GL_SHADER_STORAGE_BUFFER, bindingSpot, lightStorage.spotLightSSBO, 0, lightStorage.spotLightCount);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingSpot, lightStorage.spotLightSSBO);
    }
    else  
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingSpot, 0);
}

}
