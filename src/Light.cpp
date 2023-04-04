#include "OpenGLGraphics.hpp"
#include "Engine.hpp"

#define MEMORY_IMPLEMENTATION
#include "MyMemory.hpp"


namespace RedFoxEngine
{
Light::Light(LightType lightType)
{
    /*
    switch (lightType)
    {
    case DIRECTIONAL:
        shadowParameters.projection = RedFoxMaths::Mat4::GetOrthographicMatrix(-50, 50, -50, 50, 0.1, 100);
        break;
    case POINT:
        shadowParameters.projection = RedFoxMaths::Mat4::GetPerspectiveMatrix(80, 1, 0.1, 25);
        break;
    case SPOT:
        shadowParameters.projection = RedFoxMaths::Mat4::GetPerspectiveMatrix(80, 1, 0.1, 25);
        break;
    default:
        break;
    }*/

    type = lightType;
    glCreateFramebuffers(1, &shadowParameters.depthMapFBO);

    glGenTextures(1, &shadowParameters.depthMap);
    glBindTexture(GL_TEXTURE_2D, shadowParameters.depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        shadowParameters.SHADOW_WIDTH, shadowParameters.SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, shadowParameters.depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowParameters.depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Light::operator=(Light& light)
{
    memcpy(this, &light, sizeof(Light));
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

    /*
    int lightCount = 0;
    {
        Light*light = (Light*)m_graphics.GetPointLightBuffer(&lightCount);

        for(int i = 0; i < lightCount; i++)
        {
            light[i] = {};
            light[i].position = {{sinf(time) * i * 2,
                                  sinf((time / 3) * i * 3),
                                  cosf((time / 6) * 2) * i * 3}};
            light[i].constant  = 1.0f;
            light[i].linear    = 0.09f;
            light[i].quadratic = 0.032f;
            light[i].ambient = {{0.01, 0.01, 0.01}};
            light[i].specular = {{0.1, 0.1, 0.1}};
            float intensity = 0.3f;
            light[i].diffuse = {{sinf(time * 0.5 * i) * intensity,
                                 sinf(time * 0.6 * i) * intensity,
                                 cosf(time * 0.3 * i) * intensity}};
        }
    }
    m_graphics.ReleasePointLightBuffer();
    {
        Light*light = (Light*)m_graphics.GetDirLightBuffer(&lightCount);

        for(int i = 0; i < lightCount; i++)
        {
            light[i] = {};
            light[i].direction = {{0, -1, 0}};
            light[i].ambient = {{0.01, 0.01, 0.01}};
            light[i].specular = {{0.1, 0.1, 0.1}};
            light[i].ambient = {{0.01, 0.01, 0.01}};
        }
        m_graphics.ReleaseDirLightBuffer();
    }*/

    int dirCount = 0, pointCount = 0, spotCount = 0;

    Light* dirligths = (Light*)MyMalloc(&m_tempAllocator, sizeof(Light) * 1000);
    Light* pointLights = (Light*)MyMalloc(&m_tempAllocator, sizeof(Light) * 1000); ;
    Light* spotlights = (Light*)MyMalloc(&m_tempAllocator, sizeof(Light) * 1000); ;

    for (int i = 0; i < lightStorage->lightCount; i++)
    {
        switch (lightStorage->lights[i].type)
        {
        case (LightType::DIRECTIONAL):
            dirligths[dirCount] = lightStorage->lights[i];
            dirCount++;
            break;

        case (LightType::POINT):
            pointLights[pointCount] = lightStorage->lights[i];
            pointCount++;
            break;

        case (LightType::SPOT):
            spotlights[spotCount] = lightStorage->lights[i];
            spotCount++;
            break;

        default:
            break;
        }
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

void Graphics::FillLightBuffer(Light* lights, LightType type)
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
    glNamedBufferSubData(lightBuffer, 0, lightCount * sizeof(Light) - sizeof(LightType), lights);
    //Light *test = (Light *)glMapNamedBufferRange(lightBuffer, 0, lightCount * sizeof(Light) - sizeof(LightType), GL_MAP_READ_BIT);
}
}
