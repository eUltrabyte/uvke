#pragma once
#ifndef UVKE_COMMAND_BUFFER_HEADER
#define UVKE_COMMAND_BUFFER_HEADER

#include "../uvke.hpp"

namespace uvke {
    class UVKE_API CommandBuffer {
    public:
        CommandBuffer(VkDevice device, unsigned int queueFamilyIndex);
        virtual ~CommandBuffer();

        virtual VkCommandBuffer Begin();
        virtual void End(VkCommandBuffer commandBuffer, VkQueue queue);

        virtual void SetDevice(VkDevice device);
        virtual void SetCommandPool(VkCommandPool commandPool);
        virtual void SetCommandBuffers(std::vector<VkCommandBuffer> commandBuffers);

        virtual VkDevice& GetDevice();
        virtual VkCommandPool& GetCommandPool();
        virtual std::vector<VkCommandBuffer>& GetCommandBuffers();
        virtual VkCommandBuffer& GetCommandBuffer(unsigned int index);

    protected:
        VkDevice m_device;

    private:
        VkCommandPool m_commandPool;
        std::vector<VkCommandBuffer> m_commandBuffers;

    };
};

#endif