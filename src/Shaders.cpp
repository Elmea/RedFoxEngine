#include "OpenGLGraphics.hpp"
#include "glcorearb.h"
#include "imgui_impl_opengl3.h"
#define MEMORY_IMPLEMENTATION
#include "MyMemory.hpp"

static void updateFileTime(const char *path, FILETIME *shaderTime)
{
    HANDLE File = CreateFileA(path, GENERIC_READ,
        FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL, NULL);
    GetFileTime(File, nullptr, nullptr, shaderTime);
    CloseHandle(File);
}

static void CompileShader(RedFoxEngine::Shader &shader, Memory *tempArena)
{
    MyString vertexShaderSource = OpenAndReadEntireFile(shader.vertexPath.data, tempArena);
    MyString fragmentShaderSource = OpenAndReadEntireFile(shader.fragmentPath.data, tempArena);

    shader.vertex = glCreateShaderProgramv(GL_VERTEX_SHADER, 1,
        &vertexShaderSource.data);
    shader.fragment = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1,
        &fragmentShaderSource.data);
    bool succes = true;
    GLint linked;
    glGetProgramiv(shader.vertex, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        char message[1024];
        glGetProgramInfoLog(shader.vertex, sizeof(message), nullptr, message);
        OutputDebugStringA(message);
        Assert(!"Failed to create vertex shader!");
        succes = false;
    }
    updateFileTime(shader.vertexPath.data, &shader.vertexTime);

    glGetProgramiv(shader.fragment, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        char message[1024];
        glGetProgramInfoLog(shader.fragment, sizeof(message), nullptr, message);
        OutputDebugStringA(message);
        Assert(!"Failed to create fragment shader!");
        succes = false;
    }
    updateFileTime(shader.fragmentPath.data, &shader.fragmentTime);
    if (succes)
    {
        glGenProgramPipelines(1, &shader.pipeline);
        glUseProgramStages(shader.pipeline, GL_VERTEX_SHADER_BIT, shader.vertex);
        glUseProgramStages(shader.pipeline, GL_FRAGMENT_SHADER_BIT, shader.fragment);
    }
}

static bool needsUpdate(RedFoxEngine::Shader *shader)
{
    FILETIME vertTime = {};
    FILETIME fragTime = {};
    HANDLE File = CreateFileA(shader->vertexPath.data, GENERIC_READ,
        FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL, NULL);
    GetFileTime(File, nullptr, nullptr, &vertTime);
    CloseHandle(File);
    File = CreateFileA(shader->fragmentPath.data, GENERIC_READ,
        FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL, NULL);
    GetFileTime(File, nullptr, nullptr, &fragTime);
    CloseHandle(File);
    if (CompareFileTime(&shader->vertexTime, &vertTime) || CompareFileTime(&shader->fragmentTime, &fragTime))
        return true;
    return false;
}

void RedFoxEngine::Graphics::UpdateShaders()
{
    Memory *tempArena = &m->m_memory.temp;
    int tempSize = tempArena->usedSize;

    if (needsUpdate(&m_blinnPhong))
        CompileShader(m_blinnPhong, tempArena);
    tempArena->usedSize = tempSize;
    if (needsUpdate(&m_shadow))
        CompileShader(m_shadow, tempArena);
    tempArena->usedSize = tempSize;
    if (needsUpdate(&m_sky))
        CompileShader(m_sky, tempArena);
    tempArena->usedSize = tempSize;
    if (needsUpdate(&m_font))
        CompileShader(m_font, tempArena);
    tempArena->usedSize = tempSize;
    if (needsUpdate(&m_postProcess))
        CompileShader(m_postProcess, tempArena);
    
}

void RedFoxEngine::Graphics::InitShaders()
{
    Memory *tempArena = &m->m_memory.temp;
    int tempSize = tempArena->usedSize;
    // fragment & vertex shaders for drawing triangle
    CompileShader(m_blinnPhong, tempArena);
    tempArena->usedSize = tempSize;
    CompileShader(m_shadow, tempArena);
    tempArena->usedSize = tempSize;
    CompileShader(m_sky, tempArena);
    tempArena->usedSize = tempSize;
    CompileShader(m_font, tempArena);
    tempArena->usedSize = tempSize;
    CompileShader(m_postProcess, tempArena);
}

void RedFoxEngine::Graphics::AddPostProcessShader(Allocators *allocator, const char* fragPath)
{
    Shader temp;
    temp.vertexPath = initStringChar("Shaders/PostProcess.vert", 24, &allocator->arena);
    temp.fragmentPath = initStringChar(fragPath, strlen(fragPath), &allocator->arena);
    CompileShader(temp, &allocator->temp);

    PostProcessShader result;
    result.pipeline = temp.pipeline;
    result.vertex = temp.vertex;
    result.fragment = temp.fragment;
    result.active = true;
    result.name = fragPath + 18;
    m_postProcessShaders.push_back(result);
}

void RedFoxEngine::Graphics::SwapPostProcessShader(int idFirst, int idSecond)
{
    std::swap(m_postProcessShaders[idFirst], m_postProcessShaders[idSecond]);
}

void RedFoxEngine::Graphics::RemovePostProcessShader(Memory* arena, int id)
{
    auto it = m_postProcessShaders.begin();
    std::advance(it, id);
    m_postProcessShaders.erase(it);
}

RedFoxEngine::Kernel* RedFoxEngine::PostProcessShader::AddKernel(RedFoxMaths::Mat4 kernel)
{
    Kernel result;
    result.uniqueId = kernels.size();
    result.kernel = kernel;
    
    kernels.push_back(result);
    return &kernels[result.uniqueId];
}

void RedFoxEngine::PostProcessShader::DeleteKernel(int id)
{
    if (id > kernels.size())
        return;

    auto it = kernels.begin();
    std::advance(it, id);
    kernels.erase(it);
}

void RedFoxEngine::PostProcessShader::EditKernel(int id, RedFoxMaths::Mat4 kernel)
{
    if (id > kernels.size())
        return;

    kernels[id].kernel = kernel;
}

void RedFoxEngine::PostProcessShader::ResetKernel(int id)
{
    if (id > kernels.size())
        return;

    float kernel[4][4] = {
        {0.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 0.0f}
    };
    
    kernels[id].kernel = kernel;
}

void RedFoxEngine::PostProcessShader::BindKernelBuffer(Memory* tempAlocator)
{
    int activeCount = 0;
    for (int i = 0; i < kernels.size(); i++)
    {
        if (kernels[i].active)
            activeCount++;
    }
    
    kernelsMatrices = (RedFoxMaths::Mat4*)MyMalloc(tempAlocator, sizeof(RedFoxMaths::Mat4) * activeCount);
    int count = 0;
    for (int i = 0; i < kernels.size(); i++)
    {
        if (!kernels[i].deleted && kernels[i].active)
        {
            kernelsMatrices[count] = kernels[i].kernel;
            count++;
        }
    }
}
