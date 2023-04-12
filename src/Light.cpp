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

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Light::SetProjection(LightType type)
{
    switch (type)
    {
    case (LightType::DIRECTIONAL):
        projection = RedFoxMaths::Mat4::GetOrthographicMatrix(-100, 100, -100, 100, 0.1, 100);
        break;

    case (LightType::POINT):
        projection = RedFoxMaths::Mat4::GetPerspectiveMatrix(80, 1, 25, 0.1);
        break;

    case (LightType::SPOT):
        projection = RedFoxMaths::Mat4::GetPerspectiveMatrix(80, 1, 25, 0.1);
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
    
    glCreateBuffers(1, &m_pointLightBuffer);
    glNamedBufferStorage(m_pointLightBuffer, 100 * sizeof(Light), nullptr,
        GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
    glCreateBuffers(1, &m_dirLightBuffer);
    glNamedBufferStorage(m_dirLightBuffer, 100 * sizeof(Light), nullptr,
        GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT | GL_MAP_READ_BIT);

    glCreateBuffers(1, &m_spotLightBuffer);
    glNamedBufferStorage(m_spotLightBuffer, 100 * sizeof(Light), nullptr,
        GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
}

void Engine::UpdateLights(float time, LightStorage* lightStorage) //TODO: This function or something like this could be in game or in physics
{
    int dirCount = 0, pointCount = 0, spotCount = 0;

    LightInfo* dirligths = (LightInfo*)MyMalloc(&m_tempAllocator, sizeof(LightInfo) * 1000);
    LightInfo* pointLights = (LightInfo*)MyMalloc(&m_tempAllocator, sizeof(LightInfo) * 1000); ;
    LightInfo* spotlights = (LightInfo*)MyMalloc(&m_tempAllocator, sizeof(LightInfo) * 1000); ;

    for (int i = 0; i < lightStorage->lightCount; i++)
    {
        Light* current = &lightStorage->lights[i];
        switch (current->GetType())
        {
        case (LightType::DIRECTIONAL):
            dirligths[dirCount] = current->lightInfo;
            dirCount++;
            break;

        case (LightType::POINT):
            pointLights[pointCount] = current->lightInfo;
            pointCount++;
            break;

        case (LightType::SPOT):
            spotlights[spotCount] = current->lightInfo;
            spotCount++;
            break;

        default:
            break;
        }


        RedFoxMaths::Float3 rotation = RedFoxMaths::Float3::DirToEuler(current->lightInfo.direction, { 0.0f, 0.0f, 1.0f });
        RedFoxMaths::Mat4 lightView = RedFoxMaths::Mat4::GetTranslation(current->lightInfo.position) *
            RedFoxMaths::Mat4::GetRotationY(rotation.y) * RedFoxMaths::Mat4::GetRotationX(rotation.x) *
            RedFoxMaths::Mat4::GetRotationZ(rotation.z);

        current->lightInfo.VP = (lightStorage->lights[i].GetProjection() * lightView.GetInverseMatrix()).GetTransposedMatrix();
    }

    m_graphics.setLightsCount(dirCount, pointCount, spotCount);
    m_graphics.FillLightBuffer(dirligths, LightType::DIRECTIONAL);
    m_graphics.FillLightBuffer(pointLights, LightType::POINT);
    m_graphics.FillLightBuffer(spotlights, LightType::SPOT);
}

Light *Graphics::GetDirLightBuffer(int *lightCount)
{
    if (lightCount)
        *lightCount = m_dirLightCount;
    else
        return (nullptr);
    GLbitfield mapFlags = (GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    return((Light*)glMapNamedBufferRange(m_dirLightBuffer, 0,
        m_dirLightCount * sizeof(Light), mapFlags));

}

void Graphics::BindLights()
{
    if (m_pointLightCount)
    {
        GLuint bindingPoint = 0;
        glBindBufferRange(GL_SHADER_STORAGE_BUFFER, bindingPoint, m_pointLightBuffer, 0, m_pointLightCount);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, m_pointLightBuffer);
    }
    if (m_dirLightCount)
    {
        GLuint bindingDir = 1;
        glBindBufferRange(GL_SHADER_STORAGE_BUFFER, bindingDir, m_dirLightBuffer, 0, m_dirLightCount);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingDir, m_dirLightBuffer);
    }
    if (m_spotLightCount)
    {
        GLuint bindingSpot = 2;
        glBindBufferRange(GL_SHADER_STORAGE_BUFFER, bindingSpot, m_spotLightBuffer, 0, m_spotLightCount);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingSpot, m_spotLightBuffer);
    }
}

void Graphics::ReleaseDirLightBuffer()
{
    glUnmapNamedBuffer(m_dirLightBuffer);
    glFlush();
}

Light *Graphics::GetPointLightBuffer(int *lightCount)
{
    if (lightCount)
        *lightCount = m_pointLightCount;
    else
        return (nullptr);
    GLbitfield mapFlags = (GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    return((Light*)glMapNamedBufferRange(m_pointLightBuffer, 0,
        m_pointLightCount * sizeof(Light), mapFlags));

}

void Graphics::ReleasePointLightBuffer()
{
    glUnmapNamedBuffer(m_pointLightBuffer);
    glFlush();
}

void Graphics::FillLightBuffer(LightInfo* lights, LightType type)
{
    GLuint lightBuffer = 0;
    int lightCount = 0;
    switch (type)
    {
    case RedFoxEngine::NONE:
        return;
    case RedFoxEngine::DIRECTIONAL:
        lightBuffer = m_dirLightBuffer;
        lightCount = m_dirLightCount;
        break;
    case RedFoxEngine::POINT:
        lightBuffer = m_pointLightBuffer;
        lightCount = m_pointLightCount;
        break;
    case RedFoxEngine::SPOT:
        lightBuffer = m_spotLightBuffer;
        lightCount = m_spotLightCount;
        break;
    default:
        break;
    }
    if (!lightCount)
        return;

    glNamedBufferSubData(lightBuffer, 0, lightCount * sizeof(LightInfo), lights);
}
}
