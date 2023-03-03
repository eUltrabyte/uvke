#include "Shader.hpp"

namespace uvke {
    Shader::Shader(VkDevice device, std::vector<char> vertexCode, std::vector<char> fragmentCode)
        : m_device(device) {
        m_vertexShader = CreateShaderModule(vertexCode);
        m_fragmentShader = CreateShaderModule(fragmentCode);

        m_vertexShaderStageCreateInfo = { };
        m_vertexShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        m_vertexShaderStageCreateInfo.pNext = nullptr;
        m_vertexShaderStageCreateInfo.flags = 0;
        m_vertexShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        m_vertexShaderStageCreateInfo.module = m_vertexShader;
        m_vertexShaderStageCreateInfo.pName = "main";
        m_vertexShaderStageCreateInfo.pSpecializationInfo = nullptr;

        m_fragmentShaderStageCreateInfo = { };
        m_fragmentShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        m_fragmentShaderStageCreateInfo.pNext = nullptr;
        m_fragmentShaderStageCreateInfo.flags = 0;
        m_fragmentShaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        m_fragmentShaderStageCreateInfo.module = m_fragmentShader;
        m_fragmentShaderStageCreateInfo.pName = "main";
        m_fragmentShaderStageCreateInfo.pSpecializationInfo = nullptr;
    }

    Shader::Shader(VkDevice device, File vertexFile, File fragmentFile)
        : m_device(device) {
        m_vertexShader = CreateShaderModule(vertexFile.GetData());
        m_fragmentShader = CreateShaderModule(fragmentFile.GetData());

        m_vertexShaderStageCreateInfo = { };
        m_vertexShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        m_vertexShaderStageCreateInfo.pNext = nullptr;
        m_vertexShaderStageCreateInfo.flags = 0;
        m_vertexShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        m_vertexShaderStageCreateInfo.module = m_vertexShader;
        m_vertexShaderStageCreateInfo.pName = "main";
        m_vertexShaderStageCreateInfo.pSpecializationInfo = nullptr;

        m_fragmentShaderStageCreateInfo = { };
        m_fragmentShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        m_fragmentShaderStageCreateInfo.pNext = nullptr;
        m_fragmentShaderStageCreateInfo.flags = 0;
        m_fragmentShaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        m_fragmentShaderStageCreateInfo.module = m_fragmentShader;
        m_fragmentShaderStageCreateInfo.pName = "main";
        m_fragmentShaderStageCreateInfo.pSpecializationInfo = nullptr;

        UVKE_LOG("Shaders Created");
    }

    Shader::~Shader() {
        if(m_device != VK_NULL_HANDLE) {
            if(m_fragmentShader != VK_NULL_HANDLE) {
                vkDestroyShaderModule(m_device, m_fragmentShader, nullptr);
            }

            if(m_vertexShader != VK_NULL_HANDLE) {
                vkDestroyShaderModule(m_device, m_vertexShader, nullptr);
            }
        }

        UVKE_LOG("Shaders Destroyed");
    }

    VkShaderModule Shader::GetVertexShader() {
        return m_vertexShader;
    }

    VkShaderModule Shader::GetFragmentShader() {
        return m_fragmentShader;
    }

    VkPipelineShaderStageCreateInfo* Shader::GetVertexShaderStageCreateInfo() {
        return &m_vertexShaderStageCreateInfo;
    }
    
    VkPipelineShaderStageCreateInfo* Shader::GetFragmentShaderStageCreateInfo() {
        return &m_fragmentShaderStageCreateInfo;
    }
};