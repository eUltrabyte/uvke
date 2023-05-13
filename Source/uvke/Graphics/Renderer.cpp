#include "Renderer.hpp"

namespace uvke {
    Renderer::Renderer(std::shared_ptr<Base> base, std::shared_ptr<Window> window)
        : m_base(base), m_window(window), m_fps(0) {
        m_surface = std::make_shared<Surface>(m_base, m_window);
        
        m_surface->CheckQueues();
        m_surface->SetSwapExtent(m_window);

        UVKE_LOG("Format - " + std::to_string(m_surface->GetFormat().format));
        UVKE_LOG("Present Mode - " + std::to_string(m_surface->GetPresentMode()));
        UVKE_LOG("Extent - " + std::to_string(m_surface->GetExtent().width) + "/" + std::to_string(m_surface->GetExtent().height));

        m_swapchain = std::make_shared<Swapchain>(m_base, m_surface);

        m_commandBuffer = std::make_shared<CommandBuffer>(m_base);

        m_texture = std::make_shared<Texture>(m_base, "Resource/uvke.png");

        m_stagingBuffer = std::make_shared<StagingBuffer>(m_base, static_cast<unsigned int>(m_texture->GetSize().x * m_texture->GetSize().y * 4));
        m_stagingBuffer->Map(m_texture->GetPixels());
        m_texture->Allocate();

        m_texture->LayoutTransition(m_commandBuffer, m_surface->GetQueue(0), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        m_texture->CopyFromBuffer(m_commandBuffer, m_surface->GetQueue(0), m_stagingBuffer->GetBuffer());
        m_texture->LayoutTransition(m_commandBuffer, m_surface->GetQueue(0), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        m_stagingBuffer.reset();

        m_sampler = std::make_shared<Sampler>(m_base, m_texture);
        m_descriptor = std::make_shared<Descriptor>(m_base);

        m_vertexBuffer = std::make_shared<VertexBuffer>(m_base, std::vector<Vertex> { { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } } } );
        m_indexBuffer = std::make_shared<IndexBuffer>(m_base, std::vector<unsigned int> { 0 } );
        m_uniformBuffer = std::make_shared<UniformBuffer>(m_base, m_sampler, m_descriptor);

        m_pipeline = std::make_shared<Pipeline>(m_base, m_surface, m_vertexBuffer, m_descriptor);

        m_framebuffer = std::make_shared<Framebuffer>(m_base, m_pipeline->GetRenderPass(), m_swapchain, m_surface);

        m_syncManager = std::make_shared<SyncManager>(m_base);

        m_interface = std::make_shared<Interface>(m_base, m_window, m_surface, m_commandBuffer, m_pipeline->GetRenderPass());
        m_interface->SetFPS(0);

        m_presentation = std::make_shared<Presentation>(m_base, m_swapchain, m_syncManager);

        m_camera = std::make_shared<Camera>();
        // m_camera = std::make_shared<Camera>(Projection::Perspectivic, vec2f(m_window->GetWindowProps()->size.x, m_window->GetWindowProps()->size.y));

        UVKE_LOG("Renderer Created");
    }

    Renderer::~Renderer() {
        m_syncManager->WaitForDevice();

        m_interface.reset();

        m_presentation.reset();

        m_syncManager.reset();

        m_sampler.reset();

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
            m_camera->Move(m_window, vec2f(0.01f, 0.01f));
            m_renderables[i]->Update(m_camera);
        }

        m_syncManager->WaitForQueue(m_surface->GetQueue(1));

        m_presentation->AcquireNextImage(m_window, m_pipeline, m_framebuffer);

        m_syncManager->WaitForFence(m_syncManager->GetFrame());
        m_syncManager->ResetFence(m_syncManager->GetFrame());

        m_interface->SetRenderTime(std::chrono::duration<float, std::milli>(std::chrono::steady_clock::now() - m_frameClock.GetStart()).count());
        m_pipeline->Render(m_framebuffer, m_commandBuffer, m_syncManager->GetFrame(), m_presentation->GetIndex(), m_renderables, m_interface);

        m_presentation->Submit(m_commandBuffer, m_surface);
        m_presentation->Present(m_window, m_surface, m_pipeline, m_framebuffer);

        if(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - m_clock.GetStart()) >= std::chrono::seconds(1)) {
            m_clock.Restart();
            m_interface->SetFPS(m_fps);
            m_fps = 0;
        }

        m_syncManager->Update();
        m_frameClock.Restart();
        ++m_fps;
    }

    void Renderer::Push(std::shared_ptr<Renderable> renderable) {
        m_renderables.emplace_back(renderable);
    }

    void Renderer::Erase() {
        m_renderables.erase(m_renderables.begin());
    }
    
    void Renderer::SetSurface(std::shared_ptr<Surface> surface) {
        m_surface = surface;
    }
    
    void Renderer::SetSwapchain(std::shared_ptr<Swapchain> swapchain) {
        m_swapchain = swapchain;
    }
    
    void Renderer::SetStagingBuffer(std::shared_ptr<StagingBuffer> stagingBuffer) {
        m_stagingBuffer = stagingBuffer;
    }
    
    void Renderer::SetVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer) {
        m_vertexBuffer = vertexBuffer;
    }
    
    void Renderer::SetIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer) {
        m_indexBuffer = indexBuffer;
    }
    
    void Renderer::SetUniformBuffer(std::shared_ptr<UniformBuffer> uniformBuffer) {
        m_uniformBuffer = uniformBuffer;
    }
    
    void Renderer::SetPipeline(std::shared_ptr<Pipeline> pipeline) {
        m_pipeline = pipeline;
    }
    
    void Renderer::SetFramebuffer(std::shared_ptr<Framebuffer> framebuffer) {
        m_framebuffer = framebuffer;
    }

    void Renderer::SetCommandBuffer(std::shared_ptr<CommandBuffer> commandBuffer) {
        m_commandBuffer = commandBuffer;
    }

    void Renderer::SetTexture(std::shared_ptr<Texture> texture) {
        m_texture = texture;
    }
    
    void Renderer::SetSampler(std::shared_ptr<Sampler> sampler) {
        m_sampler = sampler;
    }
    
    void Renderer::SetSyncManager(std::shared_ptr<SyncManager> syncManager) {
        m_syncManager = syncManager;
    }

    std::shared_ptr<Base> Renderer::GetBase() {
        return m_base;
    }
    
    std::shared_ptr<Window> Renderer::GetWindow() {
        return m_window;
    }
    
    std::shared_ptr<Surface> Renderer::GetSurface() {
        return m_surface;
    }
    
    std::shared_ptr<Swapchain> Renderer::GetSwapchain() {
        return m_swapchain;
    }
    
    std::shared_ptr<StagingBuffer> Renderer::GetStagingBuffer() {
        return m_stagingBuffer;
    }
    
    std::shared_ptr<VertexBuffer> Renderer::GetVertexBuffer() {
        return m_vertexBuffer;
    }
    
    std::shared_ptr<IndexBuffer> Renderer::GetIndexBuffer() {
        return m_indexBuffer;
    }

    std::shared_ptr<Descriptor> Renderer::GetDescriptor() {
        return m_descriptor;
    }
    
    std::shared_ptr<UniformBuffer> Renderer::GetUniformBuffer() {
        return m_uniformBuffer;
    }
    
    std::shared_ptr<Pipeline> Renderer::GetPipeline() {
        return m_pipeline;
    }
    
    std::shared_ptr<Framebuffer> Renderer::GetFramebuffer() {
        return m_framebuffer;
    }

    std::shared_ptr<CommandBuffer> Renderer::GetCommandBuffer() {
        return m_commandBuffer;
    }

    std::shared_ptr<Texture> Renderer::GetTexture() {
        return m_texture;
    }
    
    std::shared_ptr<Sampler> Renderer::GetSampler() {
        return m_sampler;
    }
    
    std::shared_ptr<SyncManager> Renderer::GetSyncManager() {
        return m_syncManager;
    }
};