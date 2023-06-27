#pragma once
#ifndef UVKE_DEPTH_BUFFER_HEADER
#define UVKE_DEPTH_BUFFER_HEADER

#include "../uvke.hpp"
#include "../Core/Base.hpp"
#include "Surface.hpp"
#include "CommandBuffer.hpp"

namespace uvke {
    class UVKE_API DepthBuffer {
    public:
        DepthBuffer(std::shared_ptr<Base> base = nullptr, std::shared_ptr<Surface> surface = nullptr);
        virtual ~DepthBuffer();

        virtual void LayoutTransition(std::shared_ptr<CommandBuffer> commandBuffer, VkQueue queue, VkImageLayout oldLayout, VkImageLayout newLayout);

        virtual VkImage& GetImage();
        virtual VkDeviceMemory& GetImageMemory();
        virtual VkImageView& GetImageView();

    protected:
        std::shared_ptr<Base> m_base;

    private:
        VkImage m_image;
        VkDeviceMemory m_imageMemory;
        VkImageView m_imageView;

    };
};

#endif