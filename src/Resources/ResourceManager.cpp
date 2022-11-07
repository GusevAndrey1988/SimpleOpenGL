#include "./ResourceManager.h"
#include "../Renderer/ShaderProgarm.h"

#include <sstream>
#include <fstream>
#include <iostream>

namespace Resources {
    ResourceManager::ResourceManager(const std::string &executablePath) {
        std::string::size_type foundPosition = executablePath.find_last_of("/\\");
        path = executablePath.substr(0, foundPosition);
    }

    ResourceManager::ShaderProgramSharedPtr ResourceManager::loadShaders(
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
            ResourceManager::ShaderProgramSharedPtr(
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

    ResourceManager::ShaderProgramSharedPtr ResourceManager::getShaderProgram(
        const std::string &shaderName
    ) {
        auto it = shaderProgramMap.find(shaderName);
        if (it != shaderProgramMap.end()) {
            return it->second;
        }

        std::cerr << "Can't find the shader program: " << shaderName << std::endl;
        return nullptr;
    }
}