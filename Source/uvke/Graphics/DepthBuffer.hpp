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
        DepthBuffer(Base* base = nullptr, Surface* surface = nullptr);
        virtual ~DepthBuffer();

        virtual void LayoutTransition(CommandBuffer* commandBuffer, VkQueue queue, VkImageLayout oldLayout, VkImageLayout newLayout);

        virtual VkImage& GetImage();
        virtual VkDeviceMemory& GetImageMemory();
        virtual VkImageView& GetImageView();

    protected:
        Base* m_base;

    private:
        VkImage m_image;
        VkDeviceMemory m_imageMemory;
        VkImageView m_imageView;

    };
};

#endif