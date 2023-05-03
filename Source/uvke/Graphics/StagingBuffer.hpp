#pragma once
#ifndef UVKE_STAGING_BUFFER_HEADER
#define UVKE_STAGING_BUFFER_HEADER

#include "../uvke.hpp"
#include "../Core/Base.hpp"

namespace uvke {
    class UVKE_API StagingBuffer {
    public:
        StagingBuffer(std::shared_ptr<Base> base = nullptr, unsigned int size = 0);
        virtual ~StagingBuffer();

        virtual void Map(void* data = nullptr);
        virtual void Copy(VkCommandPool commandPool = nullptr, VkQueue queue = nullptr, VkBuffer destination = nullptr, VkDeviceSize size = 0);

        virtual void SetBase(std::shared_ptr<Base> base);

        virtual std::shared_ptr<Base> GetBase();
        virtual VkBuffer& GetBuffer();
        virtual VkDeviceMemory& GetBufferMemory();

    protected:
        std::shared_ptr<Base> m_base;

    private:
        unsigned int m_size;
        VkBuffer m_buffer;
        VkDeviceMemory m_bufferMemory;

    };
};

#endif