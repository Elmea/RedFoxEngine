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
#include "ResourceManager.hpp"

#include "imstb_truetype.h"

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
    Shader m_font;

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

    stbtt_bakedchar cdata[96];
    GLuint m_gFontTexture;

    GLuint m_quadVAO;
    unsigned int m_quadVBO;

public:
    GLuint m_imguiTexture;
    Model* m_models = nullptr;
    u32    m_modelCount;
    LightStorage lightStorage;
    
    void InitModel(Model *model, RedFoxEngine::ResourcesManager *m);
    void InitLights();
    void InitFont(Memory* temp);
    void InitQuad();
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
    void Draw(RedFoxMaths::Mat4 *p_modelMatrices, u64 *p_modelCountIndex, int totalCount, WindowDimension p_windowDimension, SkyDome p_skyDome, float time, float p_deltaTime, RedFoxEngine::ResourcesManager *m);
    void DrawShadowMaps(u64* modelCountIndex);
    void DrawSkyDome(SkyDome skyDome, float dt);
    void DrawGameObjects(u64* modelCountIndex, ResourcesManager *m);
    void DrawModelInstances(Model* model,
        RedFoxMaths::Mat4* modelMatrices, int instanceCount);
    void DrawModelShadowInstances(Model* model, int instanceCount);
    void RenderText(char* text, float x, float y, float scale);

};
} // namespace RedFoxEngine

extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
extern PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
extern PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

#define X(type, name) extern type name;
GL_FUNCTIONS(X)
#undef X
