#pragma once

#ifdef _WIN32
#include "Win32Platform.hpp"
#endif

#include <string>
#include <utility>
#include <vector>
#include <GL/gl.h>
#include "OpenGLFunctions.hpp"
#include "ObjParser.hpp"
#include "imgui.h"
#include "Light.hpp"
#include "Scene.hpp"

#include "imstb_truetype.h"

#define MAX_KERNEL 5

namespace RedFoxEngine
{

struct LightStorage
{
    u32    spotLightCount;
    GLuint spotLightSSBO;
    u32    dirLightCount;
    GLuint dirLightSSBO;
    u32    pointLightCount;
    GLuint pointLightSSBO;
    int lightCount;
    Light* lights;
    unsigned int* shadowMaps;
    Light* CreateLight(LightType type);
    void ModifyLightType(int index, LightType type);
    void RemoveLight(int lightIndex);
};

struct Textures
{
    GLuint textures[128];
    u32 textureCount; 
};

struct Shader
{
    GLuint vertex, fragment, pipeline;
    MyString vertexPath, fragmentPath;
    FILETIME vertexTime;
    FILETIME fragmentTime;
};
    
class Graphics;

class Kernel
{
private:
    bool deleted = false;
    int uniqueId;
public:
    bool active = true;
    RedFoxMaths::Mat4 kernel;
    friend class Graphics;
    friend class PostProcessShader;
};

class PostProcessShader
{
private:
    RedFoxMaths::Mat4* kernelsMatrices;
    
    GLuint vertex, fragment, pipeline;
public:
    bool active = true;
    bool useKernels;
    std::string name;

    std::vector<Kernel> kernels;
    
    Kernel* AddKernel(RedFoxMaths::Mat4 kernel);
    void DeleteKernel(int id);
    void EditKernel(int id, RedFoxMaths::Mat4 kernel);
    void BindKernelBuffer(Memory* tempAlocator);
    void ResetKernel(int id);
    friend class Graphics;
};

class Graphics
{
private:
    ResourcesManager *m = nullptr;
    GLuint m_textureSampler;
    Textures m_textures = {};
    RedFoxMaths::Mat4 m_viewProjection;

    Shader m_blinnPhong;
    Shader m_shadow;
    Shader m_sky;
    Shader m_font;
    Shader m_postProcess;

    GLuint m_quadVAO;

    GLuint m_imguiFramebuffer;
    GLuint m_sceneFramebuffer;

    u32    m_indexCount = 0;
    u32    m_vertexCount = 0;
    u32    m_vertexBufferObject;
    u32    m_vertexArrayObject;
    u32    m_ebo;

    u32    m_materialSSBO;
    u32    m_matrixSSBO;
    u32    m_textureSSBO;
    u32    m_shadowMapsSSBO;

    stbtt_bakedchar cdata[96];
    GLuint m_gFontTexture;

    unsigned int m_quadVBO;

    int m_kernelCreated = 0;
    RedFoxMaths::Mat4* m_kernelsMatrices;
    WindowDimension m_sceneTextureDimension;

    GLuint m_evenPostProcessTexture;
    GLuint m_oddPostProcessTexture;
    GLuint m_evenPostProcessFramebuffer;
    GLuint m_oddPostProcessFramebuffer;
    
    void PostProcessDrawQuad();

public:
    bool useKernelInFinalPass = true;
    int m_kernelCount;
    const int m_maxPostProcessShader = 5;
    Kernel* m_kernels;
    std::vector<PostProcessShader> m_postProcessShaders;

    WindowDimension dimension;
    GLuint          m_imguiTexture;
    GLuint          m_sceneTexture;
    Model          *m_models = nullptr;
    Material       *m_materials;
    u32             m_materialCount;
    u32             m_modelCount;
    LightStorage    lightStorage;
    bool            postProcessingEnabled;
    
    void InitGraphics(ResourcesManager *manager, WindowDimension p_dimension);
    void InitModel(Model* model, ObjModel obj);
    void InitLights();
    void InitFont();
    void InitQuad();

    void InitModelTextures(ObjImages* images);
    u32  InitTexture(void *data, int width, int height, bool resident, bool repeat);
    void InitFramebuffer();
    void InitShaders();
    void InitImGUIFramebuffer(WindowDimension dimension);
    void InitSceneFramebuffer(WindowDimension dimension);
    void InitPostProcess(Memory* arena);
    
    void BindLights();
    
    void SetViewProjectionMatrix(RedFoxMaths::Mat4 vp);
    void FillLightBuffer(LightInfo* lights, LightType type);
    void UpdateShaders();
    void UpdateImGUIFrameBuffer(WindowDimension& dimension, WindowDimension content);
    void UpdateModelMatrices(GameObject* objects, int gameObjectCount, Memory* temp);
    void PushMaterial(int count);
    void PushModelMatrices(RedFoxMaths::Mat4 *matrices, int count);

    
    void Draw(Scene *m_scene, WindowDimension p_windowDimension, float p_time, float p_delta, RedFoxMaths::Float3 position);
    void DrawShadowMaps(u64* modelCountIndex);
    void DrawSkyDome(SkyDome skyDome, float dt);
    void DrawGameObjects(u64* modelCountIndex, RedFoxMaths::Float3 view);
    void DrawModelInstances(Model* model,
        RedFoxMaths::Mat4* modelMatrices, int instanceCount);
    void DrawModelShadowInstances(Model* model, int instanceCount);
    void RenderText(GameUI ui);
    
    void ResetKernel(int id);

    void PostProcessingPass();
    // Add a kernel to the kernel array. Return it.
    Kernel* AddKernel(RedFoxMaths::Mat4 kernel);
    // Delete a kernel from the kernels array by index.
    void DeleteKernel(int id);
    // Setting an existing kernel by his index.
    void EditKernel(int id, RedFoxMaths::Mat4 kernel);
    void BindKernelBuffer(Memory* tempAlocator);

    void AddPostProcessShader(Allocators *allocator, const char* fragPath);
    void SwapPostProcessShader(int idFirst, int idSecond);
    void RemovePostProcessShader(Memory* arena, int id);
};
} // namespace RedFoxEngine

extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
extern PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
extern PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

#define X(type, name) extern type name;
GL_FUNCTIONS(X)
#undef X
