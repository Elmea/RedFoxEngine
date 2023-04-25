/* date = February 25th 2023 4:19 am */
#pragma once

#ifdef _WIN32
#include "Win32Platform.hpp"
#endif

#include <GL/gl.h>
#include "OpenGLFunctions.hpp"
#include "ObjParser.hpp"
#include "imgui.h"
#include "Light.hpp"
#include "Scene.hpp"

namespace RedFoxEngine
{

struct Material
{
    RedFoxMaths::Float3 ambient;
    float Opaqueness;

    RedFoxMaths::Float3 diffuse;
    float Shininess;

    RedFoxMaths::Float3 specular;
    int diffuseMap;

    RedFoxMaths::Float3 emissive;
    int normalMap;
};

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
};

class Graphics
{
private:
    GLuint m_textureSampler;
    Textures m_textures = {};
    u32 m_materialCount;
    RedFoxMaths::Mat4 m_viewProjection;

    Shader m_blinnPhong;
    Shader m_shadow;
    Shader m_sky;

    GLuint m_imguiFramebuffer;

    u32    m_indexCount = 0;
    u32    m_vertexCount = 0;
    u32    m_vertexBufferObject;
    u32    m_vertexArrayObject;
    u32    m_ebo;

    u32    m_materialSSBO;
    u32    m_matrixSSBO;
    u32    m_textureSSBO;
    u32    m_shadowMapsSSBO;
public:
    GLuint m_imguiTexture;
    Model* m_models = nullptr;
    u32    m_modelCount;
    LightStorage lightStorage;
    
    void InitModel(Model *model);
    void InitLights();
    void InitModelTextures(ObjModel *model);
    u32 InitTexture(void *data, int width, int height, bool resident, bool repeat);
    void InitFramebuffer();
    void InitShaders(Memory *tempArena);
    void InitGraphics(Memory *tempArena, WindowDimension dimension);
    void InitImGUIFramebuffer(WindowDimension dimension);
    void BindLights();
    void SetViewProjectionMatrix(RedFoxMaths::Mat4 vp);
    void FillLightBuffer(LightInfo* lights, LightType type);
    void UpdateImGUIFrameBuffer(WindowDimension& dimension, WindowDimension content);
    void UpdateModelMatrices(GameObject* objects, int gameObjectCount, Memory* temp);
    void PushMaterial(Material *materials, int count);
    void Draw(Scene *m_scene, WindowDimension p_windowDimension, float p_time, float p_delta);
    void DrawShadowMaps(u64* modelCountIndex);
    void DrawSkyDome(SkyDome skyDome, float dt);
    void DrawGameObjects(u64* modelCountIndex);
    void DrawModelInstances(Model* model,
        RedFoxMaths::Mat4* modelMatrices, int instanceCount);
    void DrawModelShadowInstances(Model* model, int instanceCount);
};
} // namespace RedFoxEngine

extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
extern PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
extern PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

#define X(type, name) extern type name;
GL_FUNCTIONS(X)
#undef X
