#include "Framebuffer.hpp"

namespace uvke {
    Framebuffer::Framebuffer(std::shared_ptr<Base> base, VkRenderPass renderPass, std::shared_ptr<Swapchain> swapchain, std::shared_ptr<Surface> surface, std::shared_ptr<DepthBuffer> depthBuffer)
        : m_base(base), m_renderPass(renderPass), m_swapchain(swapchain), m_surface(surface), m_depthBuffer(depthBuffer) {
        m_framebuffers = std::vector<VkFramebuffer>(m_swapchain->GetImageViews().size());

        for(auto i = 0; i < m_framebuffers.size(); ++i) {
            std::array<VkImageView, 2> imageViews = {
                m_swapchain->GetImageView(i),
                m_depthBuffer->GetImageView()
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

        UVKE_LOG("Framebuffers Created");
    }
    
    Framebuffer::~Framebuffer() {
        if(m_base->GetDevice() != VK_NULL_HANDLE) {
            for(auto i = 0; i < m_framebuffers.size(); ++i) {
                if(m_framebuffers[i] != VK_NULL_HANDLE) {
                    vkDestroyFramebuffer(m_base->GetDevice(), m_framebuffers[i], nullptr);
                }
            }
        }

        UVKE_LOG("Framebuffers Destroyed");
    }

    void Framebuffer::Recreate() {
        for(auto i = 0; i < m_framebuffers.size(); ++i) {
            vkDestroyFramebuffer(m_base->GetDevice(), m_framebuffers[i], nullptr);
        }

        m_framebuffers = std::vector<VkFramebuffer>(m_swapchain->GetImageViews().size());

        for(auto i = 0; i < m_framebuffers.size(); ++i) {
            std::array<VkImageView, 2> imageViews = {
                m_swapchain->GetImageView(i),
                m_depthBuffer->GetImageView()
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

    void Framebuffer::SetBase(std::shared_ptr<Base> base) {
        m_base = base;
    }
    
    void Framebuffer::SetSurface(std::shared_ptr<Surface> surface) {
        m_surface = surface;
    }
    
    void Framebuffer::SetSwapchain(std::shared_ptr<Swapchain> swapchain) {
        m_swapchain = swapchain;
    }
    
    void Framebuffer::SetRenderPass(VkRenderPass renderPass) {
        m_renderPass = renderPass;
    }

    std::shared_ptr<Base> Framebuffer::GetBase() {
        return m_base;
    }
    
    std::shared_ptr<Surface> Framebuffer::GetSurface() {
        return m_surface;
    }
    
    std::shared_ptr<Swapchain> Framebuffer::GetSwapchain() {
        return m_swapchain;
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