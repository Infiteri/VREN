#include "VertexArray.h"

#include <glad/glad.h>
#include <memory>

namespace VREN
{
    VertexArray::VertexArray()
    {
        glGenVertexArrays(1, &id);
        Bind();
    }

    VertexArray::~VertexArray() { glDeleteVertexArrays(1, &id); }

    void VertexArray::Bind() { glBindVertexArray(id); }

    void VertexArray::Unbind() { glBindVertexArray(0); }

    void VertexArray::GenerateVertexBuffer(void *data, u64 size)
    {
        vertexBuffer = std::make_shared<Buffer>(BufferType::Vertex);
        vertexBuffer->BufferData(data, size);
    }

    void VertexArray::GenerateIndexBuffer(void *data, u64 size)
    {
        indexBuffer = std::make_shared<Buffer>(BufferType::Index);
        indexBuffer->BufferData(data, size);
    }

} // namespace Core