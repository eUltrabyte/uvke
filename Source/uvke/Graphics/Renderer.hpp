#pragma once
#ifndef UVKE_RENDERER_HEADER
#define UVKE_RENDERER_HEADER

#include "../uvke.hpp"
#include "../Core/Base.hpp"
#include "../Core/Window.hpp"
#include "Surface.hpp"
#include "Swapchain.hpp"
#include "StagingBuffer.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "Descriptor.hpp"
#include "UniformBuffer.hpp"
#include "Pipeline.hpp"
#include "Framebuffer.hpp"
#include "CommandBuffer.hpp"
#include "Texture.hpp"
#include "Sampler.hpp"
#include "SyncManager.hpp"
#include "Interface.hpp"
#include "Camera.hpp"
#include "Renderable.hpp"

namespace uvke {
    class UVKE_API Renderer {
    public:
        Renderer(std::shared_ptr<Base> base, std::shared_ptr<Window> window);
        virtual ~Renderer();

        virtual void Render();
        virtual void Push(std::shared_ptr<Renderable> renderable);
        virtual void Erase();

        virtual void SetBase(std::shared_ptr<Base> base);
        virtual void SetWindow(std::shared_ptr<Window> window);
        virtual void SetSurface(std::shared_ptr<Surface> surface);
        virtual void SetSwapchain(std::shared_ptr<Swapchain> swapchain);
        virtual void SetStagingBuffer(std::shared_ptr<StagingBuffer> stagingBuffer);
        virtual void SetVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer);
        virtual void SetIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer);
        virtual void SetUniformBuffer(std::shared_ptr<UniformBuffer> uniformBuffer);
        virtual void SetPipeline(std::shared_ptr<Pipeline> pipeline);
        virtual void SetFramebuffer(std::shared_ptr<Framebuffer> framebuffer);
        virtual void SetCommandBuffer(std::shared_ptr<CommandBuffer> commandBuffer);
        virtual void SetTexture(std::shared_ptr<Texture> texture);
        virtual void SetSampler(std::shared_ptr<Sampler> sampler);
        virtual void SetSyncManager(std::shared_ptr<SyncManager> syncManager);

        virtual std::shared_ptr<Base> GetBase();
        virtual std::shared_ptr<Window> GetWindow();
        virtual std::shared_ptr<Surface> GetSurface();
        virtual std::shared_ptr<Swapchain> GetSwapchain();
        virtual std::shared_ptr<StagingBuffer> GetStagingBuffer();
        virtual std::shared_ptr<VertexBuffer> GetVertexBuffer();
        virtual std::shared_ptr<IndexBuffer> GetIndexBuffer();
        virtual std::shared_ptr<Descriptor> GetDescriptor();
        virtual std::shared_ptr<UniformBuffer> GetUniformBuffer();
        virtual std::shared_ptr<Pipeline> GetPipeline();
        virtual std::shared_ptr<Framebuffer> GetFramebuffer();
        virtual std::shared_ptr<CommandBuffer> GetCommandBuffer();
        virtual std::shared_ptr<Texture> GetTexture();
        virtual std::shared_ptr<Sampler> GetSampler();
        virtual std::shared_ptr<SyncManager> GetSyncManager();

    private:
        std::shared_ptr<Base> m_base;
        std::shared_ptr<Window> m_window;
        std::shared_ptr<Surface> m_surface;
        std::shared_ptr<Swapchain> m_swapchain;
        std::shared_ptr<StagingBuffer> m_stagingBuffer;
        std::shared_ptr<VertexBuffer> m_vertexBuffer;
        std::shared_ptr<IndexBuffer> m_indexBuffer;
        std::shared_ptr<Descriptor> m_descriptor;
        std::shared_ptr<UniformBuffer> m_uniformBuffer;
        std::vector<std::shared_ptr<Renderable>> m_renderables;
        std::shared_ptr<Pipeline> m_pipeline;
        std::shared_ptr<Framebuffer> m_framebuffer;
        std::shared_ptr<CommandBuffer> m_commandBuffer;
        std::shared_ptr<Texture> m_texture;
        std::shared_ptr<Sampler> m_sampler;
        std::shared_ptr<SyncManager> m_syncManager;
        std::shared_ptr<Interface> m_interface;
        std::shared_ptr<Camera> m_camera;
        Clock m_clock;
        Clock m_frameClock;
        int m_fps;

    };
};

#endif