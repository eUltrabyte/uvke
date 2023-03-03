#include "Framebuffer.hpp"

namespace uvke {
    Framebuffer::Framebuffer(VkDevice device, VkRenderPass renderPass, std::shared_ptr<Swapchain> swapchain, std::shared_ptr<Surface> surface)
        : m_device(device), m_renderPass(renderPass), m_swapchain(swapchain), m_surface(surface) {
        m_framebuffers = std::vector<VkFramebuffer>(m_swapchain->GetImageViews().size());

        for(auto i = 0; i < m_framebuffers.size(); ++i) {
            VkFramebufferCreateInfo framebufferCreateInfo { };
            framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferCreateInfo.pNext = nullptr;
            framebufferCreateInfo.flags = 0;
            framebufferCreateInfo.renderPass = m_renderPass;
            framebufferCreateInfo.attachmentCount = 1;
            framebufferCreateInfo.pAttachments = &m_swapchain->GetImageView(i);
            framebufferCreateInfo.width = m_surface->GetExtent().width;
            framebufferCreateInfo.height = m_surface->GetExtent().height;
            framebufferCreateInfo.layers = 1;

            UVKE_ASSERT(vkCreateFramebuffer(m_device, &framebufferCreateInfo, nullptr, &m_framebuffers[i]));
        }

        UVKE_LOG("Framebuffers Created");
    }
    
    Framebuffer::~Framebuffer() {
        if(m_device != VK_NULL_HANDLE) {
            for(auto i = 0; i < m_framebuffers.size(); ++i) {
                if(m_framebuffers[i] != VK_NULL_HANDLE) {
                    vkDestroyFramebuffer(m_device, m_framebuffers[i], nullptr);
                }
            }
        }

        UVKE_LOG("Framebuffers Destroyed");
    }

    void Framebuffer::Recreate() {
        for(auto i = 0; i < m_framebuffers.size(); ++i) {
            vkDestroyFramebuffer(m_device, m_framebuffers[i], nullptr);
        }

        m_framebuffers = std::vector<VkFramebuffer>(m_swapchain->GetImageViews().size());

        for(auto i = 0; i < m_framebuffers.size(); ++i) {
            VkFramebufferCreateInfo framebufferCreateInfo { };
            framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferCreateInfo.pNext = nullptr;
            framebufferCreateInfo.flags = 0;
            framebufferCreateInfo.renderPass = m_renderPass;
            framebufferCreateInfo.attachmentCount = 1;
            framebufferCreateInfo.pAttachments = &m_swapchain->GetImageView(i);
            framebufferCreateInfo.width = m_surface->GetExtent().width;
            framebufferCreateInfo.height = m_surface->GetExtent().height;
            framebufferCreateInfo.layers = 1;

            UVKE_ASSERT(vkCreateFramebuffer(m_device, &framebufferCreateInfo, nullptr, &m_framebuffers[i]));
        }

        UVKE_LOG("Framebuffers Recreated");
    }

    void Framebuffer::SetDevice(VkDevice device) {
        m_device = device;
    }
    
    void Framebuffer::SetRenderPass(VkRenderPass renderPass) {
        m_renderPass = renderPass;
    }
    
    void Framebuffer::SetSwapchain(std::shared_ptr<Swapchain> swapchain) {
        m_swapchain = swapchain;
    }
    
    void Framebuffer::SetSurface(std::shared_ptr<Surface> surface) {
        m_surface = surface;
    }

    VkDevice& Framebuffer::GetDevice() {
        return m_device;
    }
    
    VkRenderPass& Framebuffer::GetRenderPass() {
        return m_renderPass;
    }

    std::shared_ptr<Swapchain>& Framebuffer::GetSwapchain() {
        return m_swapchain;
    }

    std::shared_ptr<Surface>& Framebuffer::GetSurface() {
        return m_surface;
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