#pragma once
#ifndef UVKE_FRAMEBUFFER_HEADER
#define UVKE_FRAMEBUFFER_HEADER

#include "../uvke.hpp"
#include "../Core/Base.hpp"
#include "Surface.hpp"
#include "Swapchain.hpp"

namespace uvke {
    class UVKE_API Framebuffer {
    public:
        Framebuffer(std::shared_ptr<Base> base = nullptr, VkRenderPass renderPass = nullptr, std::shared_ptr<Swapchain> swapchain = nullptr, std::shared_ptr<Surface> surface = nullptr);
        virtual ~Framebuffer();

        virtual void Recreate();

        virtual void SetBase(std::shared_ptr<Base> base);
        virtual void SetSurface(std::shared_ptr<Surface> surface);
        virtual void SetSwapchain(std::shared_ptr<Swapchain> swapchain);
        virtual void SetRenderPass(VkRenderPass renderPass);

        virtual std::shared_ptr<Base> GetBase();
        virtual std::shared_ptr<Surface> GetSurface();
        virtual std::shared_ptr<Swapchain> GetSwapchain();
        virtual VkRenderPass& GetRenderPass();
        virtual std::vector<VkFramebuffer>& GetFramebuffers();
        virtual VkFramebuffer& GetFramebuffer(int index);

    protected:
        std::shared_ptr<Base> m_base;
        std::shared_ptr<Surface> m_surface;
        std::shared_ptr<Swapchain> m_swapchain;
        VkRenderPass m_renderPass;

    private:
        std::vector<VkFramebuffer> m_framebuffers;

    };
};

#endif