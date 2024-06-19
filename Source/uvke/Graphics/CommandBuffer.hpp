#pragma once
#ifndef UVKE_COMMAND_BUFFER_HEADER
#define UVKE_COMMAND_BUFFER_HEADER

#include "../uvke.hpp"
#include "../Core/Base.hpp"

namespace uvke {
    class UVKE_API CommandBuffer {
    public:
        CommandBuffer(Base* base = nullptr);
        virtual ~CommandBuffer();

        virtual VkCommandBuffer Begin();
        virtual void End(VkCommandBuffer commandBuffer, VkQueue queue);

        virtual void SetBase(Base* base);
        virtual void SetCommandPool(VkCommandPool commandPool);
        virtual void SetCommandBuffers(std::vector<VkCommandBuffer> commandBuffers);

        virtual VkCommandPool& GetCommandPool();
        virtual std::vector<VkCommandBuffer>& GetCommandBuffers();
        virtual VkCommandBuffer& GetCommandBuffer(unsigned int index);

    protected:
        Base* m_base;

    private:
        VkCommandPool m_commandPool;
        std::vector<VkCommandBuffer> m_commandBuffers;

    };
};

#endif