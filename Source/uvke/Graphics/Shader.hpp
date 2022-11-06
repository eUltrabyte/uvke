#pragma once
#ifndef UVKE_SHADER_HEADER
#define UVKE_SHADER_HEADER

#include "../uvke.hpp"

namespace uvke {
    class UVKE_API Shader {
    public:
        Shader(VkDevice device, std::vector<char> vertexCode, std::vector<char> fragmentCode);
        Shader(VkDevice device, File vertexFile, File fragmentFile);
        virtual ~Shader();

        virtual VkShaderModule GetVertexShader();
        virtual VkShaderModule GetFragmentShader();

    private:
        VkShaderModule CreateShaderModule(std::vector<char> code) {
            VkShaderModule shaderModule;

            {
                VkShaderModuleCreateInfo shaderModuleCreateInfo { };
                shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
                shaderModuleCreateInfo.pNext = nullptr;
                shaderModuleCreateInfo.flags = 0;
                shaderModuleCreateInfo.codeSize = code.size();
                shaderModuleCreateInfo.pCode = reinterpret_cast<const unsigned int*>(code.data());

                UVKE_ASSERT(vkCreateShaderModule(m_device, &shaderModuleCreateInfo, nullptr, &shaderModule));
            }
            
            return shaderModule;
        }

        VkDevice m_device;
        VkShaderModule m_vertexShader;
        VkShaderModule m_fragmentShader;

    };
};

#endif