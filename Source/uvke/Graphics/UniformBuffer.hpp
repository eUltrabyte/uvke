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
        UniformBuffer(VkPhysicalDevice physicalDevice = nullptr, VkDevice device = nullptr);
        virtual ~UniformBuffer();

        virtual void Update(const UniformBufferObject& ubo);

        virtual VkDescriptorSetLayout& GetDescriptorSetLayout();
        virtual VkDescriptorPool& GetDescriptorPool();
        virtual VkDescriptorSet& GetDescriptorSet();
        virtual unsigned int GetSize();
        virtual VkBuffer& GetBuffer();

    protected:
        VkPhysicalDevice m_physicalDevice;
        VkDevice m_device;

    private:
        VkDescriptorSetLayoutBinding m_descriptorSetLayoutBinding;
        VkDescriptorSetLayout m_descriptorSetLayout;
        VkDescriptorPool m_descriptorPool;
        VkDescriptorSet m_descriptorSet;
        VkBuffer m_buffer;
        VkDeviceMemory m_bufferMemory;

    };
};

#endif