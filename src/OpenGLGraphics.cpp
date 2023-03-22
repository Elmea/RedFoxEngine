#include "OpenGLGraphics.hpp"
#include "glcorearb.h"

#define MEMORY_IMPLEMENTATION
#include "MyMemory.hpp"

namespace RedFoxEngine
{

void Graphics::UpdateImGUIFrameBuffer(WindowDimension &dimension,
    WindowDimension content)
{
    glDeleteTextures(1, &m_imguiTexture);
    glDeleteFramebuffers(1, &m_imguiFramebuffer);
    glCreateFramebuffers(1, &m_imguiFramebuffer);
    glCreateTextures(GL_TEXTURE_2D, 1, &m_imguiTexture);
    glNamedFramebufferTexture(m_imguiFramebuffer,
        GL_COLOR_ATTACHMENT0, m_imguiTexture , 0);
    unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0};
    glNamedFramebufferDrawBuffers(m_imguiFramebuffer, 1,
        attachments);
    glTextureParameteri(m_imguiTexture,
        GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_imguiTexture,
        GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureStorage2D(m_imguiTexture , 1, GL_RGBA8,
        content.width, content.height);
    glNamedRenderbufferStorage(m_rboIMGUI, GL_DEPTH_COMPONENT,
        content.width, content.height);
    glNamedFramebufferRenderbuffer(m_imguiFramebuffer,
        GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 1);
    dimension = content;
}

void Graphics::InitImGUIFrameBuffer(WindowDimension dimension)
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

    int error = 0;
    if ((error = glCheckNamedFramebufferStatus(m_imguiFramebuffer, GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
        __debugbreak();
}

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
    InitImGUIFrameBuffer(dimension);
#if 0
    glCreateFramebuffers(1, &m_imguiFramebuffer);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_imguiTexture);
    glCreateTextures(GL_TEXTURE_2D, 1, &m_gNormal);
    glCreateTextures(GL_TEXTURE_2D, 1, &m_gAlbedoSpec);
    
    glTextureParameteri(m_imguiTexture  , GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_imguiTexture  , GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(m_gNormal    , GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_gNormal    , GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(m_gAlbedoSpec, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_gAlbedoSpec, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    glTextureStorage2D(m_imguiTexture  , 1, GL_RGBA8, dimension.width,
        dimension.height);
    glNamedFramebufferTexture(m_imguiFramebuffer, GL_COLOR_ATTACHMENT0,
         m_imguiTexture, 0);
    
    glTextureStorage2D(m_gNormal    , 1, GL_RGBA16F, dimension.width,
        dimension.height);
    glTextureStorage2D(m_gAlbedoSpec, 1, GL_RGBA8UI, dimension.width,
        dimension.height);
    
    glNamedFramebufferTexture(m_imguiFramebuffer, GL_COLOR_ATTACHMENT1,
         m_gNormal    , 0);
    glNamedFramebufferTexture(m_imguiFramebuffer, GL_COLOR_ATTACHMENT2,
         m_gAlbedoSpec, 0);

    
    // glCreateTextures(GL_TEXTURE_2D, 1, &m_tFramebuffer);
    // glTextureParameteri(m_tFramebuffer  , GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glTextureParameteri(m_tFramebuffer  , GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTextureStorage2D(m_tFramebuffer, 1, GL_RGBA8, dimension.width, dimension.height);
    
    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
//    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glNamedFramebufferDrawBuffers(m_imguiFramebuffer, 1, attachments);
    // create and attach depth buffer (renderbuffer)
    unsigned int rboDepth;
    glCreateRenderbuffers(1, &rboDepth);
    glNamedRenderbufferStorage(rboDepth, GL_DEPTH_COMPONENT, dimension.width, dimension.height);
    glNamedFramebufferRenderbuffer(m_imguiFramebuffer, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

    // finally check if framebuffer is complete
    int error = 0;
    if ((error = glCheckNamedFramebufferStatus(m_imguiFramebuffer, GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
        __debugbreak();
#endif
    // set to FALSE to disable vsync
    wglSwapIntervalEXT(1);
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
            nullptr, GL_DYNAMIC_STORAGE_BIT);
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

        // This is the matrix we're going to use for instanced models
        glEnableVertexArrayAttrib (model->vao, 3);
        glVertexArrayAttribFormat (model->vao, 3, 4, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribBinding(model->vao, 3, model->vbm);

        glEnableVertexArrayAttrib (model->vao, 4);
        glVertexArrayAttribFormat (model->vao, 4, 4, GL_FLOAT, GL_FALSE,
            (sizeof(float) * 4));
        glVertexArrayAttribBinding(model->vao, 4, model->vbm);

        glEnableVertexArrayAttrib (model->vao, 5);
        glVertexArrayAttribFormat (model->vao, 5, 4, GL_FLOAT, GL_FALSE,
            (2 * sizeof(float) * 4));
        glVertexArrayAttribBinding(model->vao, 5, model->vbm);

        glEnableVertexArrayAttrib (model->vao, 6);
        glVertexArrayAttribFormat (model->vao, 6, 4, GL_FLOAT, GL_FALSE,
            (3 * sizeof(float) * 4));
        glVertexArrayAttribBinding(model->vao, 6, model->vbm);

        glVertexArrayBindingDivisor(model->vao, 3, 1);
        glVertexArrayBindingDivisor(model->vao, 4, 1);
        glVertexArrayBindingDivisor(model->vao, 5, 1);
        glVertexArrayBindingDivisor(model->vao, 6, 1);
    }
    InitTexture(&model->obj);
    DeInitGraphicsObj(&model->obj);
}

GLuint Graphics::InitTexture(void *data,int height, int width)
{
    GLuint texture;
    glCreateTextures(GL_TEXTURE_2D, 1, &texture);

    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER,
        GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTextureStorage2D(texture, 1, GL_RGBA32F, width, height);
    glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA,
         GL_UNSIGNED_BYTE, data);
    return (texture);
}

void Graphics::InitTexture(ObjModel *model)
{
    GLuint texture[128];

    WaitForSingleObject(model->images.thread, INFINITE); // TODO(V. Caraulan): Wrap in waiting for thread function
    for (int i = 0; i < (int)model->images.count; i++)
    {
        texture[i] = InitTexture(model->images.data[i].data,
            model->images.data[i].height, model->images.data[i].width);
    }
    
    for (int i = 0; i < (int)model->materials.count; i++)
    {
        int temp = model->materials.material[i].diffuseMap.index0;
        model->materials.material[i].diffuseMap.index0 = texture[temp];
    }
}

void Graphics::InitShaders(Memory *tempArena)
{
    // fragment & vertex shaders for drawing triangle
    {
        MyString vertexShaderSource = OpenAndReadEntireFile(
            "src\\Shaders\\vertex.vert", tempArena);
        MyString fragmentShaderSource = OpenAndReadEntireFile(
            "src\\Shaders\\fragment.frag", tempArena);

        m_vshader = glCreateShaderProgramv(GL_VERTEX_SHADER, 1,
            &vertexShaderSource.data);
        m_fshader = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1,
            &fragmentShaderSource.data);

        GLint linked;
        glGetProgramiv(m_vshader, GL_LINK_STATUS, &linked);
        if (!linked)
        {
            char message[1024];
            glGetProgramInfoLog(m_vshader, sizeof(message), nullptr, message);
            OutputDebugStringA(message);
            Assert(!"Failed to create vertex shader!");
        }

        glGetProgramiv(m_fshader, GL_LINK_STATUS, &linked);
        if (!linked)
        {
            char message[1024];
            glGetProgramInfoLog(m_fshader, sizeof(message), nullptr, message);
            OutputDebugStringA(message);
            Assert(!"Failed to create fragment shader!");
        }


        glGenProgramPipelines(1, &m_pipeline);
        glUseProgramStages(m_pipeline, GL_VERTEX_SHADER_BIT, m_vshader);
        glUseProgramStages(m_pipeline, GL_FRAGMENT_SHADER_BIT, m_fshader);
    }
}

void Graphics::SetViewProjectionMatrix(RedFoxMaths::Mat4 vp)
{
    m_viewProjection = vp;
}

void Graphics::Draw(GameObject *objects, int gameObjectCount, Memory *temp)
{
    // clear screen
    glBindFramebuffer(GL_FRAMEBUFFER, m_imguiFramebuffer);
    glClearColor(0.392f, 0.584f, 0.929f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // activate shaders for next draw call
    glBindProgramPipeline(m_pipeline);
    GLint u_matrix = 0;
    glProgramUniformMatrix4fv(m_vshader, u_matrix, 1, GL_TRUE, 
        m_viewProjection.AsPtr());

    u_matrix = 1;

    // for (int i = 0; i < gameObjectCount; i++)
    // {
    //     if (objects[i].model)
    //     {
    //         RedFoxMaths::Mat4 modelMatrix = objects[i].GetWorldMatrix();
    //         glProgramUniformMatrix4fv(m_vshader, u_matrix, 1, GL_TRUE, modelMatrix.AsPtr());
    //         DrawModel(objects[i].model);
    //     }
    // }

    
    RedFoxMaths::Mat4 *mem = (RedFoxMaths::Mat4 *)MyMalloc(temp, sizeof(RedFoxMaths::Mat4) * gameObjectCount);
    int batchCount = 768; //TODO figure out a good value for this
    int batchInstancedCount = gameObjectCount / batchCount;
    for(int index = 0;index < batchInstancedCount; index++)
    {
        for (int i = 0; i < batchCount; i++)
            mem[i] = objects[i + (index * batchCount)].GetWorldMatrix().GetTransposedMatrix();
        glNamedBufferSubData(objects->model->vbm,	0,	sizeof(RedFoxMaths::Mat4) * batchCount, mem);
        DrawModelInstances(objects->model, batchCount);
    }
    batchCount = gameObjectCount % batchCount;
    if (batchCount)
    {
        for (int i = 0; i < batchCount; i++)
            mem[i] = objects[(gameObjectCount - batchCount) + i].GetWorldMatrix().GetTransposedMatrix();
        glNamedBufferSubData(objects->model->vbm,	0,	sizeof(RedFoxMaths::Mat4) * batchCount, mem);
        DrawModelInstances(objects->model, batchCount);
    }
}

void Graphics::DrawModel(Model *model)
{
    // provide vertex input
    glBindVertexArray(model->vao);

    GLint diffuseMap = 0;
    glBindTextureUnit(diffuseMap, 0);

    for (int i = 0; i < (int)model->obj.meshCount; i++)
    {
        if (model->obj.materials.count && model->obj.materials.material[model->obj.meshes[i].materialIndex].hasTexture)
            glBindTextureUnit(diffuseMap,
                              model->obj.materials.material[model->obj.meshes[i].materialIndex].diffuseMap.index0);
        else
            glBindTextureUnit(diffuseMap, 0); // TODO: create a default 'missing' texture
       glDrawElements(GL_TRIANGLES, model->obj.meshes[i].indexCount ,GL_UNSIGNED_INT, (void *)(model->obj.meshes[i].indexStart * sizeof(u32)));
    }
}

void Graphics::DrawModelInstances(Model *model, int instanceCount)
{
    // provide vertex input
    glBindVertexArray(model->vao);

    GLint diffuseMap = 0;
    glBindTextureUnit(diffuseMap, 0);

    for (int i = 0; i < (int)model->obj.meshCount; i++)
    {
        if (model->obj.materials.count && model->obj.materials.material[model->obj.meshes[i].materialIndex].hasTexture)
            glBindTextureUnit(diffuseMap,
                              model->obj.materials.material[model->obj.meshes[i].materialIndex].diffuseMap.index0);
        else
            glBindTextureUnit(diffuseMap, 0); // TODO: create a default 'missing' texture
        glDrawElementsInstanced(GL_TRIANGLES, model->obj.meshes[i].indexCount ,GL_UNSIGNED_INT, (void *)((model->obj.meshes[i].indexStart) * sizeof(u32)), instanceCount);
    }
}
} // namespace RedFoxEngine
