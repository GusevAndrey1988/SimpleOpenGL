#pragma once

#include <glad/glad.h>
#include <string>

#include <memory>

namespace Renderer {
    class Texture2D {
        public:
            Texture2D(
                GLuint width,
                GLuint height,
                const unsigned char *data,
                unsigned int channels = 4,
                const GLenum &filter = GL_LINEAR,
                const GLenum &wrapMode = GL_CLAMP_TO_EDGE
            );
            Texture2D(Texture2D &&other);
            Texture2D() = delete;
            Texture2D(const Texture2D&) = delete;
            ~Texture2D();

            Texture2D& operator=(Texture2D &&other);
            Texture2D& operator=(const Texture2D&) = delete;

            void bind() const;

        private:
            GLuint id;
            GLuint width;
            GLuint height;
            GLenum mode;

            void selectMode(unsigned int channels);
    };

    using Texture2DSharedPtr = std::shared_ptr<Texture2D>;
}