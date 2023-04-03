#include "OpenGLGraphics.hpp"
#include "Engine.hpp"


namespace RedFoxEngine
{
Light::Light(LightType lightType)
{
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
    }

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

void Graphics::InitLights()
{
    //NOTE: The allocation of pointLightCount to all of the lights is not a mistake.
    //     We should think about a max value, but that is the intention, all lights are
    // allocated to their max value first, then we can increase in size or decrease at runtime
    // the amount we actually use without reallocating.
    
    m_pointLightCount = 100; //TODO: in the future all lights will start at 0 count, and 
    // the game or engine editor will add them in the scene.
    m_dirLightCount = 1;
    m_spotLightCount = 0;
    glCreateBuffers(1, &m_pointLightBuffer);
    glNamedBufferStorage(m_pointLightBuffer, m_pointLightCount * sizeof(Light), nullptr,
        GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
    glCreateBuffers(1, &m_dirLightBuffer);
    glNamedBufferStorage(m_dirLightBuffer, m_pointLightCount * sizeof(Light), nullptr,
        GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);

    glCreateBuffers(1, &m_spotLightBuffer);
    glNamedBufferStorage(m_spotLightBuffer, m_pointLightCount * sizeof(Light), nullptr,
        GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
}

void Engine::UpdateLights(float time) //TODO: This function or something like this could be in game or in physics
{

    /*
    int lightCount = 0;
    {
        Light*light = (Light*)m_graphics.GetPointLightBuffer(&lightCount);

        for(int i = 0; i < lightCount; i++)
        {
            //light[i] = {};
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
            //light[i] = {};
            light[i].direction = {{0, -1, 0}};
            light[i].ambient = {{0.01, 0.01, 0.01}};
            light[i].specular = {{0.1, 0.1, 0.1}};
            light[i].ambient = {{0.01, 0.01, 0.01}};
        }
        m_graphics.ReleaseDirLightBuffer();
    }*/
    
    int dirCount, pointCount, spotCount = 0;

    // To do : Think a way to store light, in engine or graphics ? 
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

void Graphics::SetDirLightBuffer(Light* pointLight, int lightCount)
{
    if (lightCount)
    {
        glNamedBufferSubData(m_dirLightBuffer, 0, lightCount * sizeof(Light), pointLight);
    }
}

void Graphics::SetPointLightBuffer(Light* pointLight, int lightCount)
{
    if (lightCount)
    {
        glNamedBufferSubData(m_pointLightBuffer, 0, lightCount * sizeof(Light), pointLight);
    }
}

void Graphics::SetSpotLightBuffer(Light* pointLight, int lightCount)
{
    if (lightCount)
    {
        glNamedBufferSubData(m_spotLightBuffer, 0, lightCount * sizeof(Light), pointLight);
    }
}

}
