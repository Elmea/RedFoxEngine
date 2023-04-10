#include "OpenGLGraphics.hpp"
#include "glcorearb.h"

#define MEMORY_IMPLEMENTATION
#include "MyMemory.hpp"

namespace RedFoxEngine
{

void Graphics::InitGraphics(Memory *tempArena, WindowDimension dimension)
{
    InitShaders(tempArena);
    // setup global GL state
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
    }
    InitImGUIFramebuffer(dimension);
    InitGeometryFramebuffer(dimension);
    {
        glCreateBuffers(1, &m_matrixSSBO);
        glNamedBufferStorage(m_matrixSSBO,
            10000 * sizeof(RedFoxMaths::Mat4),
            nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
        glCreateBuffers(1, &m_textureSSBO);

        glCreateBuffers(1, &m_materialSSBO);
        glNamedBufferStorage(m_materialSSBO,
            100 * sizeof(ObjMaterial),
            nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
    }
    glCreateSamplers(1, &m_textureSampler);
    glSamplerParameteri(m_textureSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glSamplerParameteri(m_textureSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glSamplerParameteri(m_textureSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(m_textureSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glNamedBufferStorage(m_textureSSBO, 10000 * sizeof(u64), nullptr, GL_DYNAMIC_STORAGE_BIT);
    wglSwapIntervalEXT(0);
}

void Graphics::InitGeometryFramebuffer(WindowDimension dimension)
{
    glCreateFramebuffers(1, &m_gBuffer);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_gPosition);
    glCreateTextures(GL_TEXTURE_2D, 1, &m_gNormal);
    glCreateTextures(GL_TEXTURE_2D, 1, &m_gAlbedoSpec);

    glTextureParameteri(m_gPosition  , GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_gPosition  , GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(m_gNormal    , GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_gNormal    , GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(m_gAlbedoSpec, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_gAlbedoSpec, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTextureStorage2D(m_gPosition  , 1, GL_RGBA16F, dimension.width,
        dimension.height);
    glTextureStorage2D(m_gNormal    , 1, GL_RGBA16F, dimension.width,
        dimension.height);
    glTextureStorage2D(m_gAlbedoSpec, 1, GL_RGBA8, dimension.width,
        dimension.height);
    glNamedFramebufferTexture(m_gBuffer, GL_COLOR_ATTACHMENT0,
         m_gPosition  , 0);
    glNamedFramebufferTexture(m_gBuffer, GL_COLOR_ATTACHMENT1,
         m_gNormal    , 0);
    glNamedFramebufferTexture(m_gBuffer, GL_COLOR_ATTACHMENT2,
         m_gAlbedoSpec, 0);

    // tell OpenGL which color attachments we'll use (of this framebuffer)
    // for rendering
    unsigned int attachments[] = {
                                    GL_COLOR_ATTACHMENT0,
                                    GL_COLOR_ATTACHMENT1,
                                    GL_COLOR_ATTACHMENT2
                                 };

    glNamedFramebufferDrawBuffers(m_gBuffer, sizeof(attachments) / sizeof(int),
        attachments);
    // create and attach depth buffer (renderbuffer)
    unsigned int rboDepth;
    glCreateRenderbuffers(1, &rboDepth);
    glNamedRenderbufferStorage(rboDepth, GL_DEPTH_COMPONENT,
        dimension.width, dimension.height);
    glNamedFramebufferRenderbuffer(m_gBuffer, GL_DEPTH_ATTACHMENT,
        GL_RENDERBUFFER, rboDepth);

    // finally check if framebuffer is complete
    int error = 0;
    if ((error = glCheckNamedFramebufferStatus(m_gBuffer, GL_FRAMEBUFFER))
        != GL_FRAMEBUFFER_COMPLETE)
        __debugbreak();
}

void Graphics::InitImGUIFramebuffer(WindowDimension dimension)
{
    glCreateFramebuffers(1, &m_imguiFramebuffer);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_imguiTexture);
    glTextureParameteri(m_imguiTexture  , GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_imguiTexture  , GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureStorage2D(m_imguiTexture  , 1, GL_RGBA8, dimension.width,
        dimension.height);
    glNamedFramebufferTexture(m_imguiFramebuffer, GL_COLOR_ATTACHMENT0,
         m_imguiTexture, 0);
    unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0};
    glNamedFramebufferDrawBuffers(m_imguiFramebuffer, 1, attachments);
    glCreateRenderbuffers(1, &m_rboIMGUI);
    glNamedRenderbufferStorage(m_rboIMGUI, GL_DEPTH24_STENCIL8,
        dimension.width, dimension.height);
    glNamedFramebufferRenderbuffer(m_imguiFramebuffer, GL_DEPTH_ATTACHMENT,
        GL_RENDERBUFFER, m_rboIMGUI);

    if (glCheckNamedFramebufferStatus(m_imguiFramebuffer, GL_FRAMEBUFFER) !=
        GL_FRAMEBUFFER_COMPLETE)
        __debugbreak();
}

void Graphics::UpdateImGUIFrameBuffer(WindowDimension &dimension,
    WindowDimension content)
{
    dimension = content;
    glDeleteTextures(1, &m_imguiTexture);
    glDeleteFramebuffers(1, &m_imguiFramebuffer);
    InitImGUIFramebuffer(dimension);

    glDeleteTextures(1, &m_gPosition);
    glDeleteTextures(1, &m_gNormal);
    glDeleteTextures(1, &m_gAlbedoSpec);
    glDeleteFramebuffers(1, &m_gBuffer);
    InitGeometryFramebuffer(dimension);
}

void Graphics::InitQuad()
{
    float vertices[] = {
            // positions  // texture Coords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
             1.0f,  1.0f,  1.0f, 1.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
        };
    unsigned int vbo;
    {
        glCreateBuffers(1, &vbo);
        glNamedBufferStorage(vbo, sizeof(vertices), vertices, 0);
    }
    // vertex input
    {
        glCreateVertexArrays(1, &m_quadVAO);

        int vbuf_index = 0;
        glVertexArrayVertexBuffer(m_quadVAO, vbuf_index, vbo, 0,
            4 * sizeof(float));

        int a_pos = 0;
        glEnableVertexArrayAttrib (m_quadVAO, a_pos);
        glVertexArrayAttribFormat (m_quadVAO, a_pos, 2, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribBinding(m_quadVAO, a_pos, vbuf_index);

        int a_uv = 1;
        glEnableVertexArrayAttrib (m_quadVAO, a_uv);
        glVertexArrayAttribFormat (m_quadVAO, a_uv, 2, GL_FLOAT, GL_FALSE,
            sizeof(float) * 2);
        glVertexArrayAttribBinding(m_quadVAO, a_uv, vbuf_index);
    }
}

void Graphics::InitModel(Model *model)
{
    // vertex buffer containing triangle vertices
    unsigned int vbo;
    {
        glCreateBuffers(1, &vbo);
        glNamedBufferStorage(vbo, model->obj.vertexCount * (sizeof(ObjVertex)),
            model->obj.vertices, 0);
    }
    unsigned int ebo;
    {
        glCreateBuffers(1, &ebo);
        glNamedBufferStorage(ebo, model->obj.indexCount * sizeof(u32),
            model->obj.indices, 0);
    }
    // vertex input
    {
        glCreateVertexArrays(1, &model->vao);

        int vbuf_index = 0;
        glVertexArrayVertexBuffer(model->vao, vbuf_index, vbo, 0,
            sizeof(ObjVertex));

        glVertexArrayElementBuffer(model->vao, ebo);

        int a_pos = 0;
        glEnableVertexArrayAttrib (model->vao, a_pos);
        glVertexArrayAttribFormat (model->vao, a_pos, 3, GL_FLOAT, GL_FALSE,
            offsetof(struct ObjVertex, position));
        glVertexArrayAttribBinding(model->vao, a_pos, vbuf_index);

        int a_normal = 1;
        glEnableVertexArrayAttrib (model->vao, a_normal);
        glVertexArrayAttribFormat (model->vao, a_normal, 3, GL_FLOAT, GL_FALSE,
            offsetof(struct ObjVertex, normal));
        glVertexArrayAttribBinding(model->vao, a_normal, vbuf_index);

        int a_uv = 2;
        glEnableVertexArrayAttrib (model->vao, a_uv);
        glVertexArrayAttribFormat (model->vao, a_uv, 2, GL_FLOAT, GL_FALSE,
            offsetof(struct ObjVertex, textureUV));
        glVertexArrayAttribBinding(model->vao, a_uv, vbuf_index);
        int a_textureID = 3;
        glEnableVertexArrayAttrib (model->vao, a_textureID);
        glVertexArrayAttribIFormat (model->vao, a_textureID, 1, GL_UNSIGNED_INT,
            offsetof(struct ObjVertex, materialID));
        glVertexArrayAttribBinding(model->vao, a_textureID, vbuf_index);
    }
    InitModelTextures(&model->obj);
    DeInitGraphicsObj(&model->obj);
}

u32 Graphics::InitTexture(void *data,int height, int width)
{
    GLuint texture;
    glCreateTextures(GL_TEXTURE_2D, 1, &texture);
    // m_textures.textures[i] = i;
    glTextureStorage2D(texture, 1, GL_RGBA8, width, height);
    glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA,
         GL_UNSIGNED_BYTE, data);
    GLuint64 textureHandle = glGetTextureSamplerHandleARB(texture, m_textureSampler);
    glMakeTextureHandleResidentARB(textureHandle);
    return (texture);
}

void Graphics::InitModelTextures(ObjModel *model)
{
    // TODO(V. Caraulan): Wrap in waiting for thread function
    WaitForSingleObject(model->images.thread, INFINITE);

    for (int i = 0; i < (int)model->materials.count; i++)
    {
        model->materials.material[i].diffuseMap.index0 += m_textures.textureCount;
        model->materials.material[i].normalMap.index0 += m_textures.textureCount;
    }
    for (int i = 0; i < (int)model->images.count; i++)
    {
        if (model->images.data[i].height && model->images.data[i].width)
        {
            m_textures.textures[m_textures.textureCount++] = InitTexture(model->images.data[i].data,
                model->images.data[i].height, model->images.data[i].width);
        }
    }
}

static void CompileShaders(const char *vertexShaderSource,
                    const char *fragmentShaderSource,
    GLuint &vert, GLuint &frag, GLuint &pipeline)
{
    vert = glCreateShaderProgramv(GL_VERTEX_SHADER, 1,
        &vertexShaderSource);
    frag = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1,
        &fragmentShaderSource);

    GLint linked;
    glGetProgramiv(vert, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        char message[1024];
        glGetProgramInfoLog(vert, sizeof(message), nullptr, message);
        OutputDebugStringA(message);
        Assert(!"Failed to create vertex shader!");
    }

    glGetProgramiv(frag, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        char message[1024];
        glGetProgramInfoLog(frag, sizeof(message), nullptr, message);
        OutputDebugStringA(message);
        Assert(!"Failed to create fragment shader!");
    }
    glGenProgramPipelines(1, &pipeline);
    glUseProgramStages(pipeline, GL_VERTEX_SHADER_BIT, vert);
    glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, frag);
}

void Graphics::InitShaders(Memory *tempArena)
{
    int tempSize = tempArena->usedSize;
    // fragment & vertex shaders for drawing triangle
    MyString vertexShaderSource = OpenAndReadEntireFile(
        "Shaders\\gBuffer.vert", tempArena);
    MyString fragmentShaderSource = OpenAndReadEntireFile(
        "Shaders\\gBuffer.frag", tempArena);
    CompileShaders(vertexShaderSource.data, fragmentShaderSource.data,
        m_gvshader, m_gfshader, m_gpipeline);
    tempArena->usedSize = tempSize;
    vertexShaderSource = OpenAndReadEntireFile(
        "Shaders\\vertex.vert", tempArena);
    fragmentShaderSource = OpenAndReadEntireFile(
        "Shaders\\blin_phong.frag", tempArena);
    CompileShaders(vertexShaderSource.data, fragmentShaderSource.data,
        m_vshader, m_fshader, m_pipeline);
    tempArena->usedSize = tempSize;
}

void Graphics::SetViewProjectionMatrix(RedFoxMaths::Mat4 vp)
{
    m_viewProjection = vp;
}

void Graphics::DrawGBuffer(GameObject *objects, int gameObjectCount,
    Memory *temp)
{
    //NOTE: here we clear the 0 framebuffer
    glClearColor(0, 0, 0, 0.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // clear screen
    glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_BLEND);

    int batchCount = 10000;
    RedFoxMaths::Mat4 *mem = (RedFoxMaths::Mat4 *)MyMalloc(temp,
        sizeof(RedFoxMaths::Mat4) * batchCount * (m_modelCount + 1));
    u64 *modelCountIndex = (u64 *)MyMalloc(temp,
        sizeof(u64) * m_modelCount);
    memset(modelCountIndex, 0, sizeof(u64) * m_modelCount);
    for(int index = 0;index < gameObjectCount; index++)
    {
        if (objects[index].model)
        {
            u64 modelIndex = objects[index].model - m_models;
            u64 countIndex = modelCountIndex[modelIndex];
            mem[countIndex + (batchCount * modelIndex)] =
                objects[index].GetWorldMatrix().GetTransposedMatrix();
            modelCountIndex[modelIndex]++;
        }
    }
    GLint u_matrix = 0;
    glProgramUniformMatrix4fv(m_gvshader, u_matrix, 1, GL_TRUE,
        m_viewProjection.AsPtr());

    for (int i = 0; i < (int)m_modelCount; i++)
    {
        if (modelCountIndex[i])
            DrawModelInstances(&m_models[i], &mem[batchCount * i],
                modelCountIndex[i]);
    }
}

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

void Graphics::DrawModelInstances(Model *model,
    RedFoxMaths::Mat4 *modelMatrices, int instanceCount)
{
    // provide vertex input
    GLuint64 textureHandles[128];
    for (int i = 0; i < (int)m_textures.textureCount; i++)
        textureHandles[i] = glGetTextureSamplerHandleARB(m_textures.textures[i], m_textureSampler);
    glNamedBufferSubData(m_matrixSSBO,	0, sizeof(RedFoxMaths::Mat4) * instanceCount, modelMatrices);
    glNamedBufferSubData(m_textureSSBO,	0, sizeof(u64) * (m_textures.textureCount), textureHandles);
    Material materials[100] = {};

    for (int i = 0; i < (int)model->obj.materials.count; i++)
    {
        materials[i].ambient    = model->obj.materials.material[i].ambient;
        materials[i].Opaqueness = model->obj.materials.material[i].Opaqueness;
        materials[i].diffuse    = model->obj.materials.material[i].diffuse;
        materials[i].Shininess  = model->obj.materials.material[i].Shininess;
        materials[i].specular   = model->obj.materials.material[i].specular;
        materials[i].emissive   = model->obj.materials.material[i].emissive;
        if (model->obj.materials.material[i].hasTexture == 0)
            materials[i].diffuseMap = -1;
        else
            materials[i].diffuseMap = model->obj.materials.material[i].diffuseMap.index0;
        if (model->obj.materials.material[i].hasNormal == 0)
            materials[i].normalMap = -1;
        else
            materials[i].normalMap = model->obj.materials.material[i].normalMap.index0;
    }
    glNamedBufferSubData(m_materialSSBO,	0, sizeof(ObjMaterial) * model->obj.materials.count, materials);
    glBindProgramPipeline(m_gpipeline);
    glBindVertexArray(model->vao);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_matrixSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_textureSSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_materialSSBO);

    glDrawElementsInstanced(GL_TRIANGLES, model->obj.indexCount,
        GL_UNSIGNED_INT, 0, instanceCount);
}

void Graphics::DrawQuad(WindowDimension dimension)
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_imguiFramebuffer);
    // clear screen
    glEnable(GL_BLEND);
    glBindProgramPipeline(m_pipeline);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    // Bind the buffer to a binding point
    glBindVertexArray(m_quadVAO);
    BindLights();
    glBindTextureUnit(0, m_gPosition);
    glBindTextureUnit(1, m_gNormal);
    glBindTextureUnit(2, m_gAlbedoSpec);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
#if 0 //TODO this might be necesary if we want to draw objects after defered shading
    glBlitNamedFramebuffer(m_gBuffer, 0, 0, 0, dimension.width, dimension.height,
                           0, 0, dimension.width, dimension.height,
                           GL_DEPTH_BUFFER_BIT, GL_NEAREST);
#endif
}

} // namespace RedFoxEngine
