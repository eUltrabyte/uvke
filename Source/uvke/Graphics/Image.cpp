#include "Image.hpp"

namespace uvke {
    Image::Image(Base* base, const vec2i& size, VkFormat format, VkImageUsageFlags usage, VkSampleCountFlagBits sampleCount)
        : m_base(base) {
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
            imageCreateInfo.format = format;
            imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
            imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            imageCreateInfo.queueFamilyIndexCount = 0;
            imageCreateInfo.pQueueFamilyIndices = nullptr;
            imageCreateInfo.usage = usage;
            imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            imageCreateInfo.samples = sampleCount;

            UVKE_ASSERT(vkCreateImage(m_base->GetDevice(), &imageCreateInfo, nullptr, &m_image));
        }

        UVKE_LOG_ADDRESS("Image Created");
    }
    
    Image::~Image() {
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

        UVKE_LOG_ADDRESS("Image Destroyed");
    }

    void Image::Allocate(VkFormat format, VkImageAspectFlags flags) {
        VkMemoryAllocateInfo memoryAllocateInfo { };
        memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        memoryAllocateInfo.pNext = nullptr;
        memoryAllocateInfo.allocationSize = Helper::GetRequirementsSize(m_base, m_image);
        memoryAllocateInfo.memoryTypeIndex = Helper::FindMemoryIndex(m_base, m_image);

        UVKE_ASSERT(vkAllocateMemory(m_base->GetDevice(), &memoryAllocateInfo, nullptr, &m_imageMemory));

        vkBindImageMemory(m_base->GetDevice(), m_image, m_imageMemory, 0);

        {
            VkImageViewCreateInfo imageViewCreateInfo { };
            imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            imageViewCreateInfo.pNext = nullptr;
            imageViewCreateInfo.flags = 0;
            imageViewCreateInfo.image = m_image;
            imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            imageViewCreateInfo.format = format;
            imageViewCreateInfo.subresourceRange.aspectMask = flags;
            imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
            imageViewCreateInfo.subresourceRange.levelCount = 1;
            imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
            imageViewCreateInfo.subresourceRange.layerCount = 1;

            UVKE_ASSERT(vkCreateImageView(m_base->GetDevice(), &imageViewCreateInfo, nullptr, &m_imageView));
        }

        UVKE_LOG("Image Allocated");
    }

    void Image::LayoutTransition(CommandBuffer* commandBuffer, VkQueue queue, VkImageLayout oldLayout, VkImageLayout newLayout) {
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
            UVKE_ASSERT(-1);
        }

        vkCmdPipelineBarrier(buffer, source, destination, 0, 0, nullptr, 0, nullptr, 1, &imageMemoryBarrier);

        commandBuffer->End(buffer, queue);
    }

    void Image::CopyFromBuffer(CommandBuffer* commandBuffer, VkQueue queue, VkBuffer source) {
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

    void Image::CopyToBuffer(CommandBuffer* commandBuffer, VkQueue queue, VkBuffer destination) {
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

    void Image::SetData(CommandBuffer* commandBuffer, VkQueue queue, const vec2u& size, void* data) {
        m_size = size;
        unsigned int imageSize = m_size.x * m_size.y * 4;
        std::unique_ptr<StagingBuffer> stagingBuffer = std::make_unique<StagingBuffer>(m_base, imageSize);
        VkCommandBuffer buffer = commandBuffer->Begin();

        stagingBuffer->Map(data);

		{
        	VkImageMemoryBarrier imageCopyMemoryBarrier = {};
			imageCopyMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            imageCopyMemoryBarrier.pNext = nullptr;
			imageCopyMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			imageCopyMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			imageCopyMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			imageCopyMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imageCopyMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imageCopyMemoryBarrier.image = m_image;
			imageCopyMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageCopyMemoryBarrier.subresourceRange.baseMipLevel = 0;
			imageCopyMemoryBarrier.subresourceRange.levelCount = 1;
            imageCopyMemoryBarrier.subresourceRange.baseArrayLayer = 0;
			imageCopyMemoryBarrier.subresourceRange.layerCount = 1;

			vkCmdPipelineBarrier(buffer, VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageCopyMemoryBarrier);

			VkBufferImageCopy bufferImageCopy = {};
			bufferImageCopy.bufferOffset = 0;
            bufferImageCopy.bufferRowLength = 0;
            bufferImageCopy.bufferImageHeight = 0;
            bufferImageCopy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            bufferImageCopy.imageSubresource.mipLevel = 0;
            bufferImageCopy.imageSubresource.baseArrayLayer = 0;
            bufferImageCopy.imageSubresource.layerCount = 1;
            bufferImageCopy.imageOffset = { 0, 0, 0 };
            bufferImageCopy.imageExtent = { m_size.x, m_size.y, 1 };

			vkCmdCopyBufferToImage(buffer, stagingBuffer->GetBuffer(), m_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferImageCopy);

			VkImageMemoryBarrier imageUseMemoryBarrier = {};
			imageUseMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            imageUseMemoryBarrier.pNext = nullptr;
			imageUseMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			imageUseMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			imageUseMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			imageUseMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageUseMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imageUseMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imageUseMemoryBarrier.image = m_image;
			imageUseMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageUseMemoryBarrier.subresourceRange.baseMipLevel = 0;
			imageUseMemoryBarrier.subresourceRange.levelCount = 1;
            imageUseMemoryBarrier.subresourceRange.baseArrayLayer = 0;
			imageUseMemoryBarrier.subresourceRange.layerCount = 1;

			vkCmdPipelineBarrier(buffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageUseMemoryBarrier);

			commandBuffer->End(buffer, queue);
		}

        UVKE_LOG("Image Changed Data");
    }
    
    void Image::SetBase(Base* base) {
        m_base = base;
    }
    
    vec2u& Image::GetSize() {
        return m_size;
    }
    
    VkImage& Image::GetImage() {
        return m_image;
    }
    
    VkDeviceMemory& Image::GetImageMemory() {
        return m_imageMemory;
    }

    VkImageView& Image::GetImageView() {
        return m_imageView;
    }
};