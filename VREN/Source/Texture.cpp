#include "Texture.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace VREN
{
    namespace Texture
    {
#define VALUE_TO_RET(v, r)                                                                         \
    case v:                                                                                        \
    {                                                                                              \
        return r;                                                                                  \
    }                                                                                              \
    break;

        static GLenum WrapToGL(TextureWrap wrap)
        {
            switch (wrap)
            {
                VALUE_TO_RET(TextureWrap::Repeat, GL_REPEAT);
                VALUE_TO_RET(TextureWrap::MirroredRepeat, GL_MIRRORED_REPEAT);
                VALUE_TO_RET(TextureWrap::ClampToBorder, GL_CLAMP_TO_BORDER);
                VALUE_TO_RET(TextureWrap::ClampToEdge, GL_CLAMP_TO_EDGE);
            }
            return GL_REPEAT;
        }

        static GLenum FilterToGL(TextureFilter filter)
        {
            switch (filter)
            {
                VALUE_TO_RET(TextureFilter::Nearest, GL_NEAREST);
                VALUE_TO_RET(TextureFilter::Linear, GL_LINEAR);
            }
            return GL_NEAREST;
        }

        // ManageFilterWrap
        static void MngFltrWrp(TextureWrap w, TextureFilter f)
        {
            GLenum glWrap = WrapToGL(w);
            GLenum glFilter = FilterToGL(f);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glWrap);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glWrap);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glFilter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glFilter);
        }

        TextureHandle CreateTexture2DHandle(int width, int height, u8 *pixels, u32 format,
                                            TextureWrap wrap, TextureFilter filter)
        {
            TextureHandle handle;

            GLenum glFormat = format == 3 ? GL_RGB : GL_RGBA;

            glGenTextures(1, &handle.Id);
            glBindTexture(GL_TEXTURE_2D, handle.Id);

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, glFormat, GL_UNSIGNED_BYTE,
                         pixels);
            glGenerateMipmap(GL_TEXTURE_2D);

            MngFltrWrp(wrap, filter);

            return handle;
        }

        TextureHandle CreateTexture2DHandle(const char *imagePath, TextureWrap wrap,
                                            TextureFilter filter)
        {
            TextureHandle handle;

            int width, height, channels;
            u8 *data = stbi_load(imagePath, &width, &height, &channels, 0);
            if (!data)
            {
                u8 data2[] = {255, 0, 0, 255};
                handle = CreateTexture2DHandle(1, 1, data2);
                return handle;
            }

            handle = CreateTexture2DHandle(width, height, data, channels, wrap, filter);
            return handle;
        }

        void UseHandle(const TextureHandle &handle, int level)
        {
            glActiveTexture(GL_TEXTURE0 + level);
            glBindTexture(GL_TEXTURE_2D, handle.Id);
        }

        void DestroyHandle(const TextureHandle &handle) { glDeleteTextures(1, &handle.Id); }

    } // namespace Texture
} // namespace VREN
