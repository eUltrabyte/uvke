#pragma once
#ifndef UVKE_RENDERER_HEADER
#define UVKE_RENDERER_HEADER

#include "../uvke.hpp"
#include "../Core/Base.hpp"
#include "../Window/Window.hpp"
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
#include "DepthBuffer.hpp"
#include "Sampler.hpp"
#include "SyncManager.hpp"
#include "Interface.hpp"
#include "Presentation.hpp"
#include "Camera.hpp"
#include "Component.hpp"
#include "RenderableComponent.hpp"
#include "../Utils/MeshLoader.hpp"

namespace uvke {
    class UVKE_API Renderer {
    public:
        Renderer(Base* base, Window* window);
        virtual ~Renderer();

        virtual void Update();
        virtual void Render();
        virtual void Push(Component* component);
        virtual void Erase();

        virtual void SetSurface(Surface* surface);
        virtual void SetSwapchain(Swapchain* swapchain);
        virtual void SetSampler(Sampler* sampler);
        virtual void SetStagingBuffer(StagingBuffer* stagingBuffer);
        virtual void SetVertexBuffer(VertexBuffer* vertexBuffer);
        virtual void SetIndexBuffer(IndexBuffer* indexBuffer);
        virtual void SetUniformBuffer(UniformBuffer* uniformBuffer);
        virtual void SetPipeline(Pipeline* pipeline);
        virtual void SetFramebuffer(Framebuffer* framebuffer);
        virtual void SetCommandBuffer(CommandBuffer* commandBuffer);
        virtual void SetSyncManager(SyncManager* syncManager);

        virtual Base* GetBase();
        virtual Window* GetWindow();
        virtual Surface* GetSurface();
        virtual Swapchain* GetSwapchain();
        virtual Sampler* GetSampler();
        virtual StagingBuffer* GetStagingBuffer();
        virtual VertexBuffer* GetVertexBuffer();
        virtual IndexBuffer* GetIndexBuffer();
        virtual Descriptor* GetDescriptor();
        virtual UniformBuffer* GetUniformBuffer();
        virtual Pipeline* GetPipeline();
        virtual Framebuffer* GetFramebuffer();
        virtual CommandBuffer* GetCommandBuffer();
        virtual SyncManager* GetSyncManager();

    private:
        std::unique_ptr<Base> m_base;
        std::unique_ptr<Window> m_window;
        std::unique_ptr<Surface> m_surface;
        std::unique_ptr<Swapchain> m_swapchain;
        std::unique_ptr<Sampler> m_sampler;
        std::unique_ptr<StagingBuffer> m_stagingBuffer;
        std::unique_ptr<VertexBuffer> m_vertexBuffer;
        std::unique_ptr<IndexBuffer> m_indexBuffer;
        std::unique_ptr<Descriptor> m_descriptor;
        std::unique_ptr<UniformBuffer> m_uniformBuffer;
        std::vector<Component*> m_components;
        std::unique_ptr<Pipeline> m_pipeline;
        std::unique_ptr<Framebuffer> m_framebuffer;
        std::unique_ptr<CommandBuffer> m_commandBuffer;
        std::unique_ptr<DepthBuffer> m_depthBuffer;
        std::unique_ptr<SyncManager> m_syncManager;
        std::unique_ptr<Interface> m_interface;
        std::unique_ptr<Presentation> m_presentation;
        std::unique_ptr<Camera> m_camera;
        Clock m_clock;
        Clock m_frameClock;
        Clock m_deltaClock;
        Stats m_stats;
        unsigned int m_fps;

    };
};

#endif