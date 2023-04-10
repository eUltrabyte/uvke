#include "Renderer.hpp"

namespace uvke {
    Renderer::Renderer(std::shared_ptr<Base> base, std::shared_ptr<Window> window)
        : m_base(base), m_window(window) {
        m_surface = std::make_shared<Surface>(m_base->GetInstance(), m_base->GetPhysicalDevice(), m_base->GetDevice(), m_window);

        m_surface->SetQueueFamily(m_base->GetQueueFamily());
        m_surface->SetMultiQueueMode(m_base->IsMultiQueueSupported());
        
        m_surface->CheckQueues();
        m_surface->SetSwapExtent(m_window);

        UVKE_LOG("Format - " + std::to_string(m_surface->GetFormat().format));
        UVKE_LOG("Present Mode - " + std::to_string(m_surface->GetPresentMode()));
        UVKE_LOG("Extent - " + std::to_string(m_surface->GetExtent().width) + "/" + std::to_string(m_surface->GetExtent().height));

        m_swapchain = std::make_shared<Swapchain>(m_base->GetDevice(), m_surface);

        m_shader = std::make_shared<Shader>(m_base->GetDevice(), File::Load("Resource/Shader.vert.spv"), File::Load("Resource/Shader.frag.spv"));

        m_commandBuffer = std::make_shared<CommandBuffer>(m_base->GetDevice(), m_base->GetQueueFamily());

        m_texture = std::make_shared<Texture>(m_base->GetPhysicalDevice(), m_base->GetDevice(), "Resource/uvke.png");

        m_stagingBuffer = std::make_shared<StagingBuffer>(m_base->GetPhysicalDevice(), m_base->GetDevice(), static_cast<unsigned int>(m_texture->GetSize().x * m_texture->GetSize().y * 4));
        m_stagingBuffer->Map(m_texture->GetPixels());
        m_texture->Allocate();

        m_texture->LayoutTransition(m_commandBuffer, m_surface->GetQueue(0), VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        m_texture->CopyFromBuffer(m_commandBuffer, m_surface->GetQueue(0), m_stagingBuffer->GetBuffer());
        m_texture->LayoutTransition(m_commandBuffer, m_surface->GetQueue(0), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        m_stagingBuffer.reset();

        m_sampler = std::make_shared<Sampler>(m_base->GetPhysicalDevice(), m_base->GetDevice(), m_texture);

        m_vertexBuffer = std::make_shared<VertexBuffer>(m_base->GetPhysicalDevice(), m_base->GetDevice(), std::vector<Vertex> {
            { { -0.2f, -0.15f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
            { { 0.2f, -0.15f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
            { { 0.2f, 0.15f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },
            { { -0.2f, 0.15f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },
        } );

        m_indexBuffer = std::make_shared<IndexBuffer>(m_base->GetPhysicalDevice(), m_base->GetDevice(), std::vector<unsigned int> {
            0, 1, 2, 2, 3, 0,
        } );

        m_uniformBuffer = std::make_shared<UniformBuffer>(m_base->GetPhysicalDevice(), m_base->GetDevice(), m_sampler->GetImageView(), m_sampler->GetSampler());

        m_vertexBuffer1 = std::make_shared<VertexBuffer>(m_base->GetPhysicalDevice(), m_base->GetDevice(), std::vector<Vertex> {
            { { 0.0f, -0.2f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.5f, 0.0f } },
            { { 0.2f, 0.2f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } },
            { { -0.2f, 0.2f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f } },
        } );

        m_indexBuffer1 = std::make_shared<IndexBuffer>(m_base->GetPhysicalDevice(), m_base->GetDevice(), std::vector<unsigned int> {
            0, 1, 2,
        } );

        m_uniformBuffer1 = std::make_shared<UniformBuffer>(m_base->GetPhysicalDevice(), m_base->GetDevice(), m_sampler->GetImageView(), m_sampler->GetSampler());

        m_pipeline = std::make_shared<Pipeline>(m_base->GetDevice(), m_surface, m_shader, m_vertexBuffer, m_uniformBuffer);

        m_framebuffer = std::make_shared<Framebuffer>(m_base->GetDevice(), m_pipeline->GetRenderPass(), m_swapchain, m_surface);

        m_syncManager = std::make_shared<SyncManager>(m_base->GetDevice());

        m_stagingBuffer = std::make_shared<StagingBuffer>(m_base->GetPhysicalDevice(), m_base->GetDevice(), m_vertexBuffer->GetSize());
        m_stagingBuffer->Map(m_vertexBuffer->GetVertices().data());
        m_stagingBuffer->Copy(m_commandBuffer->GetCommandPool(), m_surface->GetQueue(0), m_vertexBuffer->GetBuffer(), m_vertexBuffer->GetSize());
        m_stagingBuffer.reset();

        m_stagingBuffer = std::make_shared<StagingBuffer>(m_base->GetPhysicalDevice(), m_base->GetDevice(), m_indexBuffer->GetSize());
        m_stagingBuffer->Map(m_indexBuffer->GetIndices().data());
        m_stagingBuffer->Copy(m_commandBuffer->GetCommandPool(), m_surface->GetQueue(0), m_indexBuffer->GetBuffer(), m_indexBuffer->GetSize());
        m_stagingBuffer.reset();

        m_stagingBuffer = std::make_shared<StagingBuffer>(m_base->GetPhysicalDevice(), m_base->GetDevice(), m_vertexBuffer1->GetSize());
        m_stagingBuffer->Map(m_vertexBuffer1->GetVertices().data());
        m_stagingBuffer->Copy(m_commandBuffer->GetCommandPool(), m_surface->GetQueue(0), m_vertexBuffer1->GetBuffer(), m_vertexBuffer1->GetSize());
        m_stagingBuffer.reset();

        m_stagingBuffer = std::make_shared<StagingBuffer>(m_base->GetPhysicalDevice(), m_base->GetDevice(), m_indexBuffer1->GetSize());
        m_stagingBuffer->Map(m_indexBuffer1->GetIndices().data());
        m_stagingBuffer->Copy(m_commandBuffer->GetCommandPool(), m_surface->GetQueue(0), m_indexBuffer1->GetBuffer(), m_indexBuffer1->GetSize());
        m_stagingBuffer.reset();

        m_interface = std::make_shared<Interface>(m_base, m_window, m_surface, m_commandBuffer, m_pipeline->GetRenderPass());

        /* std::vector<VkDescriptorPoolSize> poolSizes = {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
        };

        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1000;
        pool_info.poolSizeCount = poolSizes.size();
        pool_info.pPoolSizes = poolSizes.data();

        UVKE_ASSERT(vkCreateDescriptorPool(m_base->GetDevice(), &pool_info, nullptr, &m_imguiPool));

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = nullptr;

        ImGui_ImplGlfw_InitForVulkan(m_window->GetWindow(), true);

        ImGui_ImplVulkan_InitInfo imguiVulkanInitInfo = { };
        imguiVulkanInitInfo.Instance = m_base->GetInstance();
        imguiVulkanInitInfo.PhysicalDevice = m_base->GetPhysicalDevice();
        imguiVulkanInitInfo.Device = m_base->GetDevice();
        imguiVulkanInitInfo.Queue = m_surface->GetQueue(0);
        imguiVulkanInitInfo.DescriptorPool = m_imguiPool;
        imguiVulkanInitInfo.MinImageCount = 3;
        imguiVulkanInitInfo.ImageCount = 3;
        imguiVulkanInitInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

        ImGui_ImplVulkan_Init(&imguiVulkanInitInfo, m_pipeline->GetRenderPass());

        VkCommandBuffer commandBuffer = m_commandBuffer->Begin();
        ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
        m_commandBuffer->End(commandBuffer, m_surface->GetQueue(0));

        ImGui_ImplVulkan_DestroyFontUploadObjects(); */

        UVKE_LOG("Renderer Created");
    }

    Renderer::~Renderer() {
        m_syncManager->WaitForDevice();

        /* vkDestroyDescriptorPool(m_base->GetDevice(), m_imguiPool, nullptr);
        ImGui_ImplVulkan_Shutdown(); */

        m_interface.reset();

        m_syncManager.reset();

        m_sampler.reset();

        m_texture.reset();

        m_commandBuffer.reset();

        m_framebuffer.reset();

        m_pipeline.reset();

        m_uniformBuffer.reset();
        m_indexBuffer.reset();
        m_vertexBuffer.reset();

        m_shader.reset();

        m_swapchain.reset();
        m_surface.reset();

        UVKE_LOG("Renderer Destroyed");
    }

    void Renderer::Render() {
        // UVKE_LOG("Render Time - " + std::to_string(std::chrono::duration<float, std::milli>(std::chrono::steady_clock::now() - m_frameClock.GetStart()).count()) + "ms");

        m_syncManager->WaitForQueue(m_surface->GetQueue(1));

        unsigned int index = 0;
        VkResult result = vkAcquireNextImageKHR(m_base->GetDevice(), m_swapchain->GetSwapchain(), std::numeric_limits<uint64_t>::infinity(), m_syncManager->GetAvailableSemaphore(m_syncManager->GetFrame()), VK_NULL_HANDLE, &index);
        if(result == VK_ERROR_OUT_OF_DATE_KHR) {
            m_swapchain->Recreate(m_window, m_framebuffer->GetFramebuffers(), m_pipeline->GetRenderPass());
        } else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            UVKE_FATAL("Swapchain Acquire Image Error");
        }
        
        UniformBufferObject ubo { };
        ubo.model = Identity<float>();
        ubo.model = Translate<float>(ubo.model, vec3f(0.5f, 0.0f, 0.0f));
        // ubo.model = Scale<float>(ubo.model, vec3f(1.0f, 1.0f, 1.0f));
        // ubo.model = Rotate<float>(ubo.model, vec3f(0.0f, 0.0f, 1.0f), std::chrono::duration<float, std::chrono::seconds::period>(std::chrono::steady_clock::now() - m_clock.GetStart()).count() * Radians(90.0f) * 4);

        ubo.view = LookAt<float>(vec3f(0.0f, 0.0f, -2.0f), vec3f(0.0f, 0.0f, 0.0f), vec3f(0.0f, 1.0f, -2.0f));

        if(glfwGetKey(m_window->GetWindow(), GLFW_KEY_SPACE) == GLFW_PRESS) {
            ubo.projection = Perspective<float>(Radians(45.0f), (m_window->GetWindowProps()->size.x / m_window->GetWindowProps()->size.y), 0.1f, 1000.0f);
            ubo.projection.data[1][1] *= -1;
        } else {
            ubo.projection = Ortho<float>(-1.0f, 1.0f, 1.0f, -1.0f, -150.0f, 100.0f);
        }

        m_uniformBuffer->Update(ubo);

        UniformBufferObject ubo1 { };
        ubo1.model = Identity<float>();
        ubo1.model = Translate<float>(ubo1.model, vec3f(-0.5f, 0.0f, 0.0f));

        ubo1.view = LookAt<float>(vec3f(0.0f, 0.0f, -2.0f), vec3f(0.0f, 0.0f, 0.0f), vec3f(0.0f, 1.0f, -2.0f));

        if(glfwGetKey(m_window->GetWindow(), GLFW_KEY_SPACE) == GLFW_PRESS) {
            ubo1.projection = Perspective<float>(Radians(45.0f), (m_window->GetWindowProps()->size.x / m_window->GetWindowProps()->size.y), 0.1f, 1000.0f);
            ubo1.projection.data[1][1] *= -1;
        } else {
            ubo1.projection = Ortho<float>(-1.0f, 1.0f, 1.0f, -1.0f, -150.0f, 100.0f);
        }

        m_uniformBuffer1->Update(ubo1);

        m_syncManager->WaitForFence(m_syncManager->GetFrame());
        m_syncManager->ResetFence(m_syncManager->GetFrame());

        m_interface->SetRenderTime(std::chrono::duration<float, std::milli>(std::chrono::steady_clock::now() - m_frameClock.GetStart()).count());
        m_pipeline->Render(m_framebuffer, m_commandBuffer, m_syncManager->GetFrame(), index, { m_vertexBuffer, m_vertexBuffer1 }, { m_indexBuffer, m_indexBuffer1 }, { m_uniformBuffer, m_uniformBuffer1 }, m_interface);

        VkSubmitInfo submitInfo { };
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.pNext = nullptr;
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &m_syncManager->GetAvailableSemaphore(m_syncManager->GetFrame());
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_commandBuffer->GetCommandBuffer(m_syncManager->GetFrame());
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &m_syncManager->GetFinishedSemaphore(m_syncManager->GetFrame());

        UVKE_ASSERT(vkQueueSubmit(m_surface->GetQueue(0), 1, &submitInfo, m_syncManager->GetFence(m_syncManager->GetFrame())));

        VkPresentInfoKHR presentInfo { };
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.pNext = nullptr;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &m_syncManager->GetFinishedSemaphore(m_syncManager->GetFrame());
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &m_swapchain->GetSwapchain();
        presentInfo.pImageIndices = &index;
        presentInfo.pResults = nullptr;

        result = vkQueuePresentKHR(m_surface->GetQueue(1), &presentInfo);
        if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_swapchain->IsRecreated()) {
            m_swapchain->Recreate(m_window, m_framebuffer->GetFramebuffers(), m_pipeline->GetRenderPass());
        } else if(result != VK_SUCCESS) {
            UVKE_FATAL("Framebuffer Recreation Error");
        }

        if(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - m_clock.GetStart()) >= std::chrono::seconds(1)) {
            m_clock.Restart();
        }

        m_syncManager->Update();
        m_frameClock.Restart();
    }

    void Renderer::SetBase(std::shared_ptr<Base> base) {
        m_base = base;
    }
    
    void Renderer::SetWindow(std::shared_ptr<Window> window) {
        m_window = window;
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