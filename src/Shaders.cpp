#include "OpenGLGraphics.hpp"
#include "glcorearb.h"
#include "imgui_impl_opengl3.h"
#define MEMORY_IMPLEMENTATION
#include "MyMemory.hpp"

static void CompileShader(const char *vertexShaderSource,
                    const char *fragmentShaderSource,
    RedFoxEngine::Shader &shader)
{
    shader.vertex = glCreateShaderProgramv(GL_VERTEX_SHADER, 1,
        &vertexShaderSource);
    shader.fragment = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1,
        &fragmentShaderSource);

    GLint linked;
    glGetProgramiv(shader.vertex, GL_LINK_STATUS, &linked);
    if (!linked)
    { 
        char message[1024];
        glGetProgramInfoLog(shader.vertex, sizeof(message), nullptr, message);
        OutputDebugStringA(message);
        Assert(!"Failed to create vertex shader!");
    }

    glGetProgramiv(shader.fragment, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        char message[1024];
        glGetProgramInfoLog(shader.fragment, sizeof(message), nullptr, message);
        OutputDebugStringA(message);
        Assert(!"Failed to create fragment shader!");
    }
    glGenProgramPipelines(1, &shader.pipeline);
    glUseProgramStages(shader.pipeline, GL_VERTEX_SHADER_BIT, shader.vertex);
    glUseProgramStages(shader.pipeline, GL_FRAGMENT_SHADER_BIT, shader.fragment);
}

void RedFoxEngine::Graphics::InitShaders(Memory *tempArena)
{
    int tempSize = tempArena->usedSize;
    // fragment & vertex shaders for drawing triangle
    MyString vertexShaderSource = OpenAndReadEntireFile(
        "blinn_phong.vert.glsl", tempArena);
    MyString fragmentShaderSource = OpenAndReadEntireFile(
        "blinn_phong.frag.glsl", tempArena);
    CompileShader(vertexShaderSource.data, fragmentShaderSource.data,
        m_blinnPhong);
    tempArena->usedSize = tempSize;
    vertexShaderSource = OpenAndReadEntireFile(
        "ShadowShader.vert", tempArena);
    fragmentShaderSource = OpenAndReadEntireFile(
        "ShadowShader.frag", tempArena);
    CompileShader(vertexShaderSource.data, fragmentShaderSource.data,
        m_shadow);
    tempArena->usedSize = tempSize;
    vertexShaderSource = OpenAndReadEntireFile(
        "skydome.vert", tempArena);
    fragmentShaderSource = OpenAndReadEntireFile(
        "skydome.frag", tempArena);
    CompileShader(vertexShaderSource.data, fragmentShaderSource.data,
        m_sky);
    tempArena->usedSize = tempSize;

    vertexShaderSource = OpenAndReadEntireFile(
        "font.vert", tempArena);
    fragmentShaderSource = OpenAndReadEntireFile(
        "font.frag", tempArena);
    CompileShader(vertexShaderSource.data, fragmentShaderSource.data,m_font);

    tempArena->usedSize = tempSize;

    vertexShaderSource = OpenAndReadEntireFile(
        "PostProcess.vert", tempArena);
    fragmentShaderSource = OpenAndReadEntireFile(
        "PostProcess.frag", tempArena);
    CompileShader(vertexShaderSource.data, fragmentShaderSource.data,m_postProcess);
}

void RedFoxEngine::Graphics::AddPostProcessShader(Memory *tempArena, const char* fragPath)
{
    MyString vertexShaderSource = OpenAndReadEntireFile("PostProcess.vert", tempArena);
    MyString fragmentShaderSource = OpenAndReadEntireFile(fragPath, tempArena);

    Shader result;

    CompileShader(vertexShaderSource.data, fragmentShaderSource.data, result);

    m_postProcessShaders.push_back(result);
}

void RedFoxEngine::Graphics::SwapPostProcessShader(int idFirst, int idSecond)
{
    std::swap(m_postProcessShaders[idFirst],m_postProcessShaders[idSecond]);
}

void RedFoxEngine::Graphics::RemovePostProcessShader(int id)
{
    auto it = m_postProcessShaders.begin();
    std::advance(it, id);
    m_postProcessShaders.erase(it);
}