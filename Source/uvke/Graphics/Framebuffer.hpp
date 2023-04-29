#pragma once
#ifndef UVKE_FRAMEBUFFER_HEADER
#define UVKE_FRAMEBUFFER_HEADER

#include "../uvke.hpp"
#include "Surface.hpp"
#include "Swapchain.hpp"

namespace uvke {
    class UVKE_API Framebuffer {
    public:
        Framebuffer(VkDevice device = nullptr, VkRenderPass renderPass = nullptr, std::shared_ptr<Swapchain> swapchain = nullptr, std::shared_ptr<Surface> surface = nullptr);
        virtual ~Framebuffer();

        virtual void Recreate();

        virtual void SetDevice(VkDevice device);
        virtual void SetRenderPass(VkRenderPass renderPass);
        virtual void SetSwapchain(std::shared_ptr<Swapchain> swapchain);
        virtual void SetSurface(std::shared_ptr<Surface> surface);

        virtual VkDevice& GetDevice();
        virtual VkRenderPass& GetRenderPass();
        virtual std::shared_ptr<Surface>& GetSurface();
        virtual std::shared_ptr<Swapchain>& GetSwapchain();
        virtual std::vector<VkFramebuffer>& GetFramebuffers();
        virtual VkFramebuffer& GetFramebuffer(int index);

    protected:
        VkDevice m_device;
        VkRenderPass m_renderPass;
        std::shared_ptr<Surface> m_surface;
        std::shared_ptr<Swapchain> m_swapchain;

    private:
        std::vector<VkFramebuffer> m_framebuffers;

    };
};

#endif