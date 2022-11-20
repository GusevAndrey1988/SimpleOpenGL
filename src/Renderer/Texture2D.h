#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <string>
#include <memory>
#include <map>

namespace Renderer {
    class Texture2D {
        public:
            struct SubTexture2D {
                glm::vec2 leftBottomUV;
                glm::vec2 rightTopUV;

                SubTexture2D(
                    const glm::vec2 &_leftBottomUV,
                    const glm::vec2 &_rightTopUV
                ):
                    leftBottomUV(_leftBottomUV),
                    rightTopUV(_rightTopUV)
                {}

                SubTexture2D():
                    leftBottomUV(0.f),
                    rightTopUV(1.f)
                {}
            };

            using SubTexture2DMap = std::map<std::string, SubTexture2D>;

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

            void addSubTexture(
                const std::string &name,
                const glm::vec2 &leftBottomUV,
                const glm::vec2 &rightTopUV
            );
            const SubTexture2D& getSubTexture(const std::string &name) const;

            unsigned int getWidth() const { return width; }
            unsigned int getHeight() const { return height; }

            void bind() const;

        private:
            GLuint id;
            unsigned int width;
            unsigned int height;
            GLenum mode;

            SubTexture2DMap subTextureMap;

            void selectMode(unsigned int channels);
    };

    using Texture2DSharedPtr = std::shared_ptr<Texture2D>;
}