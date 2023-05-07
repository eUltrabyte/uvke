#pragma once
#ifndef UVKE_PRESENTATION_HEADER
#define UVKE_PRESENTATION_HEADER

#include "../uvke.hpp"
#include "../Core/Base.hpp"
#include "../Core/Window.hpp"
#include "Surface.hpp"
#include "Swapchain.hpp"
#include "Framebuffer.hpp"
#include "CommandBuffer.hpp"
#include "Pipeline.hpp"
#include "SyncManager.hpp"

namespace uvke {
    class UVKE_API Presentation {
    public:
        Presentation(std::shared_ptr<Base> base = nullptr, std::shared_ptr<Swapchain> swapchain = nullptr, std::shared_ptr<SyncManager> syncManager = nullptr);
        virtual ~Presentation() = default;

        virtual void AcquireNextImage(std::shared_ptr<Window> window, std::shared_ptr<Pipeline> pipeline, std::shared_ptr<Framebuffer> framebuffer);
        virtual void Submit(std::shared_ptr<CommandBuffer> commandBuffer, std::shared_ptr<Surface> surface);
        virtual void Present(std::shared_ptr<Window> window, std::shared_ptr<Surface> surface, std::shared_ptr<Pipeline> pipeline, std::shared_ptr<Framebuffer> framebuffer);

        virtual unsigned int& GetIndex();
        virtual VkResult& GetResult();
        virtual VkSubmitInfo& GetSubmitInfo();
        virtual VkPresentInfoKHR& GetPresentInfo();

    protected:
        std::shared_ptr<Base> m_base;
        std::shared_ptr<Swapchain> m_swapchain;
        std::shared_ptr<SyncManager> m_syncManager;

    private:
        unsigned int m_index;
        VkResult m_result;
        VkSubmitInfo m_submitInfo;
        VkPresentInfoKHR m_presentInfo;

    };
};

#endif