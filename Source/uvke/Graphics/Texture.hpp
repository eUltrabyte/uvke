#pragma once
#ifndef UVKE_TEXTURE_HEADER
#define UVKE_TEXTURE_HEADER

#include "../uvke.hpp"
#include "Surface.hpp"
#include "StagingBuffer.hpp"
#include "CommandBuffer.hpp"

namespace uvke {
    class UVKE_API Texture {
    public:
        Texture(Base* base = nullptr, std::string_view filename = "");
        virtual ~Texture();

        virtual void Allocate();
        virtual void LayoutTransition(CommandBuffer* commandBuffer, VkQueue queue, VkImageLayout oldLayout, VkImageLayout newLayout);
        virtual void CopyFromBuffer(CommandBuffer* commandBuffer, VkQueue queue, VkBuffer source);
        virtual void CopyToBuffer(CommandBuffer* commandBuffer, VkQueue queue, VkBuffer destination);

        virtual void SetBase(Base* base);

        virtual vec2u& GetSize();
        virtual int& GetChannel();
        virtual unsigned char* GetPixels();
        virtual VkImage& GetImage();
        virtual VkDeviceMemory& GetImageMemory();
        virtual VkImageView& GetImageView();

    protected:
        Base* m_base;

    private:
        vec2u m_size;
        int m_channel;
        unsigned char* m_pixels;
        VkDeviceSize m_imageSize;
        VkImage m_image;
        VkDeviceMemory m_imageMemory;
        VkImageView m_imageView;

    };
};

#endif