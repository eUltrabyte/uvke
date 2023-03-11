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
        Texture(VkPhysicalDevice physicalDevice = nullptr, VkDevice device = nullptr, std::string_view filename = "");
        virtual ~Texture();

        virtual void Allocate();
        virtual void LayoutTransition(std::shared_ptr<CommandBuffer> commandBuffer, VkQueue queue, VkImageLayout oldLayout, VkImageLayout newLayout);
        virtual void CopyFromBuffer(std::shared_ptr<CommandBuffer> commandBuffer, VkQueue queue, VkBuffer source);
        virtual void CopyToBuffer(std::shared_ptr<CommandBuffer> commandBuffer, VkQueue queue, VkBuffer destination);

        virtual void SetDevice(VkDevice device);
        virtual void SetSize(vec2u size);
        virtual void SetChannel(int channel);
        virtual void SetPixels(unsigned char* pixels);
        virtual void SetImage(VkImage image);
        virtual void SetImageMemory(VkDeviceMemory imageMemory);

        virtual VkDevice& GetDevice();
        virtual vec2u& GetSize();
        virtual int& GetChannel();
        virtual unsigned char* GetPixels();
        virtual VkImage& GetImage();
        virtual VkDeviceMemory& GetImageMemory();

    protected:
        VkPhysicalDevice m_physicalDevice;
        VkDevice m_device;

    private:
        vec2u m_size;
        int m_channel;
        unsigned char* m_pixels;
        VkDeviceSize m_imageSize;
        VkImage m_image;
        VkDeviceMemory m_imageMemory;

    };
};

#endif