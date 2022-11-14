#include "Sprite.h"

namespace Renderer {
    Sprite::Sprite(
        const Texture2DSharedPtr _texture,
        const ShaderProgramSharedPtr _shaderProgram,
        const glm::vec2 &_position,
        const glm::vec2 &_size,
        float _angle
    ):
        texture(_texture),
        shaderProgram(_shaderProgram),
        position(_position),
        size(_size),
        angle(_angle)
    {

    }
   
    Sprite::~Sprite() {

    }

    void Sprite::render() const {

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