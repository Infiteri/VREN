#pragma once

#include "Base.h"
#include "Buffer/Buffer.h"
#include <memory>

namespace VREN
{
    class VREN_API VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

        void Bind();
        void Unbind();

        inline std::shared_ptr<Buffer> GetVertexBuffer() { return vertexBuffer; };
        inline std::shared_ptr<Buffer> GetIndexBuffer() { return indexBuffer; };

        void GenerateVertexBuffer(void *data, u64 size);
        void GenerateIndexBuffer(void *data, u64 size);

        inline u32 GetID() const { return id; };

    private:
        u32 id;
        std::shared_ptr<Buffer> vertexBuffer;
        std::shared_ptr<Buffer> indexBuffer;
    };
} // namespace Core