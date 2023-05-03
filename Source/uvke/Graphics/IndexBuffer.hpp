#pragma once
#ifndef UVKE_INDEX_BUFFER_HEADER
#define UVKE_INDEX_BUFFER_HEADER

#include "../uvke.hpp"
#include "../Core/Base.hpp"

namespace uvke {
    class UVKE_API IndexBuffer {
    public:
        IndexBuffer(std::shared_ptr<Base> base = nullptr, std::vector<unsigned int> indices = std::vector<unsigned int>(0));
        virtual ~IndexBuffer();

        virtual void Bind(VkCommandBuffer commandBuffer);

        virtual void SetBase(std::shared_ptr<Base> base);

        virtual std::shared_ptr<Base> GetBase();
        virtual std::vector<unsigned int>& GetIndices();
        virtual unsigned int GetSize();
        virtual VkBuffer& GetBuffer();
        virtual VkDeviceMemory& GetBufferMemory();

    protected:
        std::shared_ptr<Base> m_base;

    private:
        std::vector<unsigned int> m_indices;
        VkBuffer m_buffer;
        VkDeviceMemory m_bufferMemory;

    };
};

#endif