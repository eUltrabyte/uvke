#include "Framebuffer.hpp"

namespace uvke {
    Framebuffer::Framebuffer(Base* base, Swapchain* swapchain, Surface* surface, DepthBuffer* depthBuffer, VkRenderPass renderPass)
        : m_base(base), m_renderPass(renderPass), m_swapchain(swapchain), m_surface(surface), m_depthBuffer(depthBuffer) {
        m_framebuffers = std::vector<VkFramebuffer>(m_swapchain->GetImageViews().size());

        {
            m_image = std::make_shared<Image>(m_base, vec2i(m_surface->GetExtent().width, m_surface->GetExtent().height), m_surface->GetFormat().format, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, m_base->GetSampleCount());

            m_image->Allocate(m_surface->GetFormat().format);
        }

        for(auto i = 0; i < m_framebuffers.size(); ++i) {
            std::array<VkImageView, 3> imageViews = {
                m_image->GetImageView(),
                m_depthBuffer->GetImageView(),
                m_swapchain->GetImageView(i)
            };

            VkFramebufferCreateInfo framebufferCreateInfo { };
            framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferCreateInfo.pNext = nullptr;
            framebufferCreateInfo.flags = 0;
            framebufferCreateInfo.renderPass = m_renderPass;
            framebufferCreateInfo.attachmentCount = imageViews.size();
            framebufferCreateInfo.pAttachments = imageViews.data();
            framebufferCreateInfo.width = m_surface->GetExtent().width;
            framebufferCreateInfo.height = m_surface->GetExtent().height;
            framebufferCreateInfo.layers = 1;

            UVKE_ASSERT(vkCreateFramebuffer(m_base->GetDevice(), &framebufferCreateInfo, nullptr, &m_framebuffers[i]));
        }

        UVKE_LOG_ADDRESS("Framebuffers Created");
    }
    
    Framebuffer::~Framebuffer() {
        m_image.reset();

        if(m_base->GetDevice() != VK_NULL_HANDLE) {
            for(auto i = 0; i < m_framebuffers.size(); ++i) {
                if(m_framebuffers[i] != VK_NULL_HANDLE) {
                    vkDestroyFramebuffer(m_base->GetDevice(), m_framebuffers[i], nullptr);
                }
            }
        }

        UVKE_LOG_ADDRESS("Framebuffers Destroyed");
    }

    void Framebuffer::Recreate() {
        m_image.reset();

        for(auto i = 0; i < m_framebuffers.size(); ++i) {
            vkDestroyFramebuffer(m_base->GetDevice(), m_framebuffers[i], nullptr);
        }

        m_framebuffers = std::vector<VkFramebuffer>(m_swapchain->GetImageViews().size());

        {
            m_image = std::make_shared<Image>(m_base, vec2i(m_surface->GetExtent().width, m_surface->GetExtent().height), m_surface->GetFormat().format, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, m_base->GetSampleCount());

            m_image->Allocate(m_surface->GetFormat().format);
        }

        for(auto i = 0; i < m_framebuffers.size(); ++i) {
            std::array<VkImageView, 3> imageViews = {
                m_image->GetImageView(),
                m_depthBuffer->GetImageView(),
                m_swapchain->GetImageView(i)
            };

            VkFramebufferCreateInfo framebufferCreateInfo { };
            framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferCreateInfo.pNext = nullptr;
            framebufferCreateInfo.flags = 0;
            framebufferCreateInfo.renderPass = m_renderPass;
            framebufferCreateInfo.attachmentCount = imageViews.size();
            framebufferCreateInfo.pAttachments = imageViews.data();
            framebufferCreateInfo.width = m_surface->GetExtent().width;
            framebufferCreateInfo.height = m_surface->GetExtent().height;
            framebufferCreateInfo.layers = 1;

            UVKE_ASSERT(vkCreateFramebuffer(m_base->GetDevice(), &framebufferCreateInfo, nullptr, &m_framebuffers[i]));
        }

        UVKE_LOG("Framebuffers Recreated");
    }

    void Framebuffer::SetBase(Base* base) {
        m_base = base;
    }
    
    void Framebuffer::SetSurface(Surface* surface) {
        m_surface = surface;
    }
    
    void Framebuffer::SetSwapchain(Swapchain* swapchain) {
        m_swapchain = swapchain;
    }
    
    void Framebuffer::SetRenderPass(VkRenderPass renderPass) {
        m_renderPass = renderPass;
    }
    
    VkRenderPass& Framebuffer::GetRenderPass() {
        return m_renderPass;
    }
    
    std::vector<VkFramebuffer>& Framebuffer::GetFramebuffers() {
        return m_framebuffers;
    }

    VkFramebuffer& Framebuffer::GetFramebuffer(int index) {
        if(index < 0 || index >= m_framebuffers.size()) {
            return m_framebuffers[0];
        } else {
            return m_framebuffers[index];
        }
    }
};