#pragma once
#ifndef UVKE_UNIFORM_BUFFER_HEADER
#define UVKE_UNIFORM_BUFFER_HEADER

#include "../uvke.hpp"
#include "../Core/Base.hpp"
#include "../Core/Helper.hpp"
#include "Shader.hpp"
#include "Sampler.hpp"
#include "Descriptor.hpp"

namespace uvke {
    struct UniformBufferObject {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;
    };

    class UVKE_API UniformBuffer {
    public:
        UniformBuffer(Base* base = nullptr, Sampler* sampler = nullptr, Descriptor* descriptor = nullptr, Texture* texture = nullptr);
        virtual ~UniformBuffer();

        virtual void Update(const UniformBufferObject& ubo);
        virtual void Bind(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, unsigned int frame);

        virtual void SetBase(Base* base);

        virtual VkDescriptorPool& GetDescriptorPool();
        virtual std::vector<VkDescriptorSet>& GetDescriptorSets();
        virtual unsigned int GetSize();
        virtual VkBuffer& GetBuffer();

    protected:
        Base* m_base;

    private:
        VkDescriptorPool m_descriptorPool;
        std::vector<VkDescriptorSet> m_descriptorSets;
        VkBuffer m_buffer;
        VkDeviceMemory m_bufferMemory;

    };
};

#endif