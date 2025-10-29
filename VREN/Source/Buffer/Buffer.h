#pragma once

#include "Base.h"
#include <vector>

namespace VREN
{
    enum class BufferType : u8
    {
        Vertex = 0,
        Index = 1
    };

    enum class BufferDataType : u8
    {
        Static,
        Dynamic
    };

    class VREN_API Buffer
    {
    public:
        Buffer(BufferType bufferType);
        ~Buffer();

        void Bind();
        void Unbind();

        void Draw();
        void BufferData(void *data, u64 size, BufferDataType dataType = BufferDataType::Static);

        void AddLayout(int location, int offset, int size);
        void BindLayout();

        inline u32 GetID() { return id; };
        inline BufferType GetType() { return type; };

    private:
        struct Layout
        {
            int Loc, Off, Size;
        };

    private:
        BufferType type;
        u32 id, stride, size;
        std::vector<Layout> layout;
    };

} // namespace Core