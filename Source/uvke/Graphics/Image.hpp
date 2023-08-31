#pragma once
#ifndef UVKE_IMAGE_HEADER
#define UVKE_IMAGE_HEADER

#include "../uvke.hpp"
#include "../Core/Base.hpp"
#include "../Core/Helper.hpp"
#include "Surface.hpp"
#include "StagingBuffer.hpp"
#include "CommandBuffer.hpp"

namespace uvke {
    class UVKE_API Image {
    public:
        Image(Base* base = nullptr, const vec2i& size = { 0, 0 });
        virtual ~Image();

        virtual void Allocate();
        virtual void LayoutTransition(CommandBuffer* commandBuffer, VkQueue queue, VkImageLayout oldLayout, VkImageLayout newLayout);
        virtual void CopyFromBuffer(CommandBuffer* commandBuffer, VkQueue queue, VkBuffer source);
        virtual void CopyToBuffer(CommandBuffer* commandBuffer, VkQueue queue, VkBuffer destination);

        virtual void SetBase(Base* base);

        virtual vec2u& GetSize();
        virtual VkImage& GetImage();
        virtual VkDeviceMemory& GetImageMemory();
        virtual VkImageView& GetImageView();

    protected:
        Base* m_base;

    private:
        vec2u m_size;
        VkImage m_image;
        VkDeviceMemory m_imageMemory;
        VkImageView m_imageView;

    };
};

#endif