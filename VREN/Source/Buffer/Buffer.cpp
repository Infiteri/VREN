#include "Buffer.h"
#include "Core/Logger.h"

#include <glad/glad.h>

namespace VREN
{
    static GLenum BT_ToGL(BufferType type)
    {
        return (type == BufferType::Vertex) ? GL_ARRAY_BUFFER : GL_ELEMENT_ARRAY_BUFFER;
    }

    static GLenum BDT_ToGL(BufferDataType dt)
    {
        switch (dt)
        {
        default:
        {
            VREN_LOG(Warn, "Unknown data type in BDT_ToGL");
            return GL_STATIC_DRAW;
        }
        break;

        case BufferDataType::Dynamic:
            return GL_DYNAMIC_DRAW;
            break;

        case BufferDataType::Static:
            return GL_STATIC_DRAW;
            break;
        }
    }

    Buffer::Buffer(BufferType bufferType) : type(bufferType), id(0), stride(0), size(0)
    {
        glGenBuffers(1, &id);
        Bind();
    }

    Buffer::~Buffer()
    {
        if (id)
            glDeleteBuffers(1, &id);
    }

    void Buffer::Bind()
    {
        glBindBuffer(BT_ToGL(type), id);
        BindLayout();
    }

    void Buffer::Unbind() { glBindBuffer(BT_ToGL(type), 0); }

    void Buffer::Draw()
    {
        Bind();

        // todo: Let user decide draw type?

        switch (type)
        {

        case BufferType::Vertex:
            glDrawArrays(GL_TRIANGLES, 0,
                         size / stride); // todo :STRIDE INSTEAD OF 6 I NEED BUYFFER LAYOUT FIRST
            break;

        case BufferType::Index:
            glDrawElements(GL_TRIANGLE_STRIP, size / sizeof(u32), GL_UNSIGNED_INT, 0);
            break;
        }
    }

    void Buffer::BufferData(void *data, u64 size, BufferDataType dataType)
    {
        GLenum type = BDT_ToGL(dataType);
        this->size = size;

        Bind();
        glBufferData(BT_ToGL(this->type), size, data, type);
    }

    void Buffer::AddLayout(int location, int offset, int size)
    {
        if (type == BufferType::Index)
            return;

        layout.push_back({.Loc = location, .Off = offset, .Size = size});
        stride = 0;
        for (auto &lay : layout)
            stride += lay.Size;
        BindLayout();
    }

    void Buffer::BindLayout()
    {
        if (type == BufferType::Vertex)
        {
            for (auto &lay : layout)
            {
                glVertexAttribPointer(lay.Loc, lay.Size, GL_FLOAT, GL_FALSE, stride * sizeof(float),
                                      (void *)(lay.Off * sizeof(float)));
                glEnableVertexAttribArray(lay.Loc);
            }
        }
    }
} // namespace Core