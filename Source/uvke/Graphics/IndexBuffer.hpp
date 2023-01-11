#pragma once
#ifndef UVKE_INDEX_BUFFER_HEADER
#define UVKE_INDEX_BUFFER_HEADER

#include "../uvke.hpp"

namespace uvke {
    class UVKE_API IndexBuffer {
    public:
        IndexBuffer(VkPhysicalDevice physicalDevice = nullptr, VkDevice device = nullptr, std::vector<unsigned int> indices = std::vector<unsigned int>(0));
        virtual ~IndexBuffer();

        virtual void Bind(VkCommandBuffer commandBuffer);

        virtual std::vector<unsigned int>& GetIndices();
        virtual unsigned int GetSize();
        virtual VkBuffer& GetBuffer();

    private:
        VkPhysicalDevice m_physicalDevice;
        VkDevice m_device;
        std::vector<unsigned int> m_indices;
        VkBuffer m_buffer;
        VkDeviceMemory m_bufferMemory;

    };
};

#endif