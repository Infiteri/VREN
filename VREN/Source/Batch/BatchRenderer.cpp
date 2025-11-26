#include "BatchRenderer.h"
#include "Buffer/VertexArray.h"
#include "Math/Transform.h"
#include "Shader.h"
#include <glad/glad.h>

namespace VREN
{
    void BatchRenderer::Begin() { instanceCount = 0; }

    void BatchRenderer::Submit(const Transform &t, const Color &c)
    {
        if (instanceCount >= MaxInstances)
            return;

        InstanceData &inst = instanceData[instanceCount++];
        inst.model = t.GetMatrix();

        inst.color[0] = c.r / 255.0f;
        inst.color[1] = c.g / 255.0f;
        inst.color[2] = c.b / 255.0f;
        inst.color[3] = c.a / 255.0f;
    }

    void BatchRenderer::End(std::shared_ptr<Camera> cam, std::shared_ptr<Shader> shader)
    {
        if (instanceCount == 0)
            return;

        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, instanceCount * sizeof(InstanceData), instanceData);

        shader->Use();
        shader->Mat4(cam->GetProjection(), "uProj");
        shader->Mat4(Matrix4::Invert(cam->GetView()), "uView");

        geometryVAO->Bind();
        glDrawElementsInstanced(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0, instanceCount);

        instanceCount = 0;
    }

    void BatchRenderer::Shutdown()
    {
        if (instanceVBO)
            glDeleteBuffers(1, &instanceVBO);
        geometryVAO.reset();
    }

    void CubeBatchRenderer::Init()
    {
        float vertices[] = {
            -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f,

            -0.5f, -0.5f, 0.5f,  0.5f, -0.5f, 0.5f,  0.5f, 0.5f, 0.5f,  -0.5f, 0.5f, 0.5f};

        unsigned int indices[] = {0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4, 0, 4, 7, 7, 3, 0,
                                  1, 5, 6, 6, 2, 1, 0, 1, 5, 5, 4, 0, 3, 2, 6, 6, 7, 3};

        geometryVAO = std::make_unique<VertexArray>();
        geometryVAO->Bind();

        geometryVAO->GenerateVertexBuffer(vertices, sizeof(vertices));
        geometryVAO->GenerateIndexBuffer(indices, sizeof(indices));

        geometryVAO->GetVertexBuffer()->AddLayout(0, 0, 3);
        indexCount = sizeof(indices) / sizeof(unsigned int);

        glGenBuffers(1, &instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, MaxInstances * sizeof(InstanceData), nullptr,
                     GL_DYNAMIC_DRAW);

        for (int i = 0; i < 4; i++)
        {
            glEnableVertexAttribArray(1 + i);
            glVertexAttribPointer(1 + i, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData),
                                  (void *)(sizeof(float) * 4 * i));
            glVertexAttribDivisor(1 + i, 1);
        }

        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData),
                              (void *)(sizeof(Matrix4)));
        glVertexAttribDivisor(5, 1);

        glBindVertexArray(0);
    }

    void PlaneBatchRenderer::Init()
    {
        float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f,  -0.5f, 0.0f,
                            0.5f,  0.5f,  0.0f, -0.5f, 0.5f,  0.0f};

        unsigned int indices[] = {0, 1, 2, 2, 3, 0};

        geometryVAO = std::make_unique<VertexArray>();
        geometryVAO->Bind();

        geometryVAO->GenerateVertexBuffer(vertices, sizeof(vertices));
        geometryVAO->GenerateIndexBuffer(indices, sizeof(indices));

        geometryVAO->GetVertexBuffer()->AddLayout(0, 0, 3);
        indexCount = sizeof(indices) / sizeof(unsigned int);

        glGenBuffers(1, &instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, MaxInstances * sizeof(InstanceData), nullptr,
                     GL_DYNAMIC_DRAW);

        for (int i = 0; i < 4; i++)
        {
            glEnableVertexAttribArray(1 + i);
            glVertexAttribPointer(1 + i, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData),
                                  (void *)(sizeof(float) * 4 * i));
            glVertexAttribDivisor(1 + i, 1);
        }

        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData),
                              (void *)sizeof(Matrix4));
        glVertexAttribDivisor(5, 1);

        glBindVertexArray(0);
    }

    void SphereBatchRenderer::Init()
    {
        int rings = 16;
        int segments = 32;

        std::vector<float> vertices;
        std::vector<unsigned> indices;

        for (int y = 0; y <= rings; y++)
        {
            float v = float(y) / rings;
            float phi = v * 3.1415926f;

            for (int x = 0; x <= segments; x++)
            {
                float u = float(x) / segments;
                float theta = u * 6.2831852f;

                float sx = sinf(phi);
                float ex = sx * cosf(theta);
                float ey = cosf(phi);
                float ez = sx * sinf(theta);

                vertices.push_back(ex * 0.5f);
                vertices.push_back(ey * 0.5f);
                vertices.push_back(ez * 0.5f);
            }
        }

        for (int y = 0; y < rings; y++)
        {
            for (int x = 0; x < segments; x++)
            {
                int i0 = y * (segments + 1) + x;
                int i1 = i0 + 1;
                int i2 = i0 + (segments + 1);
                int i3 = i2 + 1;

                indices.push_back(i0);
                indices.push_back(i2);
                indices.push_back(i1);

                indices.push_back(i1);
                indices.push_back(i2);
                indices.push_back(i3);
            }
        }

        geometryVAO = std::make_unique<VertexArray>();
        geometryVAO->Bind();

        geometryVAO->GenerateVertexBuffer(vertices.data(), vertices.size() * sizeof(float));
        geometryVAO->GenerateIndexBuffer(indices.data(), indices.size() * sizeof(unsigned));

        geometryVAO->GetVertexBuffer()->AddLayout(0, 0, 3);
        indexCount = indices.size();

        glGenBuffers(1, &instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, MaxInstances * sizeof(InstanceData), nullptr,
                     GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void *)0);
        glVertexAttribDivisor(1, 1);

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData),
                              (void *)(sizeof(float) * 4));
        glVertexAttribDivisor(2, 1);

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData),
                              (void *)(sizeof(float) * 8));
        glVertexAttribDivisor(3, 1);

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData),
                              (void *)(sizeof(float) * 12));
        glVertexAttribDivisor(4, 1);

        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData),
                              (void *)sizeof(Matrix4));
        glVertexAttribDivisor(5, 1);

        glBindVertexArray(0);
    }

    void CircleBatchRenderer::Init()
    {
        const int segments = 64;

        std::vector<float> vertices;
        std::vector<unsigned> indices;

        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);

        for (int i = 0; i <= segments; i++)
        {
            float t = float(i) / segments * 6.2831852f;
            float x = cosf(t) * 0.5f;
            float y = sinf(t) * 0.5f;

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(0.0f);
        }

        for (int i = 1; i <= segments; i++)
        {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i + 1);
        }

        geometryVAO = std::make_unique<VertexArray>();
        geometryVAO->Bind();

        geometryVAO->GenerateVertexBuffer(vertices.data(), vertices.size() * sizeof(float));
        geometryVAO->GenerateIndexBuffer(indices.data(), indices.size() * sizeof(unsigned));

        geometryVAO->GetVertexBuffer()->AddLayout(0, 0, 3);
        indexCount = indices.size();

        glGenBuffers(1, &instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, MaxInstances * sizeof(InstanceData), nullptr,
                     GL_DYNAMIC_DRAW);

        for (int i = 0; i < 4; i++)
        {
            glEnableVertexAttribArray(1 + i);
            glVertexAttribPointer(1 + i, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData),
                                  (void *)(sizeof(float) * 4 * i));
            glVertexAttribDivisor(1 + i, 1);
        }

        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData),
                              (void *)sizeof(Matrix4));
        glVertexAttribDivisor(5, 1);

        glBindVertexArray(0);
    }
} // namespace VREN
