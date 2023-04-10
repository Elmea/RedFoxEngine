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
    GLuint m_vshader, m_fshader, m_pipeline;
    GLuint m_gvshader, m_gfshader, m_gpipeline;
    GLuint m_rboIMGUI;
    GLuint m_quadVAO;
    GLuint m_gBuffer;
    // GLuint m_booleanBuffer;
    GLuint m_imguiFramebuffer;
    u32    m_materialSSBO;
    u32    m_matrixSSBO;
    u32    m_textureSSBO;
    u32    m_spotLightCount;
    GLuint m_spotLightSSBO;
    u32    m_dirLightCount;
    GLuint m_dirLightSSBO;
    u32    m_pointLightCount;
    GLuint m_pointLightSSBO;
    GLuint m_gPosition, m_gNormal, m_gAlbedoSpec;
public:
    Model *m_models = nullptr;
    u32    m_modelCount;
    GLuint m_imguiTexture;
    void DrawGBuffer(GameObject *objects, int gameObjectCount, Memory *temp);
    void DrawQuad(WindowDimension dimension);
    void DrawModel(Model *model);
    // void DrawModelInstances(Model *model, int instanceCount);
    void DrawModelInstances(Model *model, RedFoxMaths::Mat4 *modelMatrices,
        int instanceCount);
    void InitQuad();
    void InitModel(Model *model);
    void InitLights();
    void BindLights();
    DirLight *GetDirLightBuffer(int *lightCount);
    void SetPointLightBuffer(PointLight* pointLight, int lightCount);
    void ReleaseDirLightBuffer();
    PointLight *GetPointLightBuffer(int *lightCount);
    void ReleasePointLightBuffer();
    void InitModelTextures(ObjModel *model);
    u32 InitTexture(void *data,int height, int width);
    void InitFramebuffer();
    void InitShaders(Memory *tempArena);
    void InitGraphics(Memory *tempArena, WindowDimension dimension);
    void InitGeometryFramebuffer(WindowDimension dimension);
    void InitImGUIFramebuffer(WindowDimension dimension);
    void UpdateImGUIFrameBuffer(WindowDimension &dimension, WindowDimension content);
    void SetViewProjectionMatrix(RedFoxMaths::Mat4 vp);
};
} // namespace RedFoxEngine

extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
extern PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
extern PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;

#define X(type, name) extern type name;
GL_FUNCTIONS(X)
#undef X
