#include "Sprite.h"

#include "ShaderProgram.h"
#include "Texture2D.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Renderer {
    Sprite::Sprite(
        const Texture2DSharedPtr _texture,
        const std::string &initialSubTexture,
        const ShaderProgramSharedPtr _shaderProgram,
        const glm::vec2 &_position,
        const glm::vec2 &_size,
        float _angle
    ):
        texture(_texture),
        shaderProgram(_shaderProgram),
        position(_position),
        size(_size),
        angle(_angle),
        vao(0),
        vertexBuffer(0),
        textureCoordBuffer(0)
    {
        createBuffers(initialSubTexture);
    }

    void Sprite::createBuffers(const std::string &subTextureName) {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        GLfloat vertex[] = {
            0.f, 0.f,
            0.f, 1.f,
            1.f, 1.f,

            1.f, 1.f,
            1.f, 0.f,
            0.f, 0.f
        };

        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, nullptr);

        auto subTexture = texture->getSubTexture(subTextureName);

        GLfloat textureCoords[] = {
            subTexture.leftBottomUV.x, subTexture.leftBottomUV.y,
            subTexture.leftBottomUV.x, subTexture.rightTopUV.y,
            subTexture.rightTopUV.x, subTexture.rightTopUV.y,

            subTexture.rightTopUV.x, subTexture.rightTopUV.y,
            subTexture.rightTopUV.x, subTexture.leftBottomUV.y,
            subTexture.leftBottomUV.x, subTexture.leftBottomUV.y,
        };

        glGenBuffers(1, &textureCoordBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, textureCoordBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), textureCoords, GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, nullptr);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
   
    Sprite::~Sprite() {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &textureCoordBuffer);
        glDeleteBuffers(1, &vertexBuffer);
    }

    void Sprite::render() const {
        shaderProgram->use();
        
        glm::mat4 model(1.f);
        model = glm::translate(model, glm::vec3(position, 0.f));
        model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5 * size.y, 0.f));
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0.f, 0.f, 1.f));
        model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5 * size.y, 0.f));
        model = glm::scale(model, glm::vec3(size, 0.f));

        glBindVertexArray(vao);
        shaderProgram->setMatrix4("modelMatrix", model);

        glActiveTexture(GL_TEXTURE0);
        texture->bind();

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void Sprite::setPosition(const glm::vec2 &newPosition) {
        position = newPosition;
    }

    void Sprite::setSize(const glm::vec2 &newSize) {
        size = newSize;
    }

    void Sprite::setAngle(float newAngle) {
        angle = newAngle;
    }
}