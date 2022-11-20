#include "./ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"

#include <sstream>
#include <fstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "./stb_image.h"

namespace Resources {
    ResourceManager::ResourceManager(const std::string &executablePath) {
        std::string::size_type foundPosition = executablePath.find_last_of("/\\");
        path = executablePath.substr(0, foundPosition);
    }

    Renderer::ShaderProgramSharedPtr ResourceManager::loadShaders(
        const std::string &shaderName,
        const std::string &vertexShaderPath,
        const std::string &fragmentShaderPath
    ) {
        std::string vertexShaderCode = getFileString(vertexShaderPath);
        if (vertexShaderCode.empty()) {
            std::cerr << "No vertex shader!" << std::endl;
            return nullptr;
        }

        std::string fragmentShaderCode = getFileString(fragmentShaderPath);
        if (fragmentShaderPath.empty()) {
            std::cerr << "No fragment shader!" << std::endl;
            return nullptr;
        }

        auto emplaceResult = shaderProgramMap.emplace(
            shaderName,
            Renderer::ShaderProgramSharedPtr(
                new Renderer::ShaderProgram(
                    vertexShaderCode,
                    fragmentShaderCode
                )
            )
        );

        if (emplaceResult.second && emplaceResult.first->second->isCompiled()) {
            return emplaceResult.first->second;
        }

        std::cerr << "Can't load shader program:\n"
            << "Vertex: " << vertexShaderPath << "\n"
            << "Fragment: " << fragmentShaderPath << std::endl;

        return nullptr;
    }

    std::string ResourceManager::getFileString(const std::string &relativePath) const {
        std::ifstream f;
        f.open(path + "/" + relativePath, std::ios::in | std::ios::binary);
        if (!f.is_open()) {
            std::cerr << "Failed to open file: " << relativePath << std::endl;
            return std::string{};
        }

        std::stringstream buffer;
        buffer << f.rdbuf();
        return buffer.str();
    }

    Renderer::ShaderProgramSharedPtr ResourceManager::getShaderProgram(
        const std::string &shaderName
    ) {
        auto it = shaderProgramMap.find(shaderName);
        if (it != shaderProgramMap.end()) {
            return it->second;
        }

        std::cerr << "Can't find the shader program: " << shaderName << std::endl;
        return nullptr;
    }

    Renderer::Texture2DSharedPtr ResourceManager::loadTexture(
        const std::string &name,
        const std::string &texturePath
    ) {
        int channels = 0;
        int width = 0;
        int height = 0;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *pixels = stbi_load(
            std::string(path + "/" + texturePath).c_str(),
            &width,
            &height,
            &channels,
            0
        );
        
        if (!pixels) {
            std::cerr << "Can't load image: " << texturePath << std::endl;
            return nullptr;
        }

        auto emplaceResult = textureMap.emplace(
            name, Renderer::Texture2DSharedPtr(new Renderer::Texture2D(
                width, height, pixels, channels, GL_NEAREST, GL_CLAMP_TO_EDGE
            ))
        );

        if (!emplaceResult.second) {
            std::cerr << "Can't emplace image" << std::endl;
            return nullptr;
        }

        stbi_image_free(pixels);

        return emplaceResult.first->second;
    }

    Renderer::Texture2DSharedPtr ResourceManager::getTexture(const std::string &name) {
        auto textureIt = textureMap.find(name);
        if (textureIt == textureMap.end()) {
            std::cerr << "Can't find the texture: " << name << std::endl;
            return nullptr;
        }

        return textureIt->second;
    }

    Renderer::SpriteSharedPtr ResourceManager::loadSprite(
        const std::string &name,
        const std::string &textureName,
        const std::string &shaderName,
        unsigned int width,
        unsigned int height,
        const std::string &subTextureName
    ) {
        auto texture = getTexture(textureName);
        if (!texture) {
            std::cerr << "Can't find the texture: " << textureName
                << " for the sprite: " << name << std::endl;
            return nullptr;
        }

        auto shaderProgram = getShaderProgram(shaderName);
        if (!shaderProgram) {
            std::cerr << "Can't find the shader program: " << shaderName 
                << " for the sprite: " << name << std::endl;
            return nullptr;
        }
    
        auto emplaceResult = spriteMap.emplace(
            name, Renderer::SpriteSharedPtr(new Renderer::Sprite(
                texture,
                subTextureName,
                shaderProgram,
                glm::vec2(0.f),
                glm::vec2(width, height)
            ))
        );

        if (!emplaceResult.second) {
            std::cerr << "Can't emplace sprite" << std::endl;
            return nullptr;
        }

        return emplaceResult.first->second;
    }

    Renderer::SpriteSharedPtr ResourceManager::getSprite(const std::string &name) {
        auto spriteIt = spriteMap.find(name);
        if (spriteIt == spriteMap.end()) {
            std::cerr << "Can't find the sprite: " << name << std::endl;
            return nullptr;
        }

        return spriteIt->second;
    }

    Renderer::Texture2DSharedPtr ResourceManager::loadTextureAtlas(
        const std::string &name,
        const std::string &texturePath,
        const std::vector<std::string> &subTextures,
        const unsigned int subTextureWidth,
        const unsigned int subTextureHeight
    ) {
        auto texture = loadTexture(name, texturePath);
        if (!texture) {
            return nullptr;
        }

        const unsigned int textureWidth = texture->getWidth();
        const unsigned int textureHeight = texture->getHeight();
        unsigned int currentTextureOffsetX = 0;
        unsigned int currentTextureOffsetY = textureHeight;

        for (const auto &currentSubTextureName : subTextures) {
            glm::vec2 leftBottomUV(
                static_cast<float>(currentTextureOffsetX) / textureWidth,
                static_cast<float>(currentTextureOffsetY - subTextureHeight) / textureHeight
            );
            glm::vec2 rightTopUV(
                static_cast<float>(currentTextureOffsetX + subTextureWidth) / textureWidth,
                static_cast<float>(currentTextureOffsetY) / textureHeight
            );

            texture->addSubTexture(currentSubTextureName, leftBottomUV, rightTopUV);

            currentTextureOffsetX += subTextureWidth;
            if (currentTextureOffsetX >= textureWidth) {
                currentTextureOffsetX = 0;
                currentTextureOffsetY -= subTextureHeight;
            }
        }
    }
}
