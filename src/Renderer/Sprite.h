#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>

#include <memory>

namespace Renderer {
    class Texture2D;
    using Texture2DSharedPtr = std::shared_ptr<Texture2D>;

    class ShaderProgram;
    using ShaderProgramSharedPtr = std::shared_ptr<ShaderProgram>;
    
    class Sprite {
        public:
            Sprite(
                const Texture2DSharedPtr texture,
                const ShaderProgramSharedPtr shaderProgram,
                const glm::vec2 &position = glm::vec2(0.f),
                const glm::vec2 &size = glm::vec2(1.f),
                float angle = 0.f
            );
            Sprite(const Sprite&) = delete;
            ~Sprite();

            void render() const;
            void setPosition(const glm::vec2 &newPosition);
            void setSize(const glm::vec2 &newSize);
            void setAngle(float newAngle);

            Sprite& operator=(const Sprite&) = delete;

        private:
            Texture2DSharedPtr texture;
            ShaderProgramSharedPtr shaderProgram;

            glm::vec2 position;
            glm::vec2 size;
            float angle;

            GLuint vao;
            GLuint vertexBuffer;
            GLuint textureCoordBuffer;

            void createBuffers();
    };

    using SpriteSharedPtr = std::shared_ptr<Sprite>;
}