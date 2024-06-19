#pragma once
#ifndef UVKE_SHADER_HEADER
#define UVKE_SHADER_HEADER

#include "../uvke.hpp"
#include "../Core/Base.hpp"

namespace uvke {
    class UVKE_API Shader {
    public:
        Shader(Base* base = nullptr, std::vector<char> vertexCode = std::vector<char>(0), std::vector<char> fragmentCode = std::vector<char>(0));
        Shader(Base* base = nullptr, File vertexFile = File(0), File fragmentFile = File(0));
        virtual ~Shader();

        virtual void SetBase(Base* base);

        virtual VkShaderModule GetVertexShader();
        virtual VkShaderModule GetFragmentShader();
        virtual VkPipelineShaderStageCreateInfo* GetVertexShaderStageCreateInfo();
        virtual VkPipelineShaderStageCreateInfo* GetFragmentShaderStageCreateInfo();

    protected:
        Base* m_base;

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

                UVKE_ASSERT(vkCreateShaderModule(m_base->GetDevice(), &shaderModuleCreateInfo, nullptr, &shaderModule));
            }
            
            return shaderModule;
        }

        VkShaderModule m_vertexShader;
        VkShaderModule m_fragmentShader;
        VkPipelineShaderStageCreateInfo m_vertexShaderStageCreateInfo;
        VkPipelineShaderStageCreateInfo m_fragmentShaderStageCreateInfo;

    };
};

#endif