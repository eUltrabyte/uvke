#pragma once
#ifndef UVKE_UNIFORM_BUFFER_HEADER
#define UVKE_UNIFORM_BUFFER_HEADER

#include "../uvke.hpp"
#include "../Core/Base.hpp"
#include "Shader.hpp"
#include "Sampler.hpp"
#include "Descriptor.hpp"

namespace uvke {
    struct UniformBufferObject {
        mat4x4f model;
        mat4x4f view;
        mat4x4f projection;
    };

    class UVKE_API UniformBuffer {
    public:
        UniformBuffer(std::shared_ptr<Base> base = nullptr, std::shared_ptr<Sampler> sampler = nullptr, std::shared_ptr<Descriptor> descriptor = nullptr);
        virtual ~UniformBuffer();

        virtual void Update(const UniformBufferObject& ubo);
        virtual void Bind(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, unsigned int frame);

        virtual void SetBase(std::shared_ptr<Base> base);

        virtual std::shared_ptr<Base> GetBase();
        virtual VkDescriptorPool& GetDescriptorPool();
        virtual std::vector<VkDescriptorSet>& GetDescriptorSets();
        virtual unsigned int GetSize();
        virtual VkBuffer& GetBuffer();

    protected:
        std::shared_ptr<Base> m_base;

    private:
        VkDescriptorPool m_descriptorPool;
        std::vector<VkDescriptorSet> m_descriptorSets;
        VkBuffer m_buffer;
        VkDeviceMemory m_bufferMemory;

    };
};

#endif