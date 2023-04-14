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

class Graphics
{
private:
    GLuint m_textureSampler;
    Textures m_textures = {};
    RedFoxMaths::Mat4 m_viewProjection;
    GLuint m_gvshader, m_gfshader, m_gpipeline;
    GLuint m_shadowvshader, m_shadowfshader, m_spipeline;
    GLuint m_skyvshader, m_skyfshader, m_skypipeline;
    GLuint m_imguiFramebuffer;
    u32    m_materialSSBO;
    u32    m_matrixSSBO;
    u32    m_textureSSBO;
    u32    m_shadowMapsSSBO;
    u32    m_spotLightCount;
    GLuint m_spotLightSSBO;
    u32    m_dirLightCount;
    GLuint m_dirLightSSBO;
    u32    m_pointLightCount;
    GLuint m_pointLightSSBO;
    RedFoxMaths::Mat4 *mem;
    u64 *modelCountIndex;
public:
    Model* m_models = nullptr;
    u32    m_modelCount;
    GLuint m_imguiTexture;
    LightStorage lightStorage;
    
    void DrawSkyDome(SkyDome skyDome, float dt);
    void DrawGameObjects();
    // void DrawModelInstances(Model *model, int instanceCount);
    void DrawModelInstances(Model *model, RedFoxMaths::Mat4 *modelMatrices,
        int instanceCount);
    void DrawModelShadowInstances(Model* model, int instanceCount);
    void InitModel(Model *model);
    void InitLights();
    void BindLights();
    void InitModelTextures(ObjModel *model);
    u32 InitTexture(void *data,int height, int width);
    void InitFramebuffer();
    void InitShaders(Memory *tempArena);
    void InitGraphics(Memory *tempArena, WindowDimension dimension);
    void InitImGUIFramebuffer(WindowDimension dimension);
    void UpdateImGUIFrameBuffer(WindowDimension& dimension, WindowDimension content);
    void SetViewProjectionMatrix(RedFoxMaths::Mat4 vp);
    void FillLightBuffer(LightInfo* lights, LightType type);

    void SetLightsCount(int dirCount, int pointCount, int spotCount);
    void CalcShadows(GameObject* objects, int gameObjectCount, Memory* temp);
};
} // namespace RedFoxEngine

extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
extern PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
extern PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

#define X(type, name) extern type name;
GL_FUNCTIONS(X)
#undef X
