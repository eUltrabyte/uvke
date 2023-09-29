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
        Image(Base* base = nullptr, const vec2i& size = { 0, 0 }, VkFormat format = VK_FORMAT_R8G8B8A8_SRGB, VkImageUsageFlags usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_1_BIT);
        virtual ~Image();

        virtual void Allocate(VkFormat format = VK_FORMAT_R8G8B8A8_SRGB, VkImageAspectFlags flags = VK_IMAGE_ASPECT_COLOR_BIT);
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