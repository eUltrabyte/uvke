#include "DepthBuffer.hpp"

namespace uvke {
    DepthBuffer::DepthBuffer(Base* base, Surface* surface)
        : m_base(base) {
        m_image = std::make_unique<Image>(m_base, glm::ivec2(surface->GetExtent().width, surface->GetExtent().height), m_base->GetDepthFormat(), VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, base->GetSampleCount());

        m_image->Allocate(m_base->GetDepthFormat(), VK_IMAGE_ASPECT_DEPTH_BIT);

        UVKE_LOG_ADDRESS("Depth Buffer Created");
    }

    DepthBuffer::~DepthBuffer() {
        m_image.reset();

        UVKE_LOG_ADDRESS("Depth Buffer Destroyed");
    }

    void DepthBuffer::Recreate(Base* base, Surface* surface) {
        m_image.reset();

        m_image = std::make_unique<Image>(m_base, glm::ivec2(surface->GetExtent().width, surface->GetExtent().height), m_base->GetDepthFormat(), VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, base->GetSampleCount());

        m_image->Allocate(m_base->GetDepthFormat(), VK_IMAGE_ASPECT_DEPTH_BIT);

        UVKE_LOG("Depth Buffer Recreated");
    }

    void DepthBuffer::LayoutTransition(CommandBuffer* commandBuffer, VkQueue queue) {
        m_image->LayoutTransition(commandBuffer, queue, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
    }

    VkImage& DepthBuffer::GetImage() {
        return m_image->GetImage();
    }
    
    VkDeviceMemory& DepthBuffer::GetImageMemory() {
        return m_image->GetImageMemory();
    }
    
    VkImageView& DepthBuffer::GetImageView() {
        return m_image->GetImageView();
    }
};