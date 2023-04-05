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
    glCreateBuffers(1, &m_booleanBuffer);
    glNamedBufferStorage(m_booleanBuffer, sizeof(int), nullptr,
        GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
    // set to FALSE to disable vsync
    wglSwapIntervalEXT(1);
}

void Graphics::InitGeometryFramebuffer(WindowDimension dimension)
{
    glCreateFramebuffers(1, &m_gBuffer);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_gPosition);
    glCreateTextures(GL_TEXTURE_2D, 1, &m_gNormal);
    glCreateTextures(GL_TEXTURE_2D, 1, &m_gAlbedoSpec);
    glCreateTextures(GL_TEXTURE_2D, 1, &m_gTangent);

    glTextureParameteri(m_gPosition  , GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_gPosition  , GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(m_gNormal    , GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_gNormal    , GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(m_gAlbedoSpec, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_gAlbedoSpec, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(m_gTangent, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_gTangent, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTextureStorage2D(m_gPosition  , 1, GL_RGBA16F, dimension.width,
        dimension.height);
    glTextureStorage2D(m_gNormal    , 1, GL_RGBA16F, dimension.width,
        dimension.height);
    glTextureStorage2D(m_gAlbedoSpec, 1, GL_RGBA8, dimension.width,
        dimension.height);
    glTextureStorage2D(m_gTangent, 1, GL_RGBA16F, dimension.width,
        dimension.height);

    glNamedFramebufferTexture(m_gBuffer, GL_COLOR_ATTACHMENT0,
         m_gPosition  , 0);
    glNamedFramebufferTexture(m_gBuffer, GL_COLOR_ATTACHMENT1,
         m_gNormal    , 0);
    glNamedFramebufferTexture(m_gBuffer, GL_COLOR_ATTACHMENT2,
         m_gAlbedoSpec, 0);
    glNamedFramebufferTexture(m_gBuffer, GL_COLOR_ATTACHMENT3,
         m_gTangent, 0);

    // tell OpenGL which color attachments we'll use (of this framebuffer)
    // for rendering
    unsigned int attachments[] = {GL_COLOR_ATTACHMENT0,
                                   GL_COLOR_ATTACHMENT1,
                                   GL_COLOR_ATTACHMENT2,
                                   GL_COLOR_ATTACHMENT3};
    glNamedFramebufferDrawBuffers(m_gBuffer, 4, attachments);
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
    glDeleteTextures(1, &m_gTangent);
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

struct NormalVertex
{
    vec3 position;
    vec3 normal;
    vec3 tangent;
    vec2 textureUV;
};

void Graphics::InitNormalMappedModel(Model *model, Memory *temp)
{
    // vertex buffer containing triangle vertices

    NormalVertex *vertices = (NormalVertex*)MyMalloc(temp, sizeof(NormalVertex) * model->obj.vertexCount);
    unsigned int vbo;
    {
        glCreateBuffers(1, &vbo);
        for (int i = 0; i < (int)model->obj.vertexCount; i += 3)
        {
            int v = i;
            float deltaUV1x = model->obj.vertices[v + 1].textureUV.x - model->obj.vertices[i].textureUV.x;
            float deltaUV1y = model->obj.vertices[v + 0].textureUV.y - model->obj.vertices[i].textureUV.y;
            float deltaUV2x = model->obj.vertices[i + 2].textureUV.x - model->obj.vertices[i].textureUV.x;
            float deltaUV2y = model->obj.vertices[i + 2].textureUV.y - model->obj.vertices[i].textureUV.y;
            vec3 edge1;
            vec3 edge2;
            edge1.x = model->obj.vertices[i + 1].position.x - model->obj.vertices[i].position.x;
            edge1.y = model->obj.vertices[i + 1].position.y - model->obj.vertices[i].position.y;
            edge1.z = model->obj.vertices[i + 1].position.z - model->obj.vertices[i].position.z;
            edge2.x = model->obj.vertices[i + 2].position.x - model->obj.vertices[i].position.x;
            edge2.y = model->obj.vertices[i + 2].position.y - model->obj.vertices[i].position.y;
            edge2.z = model->obj.vertices[i + 2].position.z - model->obj.vertices[i].position.z;
            float f = 1.0f / (deltaUV1x * deltaUV2y - deltaUV2x * deltaUV1y);

            vertices[i + 0].normal = model->obj.vertices[i + 0].normal;
            vertices[i + 1].normal = model->obj.vertices[i + 1].normal;
            vertices[i + 2].normal = model->obj.vertices[i + 2].normal;
            vertices[i + 0].position = model->obj.vertices[i + 0].position;
            vertices[i + 1].position = model->obj.vertices[i + 1].position;
            vertices[i + 2].position = model->obj.vertices[i + 2].position;
            vertices[i + 0].textureUV = model->obj.vertices[i + 0].textureUV;
            vertices[i + 1].textureUV = model->obj.vertices[i + 1].textureUV;
            vertices[i + 2].textureUV = model->obj.vertices[i + 2].textureUV;
            vertices[i].tangent.x = f * (deltaUV2y * edge1.x - deltaUV1y * edge2.x);
            vertices[i].tangent.y = f * (deltaUV2y * edge1.y - deltaUV1y * edge2.y);
            vertices[i].tangent.z = f * (deltaUV2y * edge1.z - deltaUV1y * edge2.z);
            vertices[i + 1].tangent = vertices[i].tangent;
            vertices[i + 2].tangent = vertices[i].tangent;
        }
        glNamedBufferStorage(vbo,
            model->obj.vertexCount * sizeof(NormalVertex),
            vertices, 0);
    }
    unsigned int ebo;
    {
        glCreateBuffers(1, &ebo);
        glNamedBufferStorage(ebo, model->obj.indexCount * sizeof(u32),
            model->obj.indices, 0);
    }
    {
        const int bufferBatchSize = 768; //TODO: test this number on different
                                         // GPUs   
        glCreateBuffers(1, &model->vbm);
        glNamedBufferStorage(model->vbm,
            bufferBatchSize * sizeof(RedFoxMaths::Mat4),
            nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
    }
    // vertex input
    {
        glCreateVertexArrays(1, &model->vao);

        int vbuf_index = 0;
        glVertexArrayVertexBuffer(model->vao, vbuf_index, vbo, 0,
            sizeof(NormalVertex));

        int vbuf_matrix = 4;
        glVertexArrayVertexBuffer(model->vao, vbuf_matrix, model->vbm, 0,
            sizeof(RedFoxMaths::Mat4));

        glVertexArrayElementBuffer(model->vao, ebo);

        int a_pos = 0;
        glEnableVertexArrayAttrib (model->vao, a_pos);
        glVertexArrayAttribFormat (model->vao, a_pos, 3, GL_FLOAT, GL_FALSE,
            offsetof(struct NormalVertex, position));
        glVertexArrayAttribBinding(model->vao, a_pos, vbuf_index);

        int a_normal = 1;
        glEnableVertexArrayAttrib (model->vao, a_normal);
        glVertexArrayAttribFormat (model->vao, a_normal, 3, GL_FLOAT, GL_FALSE,
            offsetof(struct NormalVertex, normal));
        glVertexArrayAttribBinding(model->vao, a_normal, vbuf_index);

        int a_tangent = 2;
        glEnableVertexArrayAttrib (model->vao, a_tangent);
        glVertexArrayAttribFormat (model->vao, a_tangent, 3, GL_FLOAT, GL_FALSE,
            offsetof(struct NormalVertex, normal));
        glVertexArrayAttribBinding(model->vao, a_tangent, vbuf_index);
        
        int a_uv = 3;
        glEnableVertexArrayAttrib (model->vao, a_uv);
        glVertexArrayAttribFormat (model->vao, a_uv, 2, GL_FLOAT, GL_FALSE,
            offsetof(struct NormalVertex, textureUV));
        glVertexArrayAttribBinding(model->vao, a_uv, vbuf_index);

        int a_matrix = 4;
        // This is the matrix we're going to use for instanced models
        glEnableVertexArrayAttrib (model->vao, a_matrix);
        glVertexArrayAttribFormat (model->vao, a_matrix, 4, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribBinding(model->vao, a_matrix, vbuf_matrix);

        glEnableVertexArrayAttrib (model->vao, a_matrix + 1);
        glVertexArrayAttribFormat (model->vao, a_matrix + 1, 4, GL_FLOAT, GL_FALSE,
            (sizeof(float) * 4));
        glVertexArrayAttribBinding(model->vao, a_matrix + 1, vbuf_matrix);

        glEnableVertexArrayAttrib (model->vao, a_matrix + 2);
        glVertexArrayAttribFormat (model->vao, a_matrix + 2, 4, GL_FLOAT, GL_FALSE,
            (2 * sizeof(float) * 4));
        glVertexArrayAttribBinding(model->vao, a_matrix + 2, vbuf_matrix);

        glEnableVertexArrayAttrib (model->vao, a_matrix + 3);
        glVertexArrayAttribFormat (model->vao, a_matrix + 3, 4, GL_FLOAT, GL_FALSE,
            (3 * sizeof(float) * 4));
        glVertexArrayAttribBinding(model->vao, a_matrix + 3, vbuf_matrix);

        glVertexArrayBindingDivisor(model->vao, a_matrix + 0, 1);
        glVertexArrayBindingDivisor(model->vao, a_matrix + 1, 1);
        glVertexArrayBindingDivisor(model->vao, a_matrix + 2, 1);
        glVertexArrayBindingDivisor(model->vao, a_matrix + 3, 1);
    }
    InitTexture(&model->obj);
    DeInitGraphicsObj(&model->obj);
}

void Graphics::InitModel(Model *model)
{
    // vertex buffer containing triangle vertices

    unsigned int vbo;
    {
        glCreateBuffers(1, &vbo);
        glNamedBufferStorage(vbo,
            model->obj.vertexCount * sizeof(ObjVertex),
            model->obj.vertices, 0);
              // for (int i = 0; i < model->obj.vertexCount; i += 3)
              // {
              // 	int v = i;
              // 	vec2 deltaUV1 = { model->obj.vertices[v + 1].textureUV - model->obj.vertices[i].textureUV };
              // 	vec2 deltaUV2 = { model->obj.vertices[i + 2].textureUV - model->obj.vertices[i].textureUV };
              // 	Vec3 edge1 = { model->obj.vertices[i + 1].position - model->obj.vertices[i].position };
              // 	Vec3 edge2 = { model->obj.vertices[i + 2].position - model->obj.vertices[i].position };
              // 	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

              // 	model->obj.vertices[i].tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
              // 	model->obj.vertices[i].tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
              // 	model->obj.vertices[i].tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
              // 	model->obj.vertices[i + 1].tangent = vertices[i].tangent;
              // 	model->obj.vertices[i + 2].tangent = vertices[i].tangent;
              // }
    }
    unsigned int ebo;
    {
        glCreateBuffers(1, &ebo);
        glNamedBufferStorage(ebo, model->obj.indexCount * sizeof(u32),
            model->obj.indices, 0);
    }
    {
        const int bufferBatchSize = 768; //TODO: test this number on different
                                         // GPUs   
        glCreateBuffers(1, &model->vbm);
        glNamedBufferStorage(model->vbm,
            bufferBatchSize * sizeof(RedFoxMaths::Mat4),
            nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
    }
    // vertex input
    {
        glCreateVertexArrays(1, &model->vao);

        int vbuf_index = 0;
        glVertexArrayVertexBuffer(model->vao, vbuf_index, vbo, 0,
            sizeof(ObjVertex));

        int vbuf_matrix = 3;
        glVertexArrayVertexBuffer(model->vao, vbuf_matrix, model->vbm, 0,
            sizeof(RedFoxMaths::Mat4));

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

        int a_matrix = 3;
        // This is the matrix we're going to use for instanced models
        glEnableVertexArrayAttrib (model->vao, a_matrix);
        glVertexArrayAttribFormat (model->vao, a_matrix, 4, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribBinding(model->vao, a_matrix, vbuf_matrix);

        glEnableVertexArrayAttrib (model->vao, 4);
        glVertexArrayAttribFormat (model->vao, 4, 4, GL_FLOAT, GL_FALSE,
            (sizeof(float) * 4));
        glVertexArrayAttribBinding(model->vao, 4, vbuf_matrix);

        glEnableVertexArrayAttrib (model->vao, 5);
        glVertexArrayAttribFormat (model->vao, 5, 4, GL_FLOAT, GL_FALSE,
            (2 * sizeof(float) * 4));
        glVertexArrayAttribBinding(model->vao, 5, vbuf_matrix);

        glEnableVertexArrayAttrib (model->vao, 6);
        glVertexArrayAttribFormat (model->vao, 6, 4, GL_FLOAT, GL_FALSE,
            (3 * sizeof(float) * 4));
        glVertexArrayAttribBinding(model->vao, 6, vbuf_matrix);

        glVertexArrayBindingDivisor(model->vao, 3, 1);
        glVertexArrayBindingDivisor(model->vao, 4, 1);
        glVertexArrayBindingDivisor(model->vao, 5, 1);
        glVertexArrayBindingDivisor(model->vao, 6, 1);
    }
    InitTexture(&model->obj);
    DeInitGraphicsObj(&model->obj);
}

u32 Graphics::InitTexture(void *data,int height, int width)
{
    GLuint texture;
    glCreateTextures(GL_TEXTURE_2D, 1, &texture);

    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER,
        GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureStorage2D(texture, 1, GL_RGBA8, width, height);
    glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA,
         GL_UNSIGNED_BYTE, data);
    return (texture);
}

void Graphics::InitTexture(ObjModel *model)
{
    GLuint texture[128];

    // TODO(V. Caraulan): Wrap in waiting for thread function
    WaitForSingleObject(model->images.thread, INFINITE);
    for (int i = 0; i < (int)model->images.count; i++)
    {
        if (model->images.data[i].height && model->images.data[i].width)
            texture[i] = InitTexture(model->images.data[i].data,
        model->images.data[i].height, model->images.data[i].width);
    }
    
    for (int i = 0; i < (int)model->materials.count; i++)
    {
        int temp = model->materials.material[i].diffuseMap.index0;
        model->materials.material[i].diffuseMap.index0 = texture[temp];
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
    // fragment & vertex shaders for drawing triangle
    MyString vertexShaderSource = OpenAndReadEntireFile(
        "Shaders\\normalGBuffer.vert", tempArena);
    MyString fragmentShaderSource = OpenAndReadEntireFile(
        "Shaders\\normalGBuffer.frag", tempArena);
    CompileShaders(vertexShaderSource.data, fragmentShaderSource.data,
        m_gvshader, m_gfshader, m_gpipeline);

    vertexShaderSource = OpenAndReadEntireFile(
        "Shaders\\vertex.vert", tempArena);
    fragmentShaderSource = OpenAndReadEntireFile(
        "Shaders\\blin_phong.frag", tempArena);
    CompileShaders(vertexShaderSource.data, fragmentShaderSource.data,
        m_vshader, m_fshader, m_pipeline);
}

void Graphics::SetViewProjectionMatrix(RedFoxMaths::Mat4 vp)
{
    m_viewProjection = vp;
}

void Graphics::DrawGBuffer(GameObject *objects, int gameObjectCount,
    Memory *temp)
{
    //NOTE: here we clear the 0 framebuffer
    glClearColor(0, 0, 0, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // clear screen
    glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindProgramPipeline(m_gpipeline);
    glDisable(GL_BLEND);

    GLint u_matrix = 0;
    glProgramUniformMatrix4fv(m_gvshader, u_matrix, 1, GL_TRUE,
        m_viewProjection.AsPtr());

    int batchCount = 768; //TODO figure out a good value for this
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
            countIndex = modelCountIndex[modelIndex];
            if (countIndex == (u64)batchCount)
            {
                DrawModelInstances(&m_models[modelIndex],
                    &mem[batchCount * modelIndex], countIndex);
                modelCountIndex[modelIndex] = 0;
            }
        }
    }
    for (int i = 0; i < (int)m_modelCount; i++)
    {
        if (modelCountIndex[i])
            DrawModelInstances(&m_models[i], &mem[batchCount * i],
                modelCountIndex[i]);
    }
}

void Graphics::DrawModelInstances(Model *model,
    RedFoxMaths::Mat4 *modelMatrices, int instanceCount)
{
    // provide vertex input
    glBindVertexArray(model->vao);
    glNamedBufferSubData(model->vbm,	0,
        sizeof(RedFoxMaths::Mat4) * instanceCount, modelMatrices);

    GLint diffuseMap = 0;
    glBindTextureUnit(diffuseMap, 0);
    GLint normalMap = 1;
    glBindTextureUnit(normalMap, 0);

    for (int i = 0; i < (int)model->obj.meshCount; i++)
    {
        ObjMesh *mesh = &model->obj.meshes[i];
        ObjMaterial *material = &model->obj.materials.material[mesh->materialIndex];
        //TODO: make sure all objs have a default material.
        int hasNormalMap = 0;
        if (material)
        {
            if (material->hasTexture)
                glBindTextureUnit(diffuseMap, material->diffuseMap.index0);
            else
                glBindTextureUnit(diffuseMap, 0);
            if (material->hasNormal)
            {
                hasNormalMap = 1;
                glBindTextureUnit(normalMap, material->normalMap.index0);
            }
        }
        glNamedBufferSubData(m_booleanBuffer, 0, sizeof(int) * 1, &hasNormalMap);
        // TODO: create a default 'missing' texture
        glDrawElementsInstanced(GL_TRIANGLES, mesh->indexCount ,GL_UNSIGNED_INT,
            (void *)((mesh->indexStart) * sizeof(u32)), instanceCount);
    }
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
    glBindTextureUnit(3, m_gTangent);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
#if 0 //TODO this might be necesary if we want to draw objects after defered shading
    glBlitNamedFramebuffer(m_gBuffer, 0, 0, 0, dimension.width, dimension.height,
                           0, 0, dimension.width, dimension.height,
                           GL_DEPTH_BUFFER_BIT, GL_NEAREST);
#endif
}

} // namespace RedFoxEngine
