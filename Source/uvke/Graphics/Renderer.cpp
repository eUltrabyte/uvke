#include "Renderer.hpp"
#include <memory>

namespace uvke {
    Renderer::Renderer(Base* base, Window* window)
        : m_base(std::make_unique<Base>(*base)), m_window(std::make_unique<Window>(*window)), m_deltaTime(0.0f), m_fps(0) {
        m_surface = std::make_unique<Surface>(m_base.get(), m_window.get());
        
        m_surface->CheckQueues();
        m_surface->SetSwapExtent(m_window.get());

        UVKE_LOG("Format - " + std::to_string(m_surface->GetFormat().format));
        UVKE_LOG("Present Mode - " + std::to_string(m_surface->GetPresentMode()));
        UVKE_LOG("Extent - " + std::to_string(m_surface->GetExtent().width) + "/" + std::to_string(m_surface->GetExtent().height));

        m_swapchain = std::make_unique<Swapchain>(m_base.get(), m_surface.get());

        m_commandBuffer = std::make_unique<CommandBuffer>(m_base.get());

        m_texture = std::make_unique<Texture>(m_base.get(), "Resource/uvke.png");

        m_stagingBuffer = std::make_unique<StagingBuffer>(m_base.get(), static_cast<unsigned int>(m_texture->GetSize().x * m_texture->GetSize().y * 4));
        m_stagingBuffer->Map(m_texture->GetPixels());
        m_texture->Allocate();

        m_texture->LayoutTransition(m_commandBuffer.get(), m_surface->GetQueue(0), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        m_texture->CopyFromBuffer(m_commandBuffer.get(), m_surface->GetQueue(0), m_stagingBuffer->GetBuffer());
        m_texture->LayoutTransition(m_commandBuffer.get(), m_surface->GetQueue(0), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        m_stagingBuffer.reset();

        m_depthBuffer = std::make_unique<DepthBuffer>(m_base.get(), m_surface.get());
        
        m_depthBuffer->LayoutTransition(m_commandBuffer.get(), m_surface->GetQueue(0), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

        m_sampler = std::make_unique<Sampler>(m_base.get(), m_texture.get());
        m_descriptor = std::make_unique<Descriptor>(m_base.get());

        m_vertexBuffer = std::make_unique<VertexBuffer>(m_base.get(), std::vector<Vertex> { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } } } );
        m_indexBuffer = std::make_unique<IndexBuffer>(m_base.get(), std::vector<unsigned int> { 0 } );
        m_uniformBuffer = std::make_unique<UniformBuffer>(m_base.get(), m_sampler.get(), m_descriptor.get());

        m_pipeline = std::make_unique<Pipeline>(m_base.get(), m_surface.get(), m_vertexBuffer.get(), m_descriptor.get());

        m_framebuffer = std::make_unique<Framebuffer>(m_base.get(), m_swapchain.get(), m_surface.get(), m_depthBuffer.get(), m_pipeline->GetRenderPass());

        m_syncManager = std::make_unique<SyncManager>(m_base.get());

        m_interface = std::make_unique<Interface>(m_base.get(), m_window.get(), m_surface.get(), m_commandBuffer.get(), m_pipeline->GetRenderPass());

        m_presentation = std::make_unique<Presentation>(m_base.get(), m_swapchain.get(), m_syncManager.get());

        m_camera = std::make_unique<Camera>();
        // m_camera = std::make_unique<Camera>(Projection::Perspectivic, vec2f(m_window->GetWindowProps()->size.x, m_window->GetWindowProps()->size.y));

        UVKE_LOG("Renderer Created");
    }

    Renderer::~Renderer() {
        m_syncManager->WaitForDevice();

        m_interface.reset();

        m_presentation.reset();

        m_syncManager.reset();

        m_sampler.reset();

        m_depthBuffer.reset();

        m_texture.reset();

        m_commandBuffer.reset();

        m_framebuffer.reset();

        m_pipeline.reset();

        m_renderables.clear();

        m_uniformBuffer.reset();
        m_indexBuffer.reset();
        m_vertexBuffer.reset();

        m_descriptor.reset();

        m_swapchain.reset();
        m_surface.reset();

        UVKE_LOG("Renderer Destroyed");
    }

    void Renderer::Render() {
        for(auto i = 0; i < m_renderables.size(); ++i) {
            m_camera->Move(m_window.get(), vec2f(0.01f, 0.01f));
            m_renderables[i]->Update(m_camera.get());
        }

        m_syncManager->WaitForQueue(m_surface->GetQueue(1));

        m_presentation->AcquireNextImage(m_window.get(), m_pipeline.get(), m_framebuffer.get());

        m_syncManager->WaitForFence(m_syncManager->GetFrame());
        m_syncManager->ResetFence(m_syncManager->GetFrame());

        m_interface->SetRenderTime(m_deltaTime);
        m_pipeline->Render(m_framebuffer.get(), m_commandBuffer.get(), m_syncManager->GetFrame(), m_presentation->GetIndex(), m_renderables, m_interface.get());

        m_presentation->Submit(m_commandBuffer.get(), m_surface.get());
        m_presentation->Present(m_window.get(), m_surface.get(), m_pipeline.get(), m_framebuffer.get());

        if(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - m_clock.GetStart()) >= std::chrono::seconds(1)) {
            m_clock.Restart();
            m_interface->SetFPS(m_fps);
            m_fps = 0;
        }

        m_syncManager->Update();
        m_deltaTime = std::chrono::duration<float, std::milli>(std::chrono::steady_clock::now() - m_frameClock.GetStart()).count();
        m_frameClock.Restart();
        ++m_fps;
    }

    void Renderer::Push(Renderable* renderable) {
        m_renderables.emplace_back(renderable);
    }

    void Renderer::Erase() {
        m_renderables.erase(m_renderables.begin());
    }
    
    void Renderer::SetSurface(Surface* surface) {
        m_surface = std::make_unique<Surface>(*surface);
    }
    
    void Renderer::SetSwapchain(Swapchain* swapchain) {
        m_swapchain = std::make_unique<Swapchain>(*swapchain);
    }
    
    void Renderer::SetStagingBuffer(StagingBuffer* stagingBuffer) {
        m_stagingBuffer = std::make_unique<StagingBuffer>(*stagingBuffer);
    }
    
    void Renderer::SetVertexBuffer(VertexBuffer* vertexBuffer) {
        m_vertexBuffer = std::make_unique<VertexBuffer>(*vertexBuffer);
    }
    
    void Renderer::SetIndexBuffer(IndexBuffer* indexBuffer) {
        m_indexBuffer = std::make_unique<IndexBuffer>(*indexBuffer);
    }
    
    void Renderer::SetUniformBuffer(UniformBuffer* uniformBuffer) {
        m_uniformBuffer = std::make_unique<UniformBuffer>(*uniformBuffer);
    }
    
    void Renderer::SetPipeline(Pipeline* pipeline) {
        m_pipeline = std::make_unique<Pipeline>(*pipeline);
    }
    
    void Renderer::SetFramebuffer(Framebuffer* framebuffer) {
        m_framebuffer = std::make_unique<Framebuffer>(*framebuffer);
    }

    void Renderer::SetCommandBuffer(CommandBuffer* commandBuffer) {
        m_commandBuffer = std::make_unique<CommandBuffer>(*commandBuffer);
    }

    void Renderer::SetTexture(Texture* texture) {
        m_texture = std::make_unique<Texture>(*texture);
    }
    
    void Renderer::SetSampler(Sampler* sampler) {
        m_sampler = std::make_unique<Sampler>(*sampler);
    }
    
    void Renderer::SetSyncManager(SyncManager* syncManager) {
        m_syncManager = std::make_unique<SyncManager>(*syncManager);
    }

    Base* Renderer::GetBase() {
        return m_base.get();
    }
    
    Window* Renderer::GetWindow() {
        return m_window.get();
    }
    
    Surface* Renderer::GetSurface() {
        return m_surface.get();
    }
    
    Swapchain* Renderer::GetSwapchain() {
        return m_swapchain.get();
    }
    
    StagingBuffer* Renderer::GetStagingBuffer() {
        return m_stagingBuffer.get();
    }
    
    VertexBuffer* Renderer::GetVertexBuffer() {
        return m_vertexBuffer.get();
    }
    
    IndexBuffer* Renderer::GetIndexBuffer() {
        return m_indexBuffer.get();
    }

    Descriptor* Renderer::GetDescriptor() {
        return m_descriptor.get();
    }
    
    UniformBuffer* Renderer::GetUniformBuffer() {
        return m_uniformBuffer.get();
    }
    
    Pipeline* Renderer::GetPipeline() {
        return m_pipeline.get();
    }
    
    Framebuffer* Renderer::GetFramebuffer() {
        return m_framebuffer.get();
    }

    CommandBuffer* Renderer::GetCommandBuffer() {
        return m_commandBuffer.get();
    }

    Texture* Renderer::GetTexture() {
        return m_texture.get();
    }
    
    Sampler* Renderer::GetSampler() {
        return m_sampler.get();
    }
    
    SyncManager* Renderer::GetSyncManager() {
        return m_syncManager.get();
    }
};