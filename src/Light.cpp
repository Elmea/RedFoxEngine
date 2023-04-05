#include "OpenGLGraphics.hpp"
#include "Engine.hpp"

namespace RedFoxEngine
{

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
    glNamedBufferStorage(m_pointLightBuffer, m_pointLightCount * sizeof(PointLight), nullptr,
        GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
    glCreateBuffers(1, &m_dirLightBuffer);
    glNamedBufferStorage(m_dirLightBuffer, m_pointLightCount * sizeof(DirLight), nullptr,
        GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);

    glCreateBuffers(1, &m_spotLightBuffer);
    glNamedBufferStorage(m_spotLightBuffer, m_pointLightCount * sizeof(SpotLight), nullptr,
        GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
}

void Engine::UpdateLights(float time) //TODO: This function or something like this could be in game or in physics
{
    int lightCount = 0;
    {
        PointLight *light = (PointLight *)m_graphics.GetPointLightBuffer(&lightCount);

        for(int i = 0; i < lightCount; i++)
        {
            light[i] = {};
            light[i].position = {{sinf(time) * i * 2,
                                  sinf((time / 3) * i * 3),
                                  cosf((time / 6) * 2) * i * 3}};
            light[i].constant  = 1.0f;
            light[i].linear    = 0.09f;
            light[i].quadratic = 0.032f;
            light[i].ambient = {{0.5, 0.5, 0.5}};
            light[i].specular = {{0.1, 0.1, 0.1}};
            float intensity = 0.3f;
            light[i].diffuse = {{sinf(time * 0.5 * i) * intensity,
                                 sinf(time * 0.6 * i) * intensity,
                                 cosf(time * 0.3 * i) * intensity}};
        }
    }
    m_graphics.ReleasePointLightBuffer();
    {
        DirLight *light = (DirLight *)m_graphics.GetDirLightBuffer(&lightCount);

        for(int i = 0; i < lightCount; i++)
        {
            light[i] = {};
            light[i].direction = {{0, -1, 0}};
            light[i].diffuse = {{0.02, 0.02, 0.02}};
            light[i].specular = {{0.1, 0.1, 0.1}};
            light[i].ambient = {{0.1, 0.1, 0.1}};
        }
        m_graphics.ReleaseDirLightBuffer();
    }
    
}

DirLight *Graphics::GetDirLightBuffer(int *lightCount)
{
    if (lightCount)
        *lightCount = m_dirLightCount;
    else
        return (nullptr);
    GLbitfield mapFlags = (GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    return((DirLight *)glMapNamedBufferRange(m_dirLightBuffer, 0,
        m_dirLightCount * sizeof(DirLight), mapFlags));

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

void Graphics::SetPointLightBuffer(PointLight* pointLight, int lightCount)
{
    if (lightCount)
        glNamedBufferSubData(m_pointLightBuffer, 0, lightCount * sizeof(PointLight), pointLight);
}

PointLight *Graphics::GetPointLightBuffer(int *lightCount)
{
    if (lightCount)
        *lightCount = m_pointLightCount;
    else
        return (nullptr);
    GLbitfield mapFlags = (GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    return((PointLight *)glMapNamedBufferRange(m_pointLightBuffer, 0,
        m_pointLightCount * sizeof(PointLight), mapFlags));
}

void Graphics::ReleasePointLightBuffer()
{
    glUnmapNamedBuffer(m_pointLightBuffer);
    glFlush();
}
}
