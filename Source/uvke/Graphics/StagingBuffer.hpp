#pragma once
#ifndef UVKE_STAGING_BUFFER_HEADER
#define UVKE_STAGING_BUFFER_HEADER

#include "../uvke.hpp"

namespace uvke {
    class UVKE_API StagingBuffer {
    public:
        StagingBuffer(VkPhysicalDevice physicalDevice = nullptr, VkDevice device = nullptr, unsigned int size = 0);
        virtual ~StagingBuffer();

        virtual void Map(void* data = nullptr);
        virtual void Copy(VkCommandPool commandPool = nullptr, VkQueue queue = nullptr, VkBuffer destination = nullptr, VkDeviceSize size = 0);

        virtual VkBuffer& GetBuffer();

    protected:
        VkPhysicalDevice m_physicalDevice;
        VkDevice m_device;

    private:
        unsigned int m_size;
        VkBuffer m_buffer;
        VkDeviceMemory m_bufferMemory;

    };
};

#endif