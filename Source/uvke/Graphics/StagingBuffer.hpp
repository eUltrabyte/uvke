#pragma once
#ifndef UVKE_STAGING_BUFFER_HEADER
#define UVKE_STAGING_BUFFER_HEADER

#include "../uvke.hpp"
#include "../Core/Helper.hpp"

namespace uvke {
    class UVKE_API StagingBuffer {
    public:
        StagingBuffer(Base* base = nullptr, unsigned int size = 0);
        virtual ~StagingBuffer();

        virtual void Map(void* data = nullptr);
        virtual void Copy(VkCommandPool commandPool = nullptr, VkQueue queue = nullptr, VkBuffer destination = nullptr, VkDeviceSize size = 0);

        virtual void SetBase(Base* base);

        virtual VkBuffer& GetBuffer();
        virtual VkDeviceMemory& GetBufferMemory();

    protected:
        Base* m_base;

    private:
        unsigned int m_size;
        VkBuffer m_buffer;
        VkDeviceMemory m_bufferMemory;

    };
};

#endif