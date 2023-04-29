#include "Swapchain.hpp"

namespace uvke {
    Swapchain::Swapchain(std::shared_ptr<Base> base, std::shared_ptr<Surface> surface)
        : m_base(base), m_surface(surface), m_isRecreated(false) {
        m_imageCount = m_surface->GetCapabilities().minImageCount + 1;
        if(m_surface->GetCapabilities().maxImageCount > 0 && m_imageCount > m_surface->GetCapabilities().maxImageCount) {
            m_imageCount = m_surface->GetCapabilities().maxImageCount;
        }

        {
            VkSwapchainCreateInfoKHR swapchainCreateInfo { };
            swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            swapchainCreateInfo.pNext = nullptr;
            swapchainCreateInfo.flags = 0;
            swapchainCreateInfo.surface = m_surface->GetSurface();
            swapchainCreateInfo.minImageCount = m_imageCount;
            swapchainCreateInfo.imageFormat = m_surface->GetFormat().format;
            swapchainCreateInfo.imageColorSpace = m_surface->GetFormat().colorSpace;
            swapchainCreateInfo.imageExtent = m_surface->GetExtent();
            swapchainCreateInfo.imageArrayLayers = 1;
            swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

            if(m_surface->IsMultiQueueMode()) {
                unsigned int indices[2] = { m_surface->GetQueueFamily(), m_surface->GetQueueFamily() + 1 };
                swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                swapchainCreateInfo.queueFamilyIndexCount = 2;
                swapchainCreateInfo.pQueueFamilyIndices = indices;
            } else {
                swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
                swapchainCreateInfo.queueFamilyIndexCount = 0;
                swapchainCreateInfo.pQueueFamilyIndices = nullptr;
            }

            swapchainCreateInfo.preTransform = m_surface->GetCapabilities().currentTransform;
            swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            swapchainCreateInfo.presentMode = m_surface->GetPresentMode();
            swapchainCreateInfo.clipped = VK_TRUE;
            swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

            UVKE_ASSERT(vkCreateSwapchainKHR(m_base->GetDevice(), &swapchainCreateInfo, nullptr, &m_swapchain));
        }

        {
            unsigned int swapchainImageCount = 0;
            vkGetSwapchainImagesKHR(m_base->GetDevice(), m_swapchain, &swapchainImageCount, nullptr);
            m_images = std::vector<VkImage>(swapchainImageCount);
            vkGetSwapchainImagesKHR(m_base->GetDevice(), m_swapchain, &swapchainImageCount, m_images.data());
        }

        {
            m_imageViews = std::vector<VkImageView>(m_images.size());

            for(auto i = 0; i < m_imageViews.size(); ++i) {
                VkImageViewCreateInfo imageViewCreateInfo { };
                imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                imageViewCreateInfo.pNext = nullptr;
                imageViewCreateInfo.flags = 0;
                imageViewCreateInfo.image = m_images[i];
                imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
                imageViewCreateInfo.format = surface->GetFormat().format;
                imageViewCreateInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
                imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
                imageViewCreateInfo.subresourceRange.levelCount = 1;
                imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
                imageViewCreateInfo.subresourceRange.layerCount = 1;

                UVKE_ASSERT(vkCreateImageView(m_base->GetDevice(), &imageViewCreateInfo, nullptr, &m_imageViews[i]));
            }
        }

        UVKE_LOG("Swapchain Created");
    }
    
    Swapchain::~Swapchain() {
        if(m_base->GetDevice() != VK_NULL_HANDLE) {
            if(m_imageViews.data() != nullptr) {
                for(auto i = 0; i < m_imageViews.size(); ++i) {
                    vkDestroyImageView(m_base->GetDevice(), m_imageViews[i], nullptr);
                }
            }

            m_imageViews.clear();
            m_images.clear();

            if(m_swapchain != VK_NULL_HANDLE) {
                vkDestroySwapchainKHR(m_base->GetDevice(), m_swapchain, nullptr);
            }
        }

        UVKE_LOG("Swapchain Destroyed");
    }

    void Swapchain::Recreate(std::shared_ptr<Window> window, VkRenderPass renderPass, std::vector<VkFramebuffer> framebuffers) {
        m_isRecreated = false;
        window->Update();

        vec2i size(window->GetWindowProps()->size.x, window->GetWindowProps()->size.y);
        for(; size.x == 0 || size.y == 0 ;) {
            window->Update();
            size = vec2i(window->GetWindowProps()->size.x, window->GetWindowProps()->size.y);
            m_surface->SetSwapExtent(window);
            window->Wait();
        }

        if(size.x != 0 || size.y != 0) {
            m_surface->SetSwapExtent(window);
        }

        vkDeviceWaitIdle(m_base->GetDevice());

        for(auto i = 0; i < framebuffers.size(); ++i) {
            vkDestroyFramebuffer(m_base->GetDevice(), framebuffers[i], nullptr);
        }

        for(auto i = 0; i < m_imageViews.size(); ++i) {
            vkDestroyImageView(m_base->GetDevice(), m_imageViews[i], nullptr);
        }

        m_imageCount = m_surface->GetCapabilities().minImageCount + 1;
        if(m_surface->GetCapabilities().maxImageCount > 0 && m_imageCount > m_surface->GetCapabilities().maxImageCount) {
            m_imageCount = m_surface->GetCapabilities().maxImageCount;
        }

        {
            VkSwapchainCreateInfoKHR swapchainCreateInfo { };
            swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            swapchainCreateInfo.pNext = nullptr;
            swapchainCreateInfo.flags = 0;
            swapchainCreateInfo.surface = m_surface->GetSurface();
            swapchainCreateInfo.minImageCount = m_imageCount;
            swapchainCreateInfo.imageFormat = m_surface->GetFormat().format;
            swapchainCreateInfo.imageColorSpace = m_surface->GetFormat().colorSpace;
            swapchainCreateInfo.imageExtent = m_surface->GetExtent();
            swapchainCreateInfo.imageArrayLayers = 1;
            swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

            if(m_surface->IsMultiQueueMode()) {
                unsigned int indices[2] = { m_surface->GetQueueFamily(), m_surface->GetQueueFamily() + 1 };
                swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                swapchainCreateInfo.queueFamilyIndexCount = 2;
                swapchainCreateInfo.pQueueFamilyIndices = indices;
            } else {
                swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
                swapchainCreateInfo.queueFamilyIndexCount = 0;
                swapchainCreateInfo.pQueueFamilyIndices = nullptr;
            }

            swapchainCreateInfo.preTransform = m_surface->GetCapabilities().currentTransform;
            swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            swapchainCreateInfo.presentMode = m_surface->GetPresentMode();
            swapchainCreateInfo.clipped = VK_TRUE;
            swapchainCreateInfo.oldSwapchain = m_swapchain;

            UVKE_ASSERT(vkCreateSwapchainKHR(m_base->GetDevice(), &swapchainCreateInfo, nullptr, &m_swapchain));
        }

        {
            unsigned int swapchainImageCount = 0;
            vkGetSwapchainImagesKHR(m_base->GetDevice(), m_swapchain, &swapchainImageCount, nullptr);
            m_images = std::vector<VkImage>(swapchainImageCount);
            vkGetSwapchainImagesKHR(m_base->GetDevice(), m_swapchain, &swapchainImageCount, m_images.data());
        }

        {
            m_imageViews = std::vector<VkImageView>(m_images.size());

            for(auto i = 0; i < m_imageViews.size(); ++i) {
                VkImageViewCreateInfo imageViewCreateInfo { };
                imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
                imageViewCreateInfo.pNext = nullptr;
                imageViewCreateInfo.flags = 0;
                imageViewCreateInfo.image = m_images[i];
                imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
                imageViewCreateInfo.format = m_surface->GetFormat().format;
                imageViewCreateInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
                imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
                imageViewCreateInfo.subresourceRange.levelCount = 1;
                imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
                imageViewCreateInfo.subresourceRange.layerCount = 1;

                UVKE_ASSERT(vkCreateImageView(m_base->GetDevice(), &imageViewCreateInfo, nullptr, &m_imageViews[i]));
            }
        }

        {
            framebuffers = std::vector<VkFramebuffer>(m_imageViews.size());

            for(auto i = 0; i < framebuffers.size(); ++i) {
                VkFramebufferCreateInfo framebufferCreateInfo { };
                framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
                framebufferCreateInfo.pNext = nullptr;
                framebufferCreateInfo.flags = 0;
                framebufferCreateInfo.renderPass = renderPass;
                framebufferCreateInfo.attachmentCount = 1;
                framebufferCreateInfo.pAttachments = &m_imageViews[i];
                framebufferCreateInfo.width = m_surface->GetExtent().width;
                framebufferCreateInfo.height = m_surface->GetExtent().height;
                framebufferCreateInfo.layers = 1;

                UVKE_ASSERT(vkCreateFramebuffer(m_base->GetDevice(), &framebufferCreateInfo, nullptr, &framebuffers[i]));
            }
        }

        UVKE_LOG("Swapchain Recreated Successfully");
    }

    void Swapchain::SetBase(std::shared_ptr<Base> base) {
        m_base = base;
    }
    
    void Swapchain::SetSurface(std::shared_ptr<Surface> surface) {
        m_surface = surface;
    }
    
    void Swapchain::SetImageCount(unsigned int count) {
        m_imageCount = count;
    }
    
    void Swapchain::SetImages(const std::vector<VkImage>& images) {
        m_images = images;
    }
    
    void Swapchain::SetImageViews(const std::vector<VkImageView>& imageViews) {
        m_imageViews = imageViews;
    }
    
    std::shared_ptr<Base> Swapchain::GetBase() {
        return m_base;
    }

    std::shared_ptr<Surface> Swapchain::GetSurface() {
        return m_surface;
    }
    
    unsigned int& Swapchain::GetImageCount() {
        return m_imageCount;
    }
    
    VkSwapchainKHR& Swapchain::GetSwapchain() {
        return m_swapchain;
    }
    
    std::vector<VkImage>& Swapchain::GetImages() {
        return m_images;
    }

    VkImage& Swapchain::GetImage(unsigned int index) {
        if(index < 0 || index >= m_images.size()) {
            return m_images[0];
        } else {
            return m_images[index];
        }
    }
    
    std::vector<VkImageView>& Swapchain::GetImageViews() {
        return m_imageViews;
    }

    VkImageView& Swapchain::GetImageView(unsigned int index) {
        if(index < 0 || index >= m_imageViews.size()) {
            return m_imageViews[0];
        } else {
            return m_imageViews[index];
        }
    }

    bool& Swapchain::IsRecreated() {
        return m_isRecreated;
    }
};