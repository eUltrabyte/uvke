#pragma once
#ifndef UVKE_PRESENTATION_HEADER
#define UVKE_PRESENTATION_HEADER

#include "../uvke.hpp"
#include "../Core/Base.hpp"
#include "../Window/Window.hpp"
#include "Surface.hpp"
#include "Swapchain.hpp"
#include "Framebuffer.hpp"
#include "CommandBuffer.hpp"
#include "Pipeline.hpp"
#include "SyncManager.hpp"

namespace uvke {
    class UVKE_API Presentation {
    public:
        Presentation(Base* base = nullptr, Swapchain* swapchain = nullptr, SyncManager* syncManager = nullptr);
        virtual ~Presentation();

        virtual void AcquireNextImage(Window* window, Surface* surface, Pipeline* pipeline, Framebuffer* framebuffer, DepthBuffer* depthBuffer);
        virtual void Submit(CommandBuffer* commandBuffer, Surface* surface);
        virtual void Present(Window* window, Surface* surface, Pipeline* pipeline, Framebuffer* framebuffer, DepthBuffer* depthBuffer);

        virtual unsigned int& GetIndex();
        virtual VkResult& GetResult();
        virtual VkSubmitInfo& GetSubmitInfo();
        virtual VkPresentInfoKHR& GetPresentInfo();

    protected:
        Base* m_base;
        Swapchain* m_swapchain;
        SyncManager* m_syncManager;

    private:
        unsigned int m_index;
        VkResult m_result;
        VkSubmitInfo m_submitInfo;
        VkPresentInfoKHR m_presentInfo;

    };
};

#endif