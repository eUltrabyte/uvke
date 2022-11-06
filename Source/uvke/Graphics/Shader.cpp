#include "Shader.hpp"

namespace uvke {
    Shader::Shader(VkDevice device, std::vector<char> vertexCode, std::vector<char> fragmentCode) {
        m_device = device;
        m_vertexShader = CreateShaderModule(vertexCode);
        m_fragmentShader = CreateShaderModule(fragmentCode);
    }

    Shader::Shader(VkDevice device, File vertexFile, File fragmentFile) {
        m_device = device;
        m_vertexShader = CreateShaderModule(vertexFile.GetData());
        m_fragmentShader = CreateShaderModule(fragmentFile.GetData());
    }

    Shader::~Shader() {
        vkDestroyShaderModule(m_device, m_fragmentShader, nullptr);
        vkDestroyShaderModule(m_device, m_vertexShader, nullptr);
    }

    VkShaderModule Shader::GetVertexShader() {
        return m_vertexShader;
    }

    VkShaderModule Shader::GetFragmentShader() {
        return m_fragmentShader;
    }
};