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
    void RemoveLight(int lightIndex);
};

struct SkyDome
{
    RedFoxMaths::Float3 sunPosition;
    GLuint topTint, botTint, sun, moon, clouds;
    RedFoxMaths::Mat4 model;
};

struct Textures
{
    GLuint textures[128];
    u32 textureCount; 
};

struct Material
{
    vec3 ambient;
    float Opaqueness;

    vec3 diffuse;
    float Shininess;

    vec3 specular;
    int diffuseMap;

    vec3 emissive;
    int normalMap;
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

    RedFoxMaths::Mat4 m_viewProjection;

    Shader m_blinnPhong;
    Shader m_shadow;
    Shader m_sky;

    GLuint m_imguiFramebuffer;

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
    void Draw(RedFoxMaths::Mat4 *p_modelMatrices, u64 *p_modelCountIndex, WindowDimension p_windowDimension, SkyDome p_skyDome, float p_deltaTime);
    void DrawShadowMaps(RedFoxMaths::Mat4 *modelMatrices, u64 *modelCountIndex);
    void DrawSkyDome(SkyDome skyDome, float dt);
    void DrawGameObjects(RedFoxMaths::Mat4 *modelMatrices, u64 *modelCountIndex);
    void DrawModelInstances(Model *model, RedFoxMaths::Mat4 *modelMatrices,
        int instanceCount);
    void DrawModelShadowInstances(Model* model, int instanceCount);
};
} // namespace RedFoxEngine

extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
extern PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
extern PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

#define X(type, name) extern type name;
GL_FUNCTIONS(X)
#undef X
