#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace uvke {
    Texture::Texture(Base* base, std::string_view filename)
        : m_base(base) {
        vec2i size = { 0, 0 };
        m_pixels = stbi_load(filename.data(), &size.x, &size.y, &m_channel, STBI_rgb_alpha);

        m_size = { static_cast<unsigned int>(size.x), static_cast<unsigned int>(size.y) };

        {
            VkImageCreateInfo imageCreateInfo { };
            imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            imageCreateInfo.pNext = nullptr;
            imageCreateInfo.flags = 0;
            imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
            imageCreateInfo.extent = { m_size.x, m_size.y, 1 };
            imageCreateInfo.mipLevels = 1;
            imageCreateInfo.arrayLayers = 1;
            imageCreateInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
            imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
            imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
            imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;

            UVKE_ASSERT(vkCreateImage(m_base->GetDevice(), &imageCreateInfo, nullptr, &m_image));
        }

        UVKE_LOG("Texture Created");
    }
    
    Texture::~Texture() {
        if(m_base->GetDevice() != VK_NULL_HANDLE) {
            if(m_imageMemory != VK_NULL_HANDLE) {
                vkFreeMemory(m_base->GetDevice(), m_imageMemory, nullptr);
            }

            if(m_image != VK_NULL_HANDLE) {
                vkDestroyImage(m_base->GetDevice(), m_image, nullptr);
            }

            if(m_imageView != VK_NULL_HANDLE) {
                vkDestroyImageView(m_base->GetDevice(), m_imageView, nullptr);
            }
        }

        UVKE_LOG("Texture Destroyed");
    }

    void Texture::Allocate() {
        stbi_image_free(m_pixels);

        VkMemoryRequirements memoryRequirements { };
        vkGetImageMemoryRequirements(m_base->GetDevice(), m_image, &memoryRequirements);

        VkPhysicalDeviceMemoryProperties memoryProperties { };
        vkGetPhysicalDeviceMemoryProperties(m_base->GetPhysicalDevice(), &memoryProperties);

        unsigned int filter = memoryRequirements.memoryTypeBits;
        VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
        unsigned int index = 0;
        for(auto i = 0; i < memoryProperties.memoryTypeCount; ++i) {
            if(filter & (1 << i) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                index = i;
                break;
            }
        }

        VkMemoryAllocateInfo memoryAllocateInfo { };
        memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memoryAllocateInfo.pNext = nullptr;
        memoryAllocateInfo.allocationSize = memoryRequirements.size;
        memoryAllocateInfo.memoryTypeIndex = index;

        UVKE_ASSERT(vkAllocateMemory(m_base->GetDevice(), &memoryAllocateInfo, nullptr, &m_imageMemory));

        vkBindImageMemory(m_base->GetDevice(), m_image, m_imageMemory, 0);

        {
            VkImageViewCreateInfo imageViewCreateInfo { };
            imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            imageViewCreateInfo.pNext = nullptr;
            imageViewCreateInfo.flags = 0;
            imageViewCreateInfo.image = m_image;
            imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            imageViewCreateInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
            imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
            imageViewCreateInfo.subresourceRange.levelCount = 1;
            imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
            imageViewCreateInfo.subresourceRange.layerCount = 1;

            UVKE_ASSERT(vkCreateImageView(m_base->GetDevice(), &imageViewCreateInfo, nullptr, &m_imageView));
        }

        UVKE_LOG("Texture Allocated");
    }

    void Texture::LayoutTransition(CommandBuffer* commandBuffer, VkQueue queue, VkImageLayout oldLayout, VkImageLayout newLayout) {
        VkCommandBuffer buffer = commandBuffer->Begin();

        VkImageMemoryBarrier imageMemoryBarrier { };
        imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        imageMemoryBarrier.pNext = nullptr;
        imageMemoryBarrier.oldLayout = oldLayout;
        imageMemoryBarrier.newLayout = newLayout;
        imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        imageMemoryBarrier.image = m_image;
        imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
        imageMemoryBarrier.subresourceRange.levelCount = 1;
        imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
        imageMemoryBarrier.subresourceRange.layerCount = 1;

        VkPipelineStageFlags source;
        VkPipelineStageFlags destination;

        if(oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            imageMemoryBarrier.srcAccessMask = 0;
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            source = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destination = VK_PIPELINE_STAGE_TRANSFER_BIT;
        } else if(oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            source = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destination = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        } else {
            UVKE_ASSERT(-1);
        }

        vkCmdPipelineBarrier(buffer, source, destination, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);

        commandBuffer->End(buffer, queue);
    }

    void Texture::CopyFromBuffer(CommandBuffer* commandBuffer, VkQueue queue, VkBuffer source) {
        VkCommandBuffer buffer = commandBuffer->Begin();

        VkBufferImageCopy bufferImageCopy { };
        bufferImageCopy.bufferOffset = 0;
        bufferImageCopy.bufferRowLength = 0;
        bufferImageCopy.bufferImageHeight = 0;
        bufferImageCopy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        bufferImageCopy.imageSubresource.mipLevel = 0;
        bufferImageCopy.imageSubresource.baseArrayLayer = 0;
        bufferImageCopy.imageSubresource.layerCount = 1;
        bufferImageCopy.imageOffset = { 0, 0, 0 };
        bufferImageCopy.imageExtent = { m_size.x, m_size.y, 1 };

        vkCmdCopyBufferToImage(buffer, source, m_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferImageCopy);

        commandBuffer->End(buffer, queue);
    }

    void Texture::CopyToBuffer(CommandBuffer* commandBuffer, VkQueue queue, VkBuffer destination) {
        VkCommandBuffer buffer = commandBuffer->Begin();

        VkBufferImageCopy bufferImageCopy { };
        bufferImageCopy.bufferOffset = 0;
        bufferImageCopy.bufferRowLength = 0;
        bufferImageCopy.bufferImageHeight = 0;
        bufferImageCopy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        bufferImageCopy.imageSubresource.mipLevel = 0;
        bufferImageCopy.imageSubresource.baseArrayLayer = 0;
        bufferImageCopy.imageSubresource.layerCount = 1;
        bufferImageCopy.imageOffset = { 0, 0, 0 };
        bufferImageCopy.imageExtent = { m_size.x, m_size.y, 1 };

        vkCmdCopyImageToBuffer(buffer, m_image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, destination, 1, &bufferImageCopy);

        commandBuffer->End(buffer, queue);
    }
    
    void Texture::SetBase(Base* base) {
        m_base = base;
    }
    
    vec2u& Texture::GetSize() {
        return m_size;
    }
    
    int& Texture::GetChannel() {
        return m_channel;
    }
    
    unsigned char* Texture::GetPixels() {
        return m_pixels;
    }
    
    VkImage& Texture::GetImage() {
        return m_image;
    }
    
    VkDeviceMemory& Texture::GetImageMemory() {
        return m_imageMemory;
    }

    VkImageView& Texture::GetImageView() {
        return m_imageView;
    }
};