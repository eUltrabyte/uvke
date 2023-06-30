#include "DepthBuffer.hpp"

namespace uvke {
    DepthBuffer::DepthBuffer(Base* base, Surface* surface)
        : m_base(base) {
        VkImageCreateInfo imageCreateInfo { };
        imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageCreateInfo.pNext = nullptr;
        imageCreateInfo.flags = 0;
        imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
        imageCreateInfo.extent = { surface->GetExtent().width, surface->GetExtent().height, 1 };
        imageCreateInfo.mipLevels = 1;
        imageCreateInfo.arrayLayers = 1;
        imageCreateInfo.format = m_base->GetDepthFormat();
        imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageCreateInfo.queueFamilyIndexCount = 0;
        imageCreateInfo.pQueueFamilyIndices = nullptr;
        imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        UVKE_ASSERT(vkCreateImage(m_base->GetDevice(), &imageCreateInfo, nullptr, &m_image));
    
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

        VkImageViewCreateInfo imageViewCreateInfo { };
        imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.pNext = nullptr;
        imageViewCreateInfo.flags = 0;
        imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewCreateInfo.image = m_image;
        imageViewCreateInfo.format = m_base->GetDepthFormat();
        imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
        imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        imageViewCreateInfo.subresourceRange.levelCount = 1;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount = 1;

        UVKE_ASSERT(vkCreateImageView(m_base->GetDevice(), &imageViewCreateInfo, nullptr, &m_imageView));
    }

    DepthBuffer::~DepthBuffer() {
        if(m_base->GetDevice() != VK_NULL_HANDLE) {
            vkDestroyImageView(m_base->GetDevice(), m_imageView, nullptr);
            vkFreeMemory(m_base->GetDevice(), m_imageMemory, nullptr);
            vkDestroyImage(m_base->GetDevice(), m_image, nullptr);
        }
    }

    void DepthBuffer::LayoutTransition(CommandBuffer* commandBuffer, VkQueue queue, VkImageLayout oldLayout, VkImageLayout newLayout) {
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
        } else if(oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
            imageMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

            if(m_base->HasStencilComponent()) {
                imageMemoryBarrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
            }

            imageMemoryBarrier.srcAccessMask = 0;
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

            source = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destination = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        } else {
            UVKE_FATAL("Layout Transition Not Supported");
        }

        vkCmdPipelineBarrier(buffer, source, destination, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);

        commandBuffer->End(buffer, queue);
    }

    VkImage& DepthBuffer::GetImage() {
        return m_image;
    }
    
    VkDeviceMemory& DepthBuffer::GetImageMemory() {
        return m_imageMemory;
    }
    
    VkImageView& DepthBuffer::GetImageView() {
        return m_imageView;
    }
};