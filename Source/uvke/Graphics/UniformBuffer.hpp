#pragma once
#ifndef UVKE_UNIFORM_BUFFER_HEADER
#define UVKE_UNIFORM_BUFFER_HEADER

#include "../uvke.hpp"
#include "Shader.hpp"

namespace uvke {
    struct UniformBufferObject {
        mat4x4f model;
        mat4x4f view;
        mat4x4f projection;
    };

    class UVKE_API UniformBuffer {
    public:
        UniformBuffer(VkPhysicalDevice physicalDevice = nullptr, VkDevice device = nullptr, VkImageView imageView = nullptr, VkSampler sampler = nullptr);
        virtual ~UniformBuffer();

        virtual void Update(const UniformBufferObject& ubo);
        virtual void Bind(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, unsigned int frame);

        virtual VkDescriptorSetLayout& GetDescriptorSetLayout();
        virtual VkDescriptorPool& GetDescriptorPool();
        virtual std::vector<VkDescriptorSet>& GetDescriptorSets();
        virtual unsigned int GetSize();
        virtual VkBuffer& GetBuffer();

    protected:
        VkPhysicalDevice m_physicalDevice;
        VkDevice m_device;

    private:
        std::array<VkDescriptorSetLayoutBinding, 2> m_descriptorSetLayoutBindings;
        VkDescriptorSetLayout m_descriptorSetLayout;
        VkDescriptorPool m_descriptorPool;
        std::vector<VkDescriptorSet> m_descriptorSets;
        VkBuffer m_buffer;
        VkDeviceMemory m_bufferMemory;

    };
};

#endif