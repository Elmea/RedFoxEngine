#include "OpenGLGraphics.hpp"
#include "glcorearb.h"
#include "imgui_impl_opengl3.h"

#define MEMORY_IMPLEMENTATION
#include "MyMemory.hpp"

#define STB_TRUETYPE_IMPLEMENTATION
#include "imstb_truetype.h"

namespace RedFoxEngine
{

    void Graphics::InitGraphics(Memory* tempArena, WindowDimension p_dimension)
    {
        dimension = p_dimension;
        InitShaders(tempArena);
        // setup global GL state
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
        }

        InitImGUIFramebuffer(dimension);
        {
            glCreateBuffers(1, &m_matrixSSBO);
            glNamedBufferStorage(m_matrixSSBO,
                100000 * sizeof(RedFoxMaths::Mat4),
                nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);

            glCreateBuffers(1, &m_materialSSBO);
            glNamedBufferStorage(m_materialSSBO,
                100100 * sizeof(Material),
                nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT | GL_MAP_READ_BIT);

            glCreateBuffers(1, &m_textureSSBO);
            glNamedBufferStorage(m_textureSSBO, 10000 * sizeof(u64), nullptr, GL_DYNAMIC_STORAGE_BIT);

            glCreateBuffers(1, &m_shadowMapsSSBO);
            glNamedBufferStorage(m_shadowMapsSSBO, 1000 * sizeof(u64), nullptr, GL_DYNAMIC_STORAGE_BIT);
        }
        glCreateSamplers(1, &m_textureSampler);
        glSamplerParameteri(m_textureSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glSamplerParameteri(m_textureSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glSamplerParameteri(m_textureSampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glSamplerParameteri(m_textureSampler, GL_TEXTURE_WRAP_T, GL_REPEAT);

        {
            glCreateBuffers(1, &m_vertexBufferObject);
            glNamedBufferStorage(m_vertexBufferObject, 1000000 * (sizeof(ObjVertex)), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
        }
        {
            glCreateBuffers(1, &m_ebo);
            glNamedBufferStorage(m_ebo, 2000000 * sizeof(u32), nullptr, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
        }
        // vertex input
        {
            glCreateVertexArrays(1, &m_vertexArrayObject);

            int vbuf_index = 0;
            glVertexArrayVertexBuffer(m_vertexArrayObject, vbuf_index, m_vertexBufferObject, 0,
                sizeof(ObjVertex));

            glVertexArrayElementBuffer(m_vertexArrayObject, m_ebo);

            int a_pos = 0;
            glEnableVertexArrayAttrib(m_vertexArrayObject, a_pos);
            glVertexArrayAttribFormat(m_vertexArrayObject, a_pos, 3, GL_FLOAT, GL_FALSE,
                offsetof(struct ObjVertex, position));
            glVertexArrayAttribBinding(m_vertexArrayObject, a_pos, vbuf_index);

            int a_normal = 1;
            glEnableVertexArrayAttrib(m_vertexArrayObject, a_normal);
            glVertexArrayAttribFormat(m_vertexArrayObject, a_normal, 3, GL_FLOAT, GL_FALSE, offsetof(struct ObjVertex, normal));
            glVertexArrayAttribBinding(m_vertexArrayObject, a_normal, vbuf_index);

            int a_uv = 2;
            glEnableVertexArrayAttrib(m_vertexArrayObject , a_uv);
            glVertexArrayAttribFormat(m_vertexArrayObject , a_uv, 2, GL_FLOAT, GL_FALSE, offsetof(struct ObjVertex, textureUV));
            glVertexArrayAttribBinding(m_vertexArrayObject, a_uv, vbuf_index);
            int a_materialID = 3;
            glEnableVertexArrayAttrib(m_vertexArrayObject , a_materialID);
            glVertexArrayAttribIFormat(m_vertexArrayObject, a_materialID, 1, GL_UNSIGNED_INT, offsetof(struct ObjVertex, materialID));
            glVertexArrayAttribBinding(m_vertexArrayObject, a_materialID, vbuf_index);
            // glVertexArrayBindingDivisor(model->vao, a_materialID, 1);
        }
        //V-SYNC
        wglSwapIntervalEXT(0);
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
        {
            glCreateBuffers(1, &m_quadVBO);
            glNamedBufferStorage(m_quadVBO, sizeof(vertices), vertices, GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
        }
        // vertex input
        {
            glCreateVertexArrays(1, &m_quadVAO);

            int vbuf_index = 0;
            glVertexArrayVertexBuffer(m_quadVAO, vbuf_index, m_quadVBO, 0,
                4 * sizeof(float));

            int a_pos = 0;
            glEnableVertexArrayAttrib(m_quadVAO, a_pos);
            glVertexArrayAttribFormat(m_quadVAO, a_pos, 2, GL_FLOAT, GL_FALSE, 0);
            glVertexArrayAttribBinding(m_quadVAO, a_pos, vbuf_index);

            int a_uv = 1;
            glEnableVertexArrayAttrib(m_quadVAO, a_uv);
            glVertexArrayAttribFormat(m_quadVAO, a_uv, 2, GL_FLOAT, GL_FALSE,
                sizeof(float) * 2);
            glVertexArrayAttribBinding(m_quadVAO, a_uv, vbuf_index);
        }
    }

    void Graphics::InitImGUIFramebuffer(WindowDimension dimension)
    {
        glCreateFramebuffers(1, &m_imguiFramebuffer);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_imguiTexture);
        glTextureParameteri(m_imguiTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(m_imguiTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureStorage2D(m_imguiTexture, 1, GL_RGBA8, dimension.width,
            dimension.height);
        glNamedFramebufferTexture(m_imguiFramebuffer, GL_COLOR_ATTACHMENT0,
            m_imguiTexture, 0);
        unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
        glNamedFramebufferDrawBuffers(m_imguiFramebuffer, 1, attachments);
        GLuint rboIMGUI;

        glCreateRenderbuffers(1, &rboIMGUI);
        glNamedRenderbufferStorage(rboIMGUI, GL_DEPTH24_STENCIL8,
            dimension.width, dimension.height);
        glNamedFramebufferRenderbuffer(m_imguiFramebuffer, GL_DEPTH_ATTACHMENT,
            GL_RENDERBUFFER, rboIMGUI);

        if (glCheckNamedFramebufferStatus(m_imguiFramebuffer, GL_FRAMEBUFFER) !=
            GL_FRAMEBUFFER_COMPLETE)
            __debugbreak();
    }

    /*If the dimensions of the window change, we need to resize*/
    void Graphics::UpdateImGUIFrameBuffer(WindowDimension& dimension,
        WindowDimension content)
    {
        dimension = content;
        glDeleteTextures(1, &m_imguiTexture);
        glDeleteFramebuffers(1, &m_imguiFramebuffer);
        InitImGUIFramebuffer(dimension);
    }

    void Graphics::InitModel(Model* model)
    {
        // vertex buffer containing triangle vertices
        model->vertexOffset = m_vertexCount;
        model->indexOffset = m_indexCount;
        // model->mesh.count = model->obj.meshCount;
        model->indexCount = model->obj.indexCount;
        InitModelTextures(&model->obj);
        
        Material materials[100] = {};

        int temp = m_materialCount;
        for (int i = 0; i < (int)model->obj.vertexCount; i++)
           model->obj.vertices[i].materialID += m_materialCount;
        for (int i = 0; i < (int)model->obj.materials.count; i++)
        {
            materials[i].diffuse = {model->obj.materials.material[i].diffuse.x, 
                                    model->obj.materials.material[i].diffuse.y,
                                    model->obj.materials.material[i].diffuse.z};
            materials[i].Shininess = model->obj.materials.material[i].Shininess;
            if (model->obj.materials.material[i].hasTexture == 0)
                materials[i].diffuseMap = -1;
            else
                materials[i].diffuseMap = model->obj.materials.material[i].diffuseMap.index0;
            if (model->obj.materials.material[i].hasNormal == 0)
                materials[i].normalMap = -1;
            else
                materials[i].normalMap = model->obj.materials.material[i].normalMap.index0;
            m_materialCount++;
        }

        glNamedBufferSubData(m_materialSSBO, temp * sizeof(Material), (model->obj.materials.count) * sizeof(Material), materials);
        {
            // (GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data);
            glNamedBufferSubData(m_vertexBufferObject, m_vertexCount * sizeof(ObjVertex), 
                model->obj.vertexCount * (sizeof(ObjVertex)), model->obj.vertices);
            m_vertexCount += model->obj.vertexCount;
        }
        {
            glNamedBufferSubData(m_ebo, m_indexCount * sizeof(u32),
                model->obj.indexCount * sizeof(u32), model->obj.indices);
            m_indexCount += model->obj.indexCount;
        }
        DeInitGraphicsObj(&model->obj);
    }

    void Graphics::InitFont(Memory* temp)
    {
        unsigned char* temp_bitmap = (unsigned char*)MyMalloc(temp, 512 * 512);

        MyString file = OpenAndReadEntireFile("Fonts\\VictorMono-Bold.ttf", temp);
        stbtt_BakeFontBitmap((const unsigned char *)file.data, 0, 32.0, temp_bitmap, 512, 512, 32, 96, cdata); // no guarantee this fits!
        glGenTextures(1, &m_gFontTexture);
        glBindTexture(GL_TEXTURE_2D, m_gFontTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 512, 512, 0, GL_RED, GL_UNSIGNED_BYTE, temp_bitmap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    u32 Graphics::InitTexture(void* data, int width, int height, bool resident, bool repeat)
    {
        GLuint texture;
        glCreateTextures(GL_TEXTURE_2D, 1, &texture);
        glTextureStorage2D(texture, 1, GL_RGBA8, width, height);
        glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA,
            GL_UNSIGNED_BYTE, data);
        if (repeat)
        {
            glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }
        if (resident)
        {
            GLuint64 textureHandle = glGetTextureSamplerHandleARB(texture, m_textureSampler);
            glMakeTextureHandleResidentARB(textureHandle);
        }
        return (texture);
    }

    void Graphics::InitModelTextures(ObjModel* model)
    {
        // TODO(V. Caraulan): Wrap in waiting for thread function
        WaitForSingleObject(model->images.thread, INFINITE);

        for (int i = 0; i < (int)model->materials.count; i++)
        {//TODO: if we ever have 1 model draw function, we nedd to get rid of this
            model->materials.material[i].diffuseMap.index0 += m_textures.textureCount;
            model->materials.material[i].normalMap.index0 += m_textures.textureCount;
        }
        for (int i = 0; i < (int)model->images.count; i++)
        {
            if (model->images.data[i].height && model->images.data[i].width)
            {
                m_textures.textures[m_textures.textureCount++] = InitTexture(model->images.data[i].data,
                    model->images.data[i].width, model->images.data[i].height, true, false);
            }
        }
    }

    void Graphics::SetViewProjectionMatrix(RedFoxMaths::Mat4 vp)
    {
        m_viewProjection = vp;
    }

    void Graphics::RenderText(GameUI ui)
    {      
        if (ui.text.data == nullptr)
            return;
        glBindFramebuffer(GL_FRAMEBUFFER, m_imguiFramebuffer);
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBindProgramPipeline(m_font.pipeline);
        glBindVertexArray(m_quadVAO);
        float xOffset = (ui.screenPosition.x / 100) * dimension.width;
        float yOffset = (ui.screenPosition.y / 100) * dimension.height;
        RedFoxMaths::Mat4 mat = RedFoxMaths::Mat4::GetOrthographicMatrix(dimension.width, 0, dimension.height, 0, 1, -1);
        if (*ui.text.data)
        {
            RedFoxMaths::Float4 vertices[4] = 
            {
                // positions  // texture Coords
                {xOffset , yOffset + ui.size.y ,  0.0f, 1.0f},
                {xOffset , yOffset ,  0.0f, 0.0f},
                {xOffset + ui.size.x , yOffset + ui.size.y ,  1.0f, 1.0f},
                {xOffset + ui.size.x , yOffset ,  1.0f, 0.0f},
            };
            glNamedBufferSubData(m_quadVBO, 0, sizeof(vertices), &vertices[0].x);
            if (ui.image)
                glBindTextureUnit(0, ui.image);
            else
                glBindTextureUnit(0, 0);
        
            glProgramUniform1i(m_font.fragment, 1, 0);
            glProgramUniformMatrix4fv(m_font.vertex, 0, 1, GL_TRUE, mat.mat16);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }
        char *text = (char *)ui.text.data;
        // float x, y;
        float offset = 0;
        while(*text)
        {
            offset += cdata[*text - 32].xadvance;
            ++text;
        }
        text = (char *)ui.text.data;
        //offset /= 2;
        float xPos = xOffset;
        float yPos = -yOffset;
        while (*text) {
            if (*text >= 32)
            {
                stbtt_aligned_quad q;
                stbtt_GetBakedQuad(cdata, 512, 512, *text - 32, &xPos, &yPos, &q, 1);
                float vertices[] = {
                     q.x0, -q.y0, q.s0, q.t0,
                     q.x0, -q.y1, q.s0, q.t1,
                     q.x1, -q.y0, q.s1, q.t0,
                     q.x1, -q.y1, q.s1, q.t1,
                };
                // update content of VBO memory
                glNamedBufferSubData(m_quadVBO, 0, sizeof(vertices), vertices);
                glBindTextureUnit(0, m_gFontTexture);
                glProgramUniform3fv(m_font.fragment, 0, 1, &ui.textColor.x);
                glProgramUniform1i(m_font.fragment, 1, 1);
                // render quad
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            }
            ++text;
        }
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Graphics::DrawSkyDome(SkyDome skyDome, float time)
    {
        glBindProgramPipeline(m_sky.pipeline);
        // glBindVertexArray(m_models[1].vao); // TODO:Maybe pass the model as a parameter

        RedFoxMaths::Mat4 mvp = m_viewProjection * skyDome.model;

        glBindTextureUnit(0, skyDome.topTint);
        glBindTextureUnit(1, skyDome.botTint);
        glBindTextureUnit(2, skyDome.sun);
        glBindTextureUnit(3, skyDome.moon);
        glBindTextureUnit(4, skyDome.clouds);
        glProgramUniform3fv(m_sky.vertex, 0, 1, &skyDome.sunPosition.x);
        glProgramUniformMatrix4fv(m_sky.vertex, 1, 1, GL_TRUE, mvp.AsPtr());
        glProgramUniform1f(m_sky.fragment, 0, time);

        Model *sphere = &m_models[1];
        glDrawElementsInstancedBaseVertexBaseInstance(GL_TRIANGLES, sphere->indexCount,
            GL_UNSIGNED_INT, (void *)(sphere->indexOffset * sizeof(u32)), 1, sphere->vertexOffset, 0);
    }

    void Graphics::Draw(Scene *m_scene, WindowDimension p_windowDimension, float p_time, float p_delta)
    {
        dimension = p_windowDimension;
        glBindVertexArray(m_vertexArrayObject);
        //
        glNamedBufferSubData(m_matrixSSBO, 0, sizeof(RedFoxMaths::Mat4) * m_scene->gameObjectCount, m_scene->m_modelMatrices);
        GLuint64 textureHandles[128];
        for (int i = 0; i < (int)m_textures.textureCount; i++)
            textureHandles[i] = glGetTextureSamplerHandleARB(m_textures.textures[i], m_textureSampler);
        glNamedBufferSubData(m_textureSSBO, 0, sizeof(u64) * (m_textures.textureCount), textureHandles);
        //
        static float test;
        if (test > 0.016)
        {
            DrawShadowMaps(m_scene->m_modelCountIndex);
            test = 0;
        }
        test += p_delta;

        glBindFramebuffer(GL_FRAMEBUFFER, m_imguiFramebuffer);
        glCullFace(GL_BACK);
        glViewport(0, 0, p_windowDimension.width,
            p_windowDimension.height);
        glClearColor(0, 0, 0, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glDisable(GL_CULL_FACE);
        DrawSkyDome(m_scene->skyDome, p_time / 500);

        glEnable(GL_CULL_FACE);
        DrawGameObjects(m_scene->m_modelCountIndex);
        // swap the buffers to show output
        glBindFramebuffer(GL_FRAMEBUFFER, 0);    
    }

    static void bindBuffer(int bindingPoint, GLuint buffer, int size)
    {
        if (size)
            glBindBufferRange(GL_SHADER_STORAGE_BUFFER, bindingPoint, buffer, 0, size);
        else
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, 0);    
    }

    void Graphics::DrawGameObjects(u64* modelCountIndex)
    {
        GLint u_matrix = 0; //location inside shader
        glProgramUniformMatrix4fv(m_blinnPhong.vertex, u_matrix, 1, GL_TRUE,
            m_viewProjection.AsPtr());

        glBindProgramPipeline(m_blinnPhong.pipeline);
        glBindVertexArray(m_vertexArrayObject);

        BindLights();
        bindBuffer(4, m_textureSSBO, m_textures.textureCount * sizeof(u64));
        // bindBuffer(5, m_materialSSBO, materialCount * sizeof(Material));
        bindBuffer(6, m_shadowMapsSSBO, lightStorage.lightCount * sizeof(u64));

        int totalCount = 0;
        for (int i = 0; i < (int)m_modelCount; i++)
        {
            if (modelCountIndex[i])
            {
                Model *model = &m_models[i];
                int instanceCount = modelCountIndex[i];
                if (model->obj.materials.material->hasTexture)
                    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 5, m_materialSSBO, 0, m_materialCount * sizeof(Material));
                else
                    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 5, m_materialSSBO, (totalCount + m_materialCount) * sizeof(Material), instanceCount * sizeof(Material));
                glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 3, m_matrixSSBO, totalCount * sizeof(RedFoxMaths::Mat4), instanceCount * sizeof(RedFoxMaths::Mat4));
                // (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex, GLuint baseinstance)
                glDrawElementsInstancedBaseVertexBaseInstance(GL_TRIANGLES, model->indexCount,
                    GL_UNSIGNED_INT, (void *)(model->indexOffset * sizeof(u32)), instanceCount, model->vertexOffset, 0);
            }
            totalCount += modelCountIndex[i];
        }
    }

    void Graphics::DrawShadowMaps(u64* modelCountIndex)
    {
        glCullFace(GL_FRONT);
        for (int lightIndex = 0; lightIndex < lightStorage.lightCount; lightIndex++)
        {
            int totalIndex = 0;
            if (lightStorage.lights[lightIndex].GetType() == LightType::NONE)
                continue;

            glBindFramebuffer(GL_FRAMEBUFFER, lightStorage.lights[lightIndex].depthMapFBO);
            glViewport(0, 0, lightStorage.lights[lightIndex].lightInfo.shadowParameters.SHADOW_WIDTH,
                lightStorage.lights[lightIndex].lightInfo.shadowParameters.SHADOW_HEIGHT);
            glClear(GL_DEPTH_BUFFER_BIT);

            GLint u_matrix = 0;
            glProgramUniformMatrix4fv(m_shadow.vertex, u_matrix, 1, GL_TRUE,
                lightStorage.lights[lightIndex].lightInfo.VP.AsPtr());

            for (int i = 0; i < (int)m_modelCount; i++)
            {
                u64 countIndex = modelCountIndex[i];
                if (countIndex)
                {
                    // DrawModelShadowInstances(&m_models[i], countIndex);
                    glBindProgramPipeline(m_shadow.pipeline);
                    glBindVertexArray(m_vertexArrayObject);
                    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 0, m_matrixSSBO, sizeof(RedFoxMaths::Mat4) * totalIndex, sizeof(RedFoxMaths::Mat4) * countIndex);
                    glDrawElementsInstancedBaseVertexBaseInstance(GL_TRIANGLES, m_models[i].indexCount,
                        GL_UNSIGNED_INT, (void *)(m_models[i].indexOffset * sizeof(u32)), countIndex, m_models[i].vertexOffset, 0);
                    totalIndex += countIndex;
                }
            }
        }
        GLuint64 shadowMapsHandles[128];
        for (int i = 0; i < (int)lightStorage.lightCount; i++)
            shadowMapsHandles[i] = glGetTextureHandleARB(lightStorage.lights[i].lightInfo.shadowParameters.depthMap);
        glNamedBufferSubData(m_shadowMapsSSBO, 0, sizeof(u64) * (lightStorage.lightCount), shadowMapsHandles);
    }

    void Graphics::PushMaterial(Material *materials, int count)
    {
        glNamedBufferSubData(m_materialSSBO, m_materialCount * sizeof(Material), sizeof(Material) * count, materials);
    }

    void Graphics::DrawModelShadowInstances(Model* model, int instanceCount)
    {
        // provide vertex input
        glBindProgramPipeline(m_shadow.pipeline);
        glBindVertexArray(m_vertexArrayObject);
        glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 0, m_matrixSSBO, 0, instanceCount);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_matrixSSBO);
        glDrawElementsInstanced(GL_TRIANGLES, model->obj.indexCount, GL_UNSIGNED_INT,
            0, instanceCount);
    }

} // namespace RedFoxEngine
