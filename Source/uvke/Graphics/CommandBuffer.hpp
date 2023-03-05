#pragma once
#ifndef UVKE_COMMAND_BUFFER_HEADER
#define UVKE_COMMAND_BUFFER_HEADER

#include "../uvke.hpp"
#include "Surface.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "UniformBuffer.hpp"
#include "Pipeline.hpp"
#include "Framebuffer.hpp"

namespace uvke {
    class UVKE_API CommandBuffer {
    public:
        CommandBuffer(VkDevice device, unsigned int queueFamilyIndex);
        virtual ~CommandBuffer();

        virtual void Record(unsigned int frame, unsigned int index, std::shared_ptr<Surface> surface, std::shared_ptr<Pipeline> pipeline, std::shared_ptr<Framebuffer> framebuffer, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer, std::shared_ptr<UniformBuffer> uniformBuffer);
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