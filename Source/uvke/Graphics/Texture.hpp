#pragma once
#ifndef UVKE_TEXTURE_HEADER
#define UVKE_TEXTURE_HEADER

#include "../uvke.hpp"
#include "Surface.hpp"
#include "StagingBuffer.hpp"
#include "CommandBuffer.hpp"
#include "Image.hpp"

namespace uvke {
    class UVKE_API Texture {
    public:
        Texture(Base* base = nullptr, std::string_view filename = "");
        virtual ~Texture();

        virtual void Allocate();
        virtual void LayoutTransition(CommandBuffer* commandBuffer, VkQueue queue, VkImageLayout oldLayout, VkImageLayout newLayout);
        virtual void CopyFromBuffer(CommandBuffer* commandBuffer, VkQueue queue, VkBuffer source);
        virtual void CopyToBuffer(CommandBuffer* commandBuffer, VkQueue queue, VkBuffer destination);

        virtual void SetData(CommandBuffer* commandBuffer, VkQueue queue, const vec2u& size, void* data);
        virtual void SetBase(Base* base);

        virtual vec2u& GetSize();
        virtual int& GetChannel();
        virtual unsigned char* GetPixels();
        virtual Image* GetImage();

    protected:
        Base* m_base;

    private:
        int m_channel;
        unsigned char* m_pixels;
        std::unique_ptr<Image> m_image;

    };
};

#endif