#include "Renderer.hpp"
#include "UniformBuffer.hpp"

namespace uvke {
    Renderer::Renderer(std::shared_ptr<Base> base, std::shared_ptr<Window> window)
        : m_base(base), m_window(window), m_framebufferRecreated(false) {
        m_surface = std::make_shared<Surface>(m_base->GetInstance(), m_base->GetPhysicalDevice(), m_base->GetDevice(), *m_window.get());

        m_surface->SetQueueFamily(m_base->GetQueueFamily());
        m_surface->SetMultiQueueMode(m_base->IsMultiQueueSupported());
        
        {
            m_surface->CheckQueues();

            UVKE_LOG("Queue Family Index - " + std::to_string(m_base->GetQueueFamily()));

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(m_base->GetPhysicalDevice(), m_base->GetQueueFamily(), m_surface->GetSurface(), &presentSupport);
            UVKE_LOG("Queue Present Support - " + std::string(presentSupport ? "True" : "False"));
        }

        m_surface->SetSwapExtent(*m_window.get());

        UVKE_LOG("Format - " + std::to_string(m_surface->GetFormat().format));
        UVKE_LOG("Present Mode - " + std::to_string(m_surface->GetPresentMode()));
        UVKE_LOG("Extent - " + std::to_string(m_surface->GetExtent().width) + "/" + std::to_string(m_surface->GetExtent().height));

        m_swapchain = std::make_shared<Swapchain>(m_base->GetDevice(), m_surface.get());

        Shader shader(m_base->GetDevice(), File::Load("Resource/Shader.vert.spv"), File::Load("Resource/Shader.frag.spv"));
        UVKE_LOG("Shaders Loaded");

        m_vertexBuffer = std::make_shared<VertexBuffer>(m_base->GetPhysicalDevice(), m_base->GetDevice(), std::vector<Vertex> {
            {{ -0.5f, 0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }},
            {{ 0.0f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }},
            {{ 0.5f, 0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }},
        } );

        m_indexBuffer = std::make_shared<IndexBuffer>(m_base->GetPhysicalDevice(), m_base->GetDevice(), std::vector<unsigned int> {
            0, 1, 2,
        } );

        m_uniformBuffer = std::make_shared<UniformBuffer>(m_base->GetPhysicalDevice(), m_base->GetDevice());

        m_pipeline = std::make_shared<Pipeline>(m_base->GetDevice(), m_surface, std::make_shared<Shader>(shader), m_vertexBuffer, m_uniformBuffer);

        m_framebuffer = std::make_shared<Framebuffer>(m_base->GetDevice(), m_pipeline->GetRenderPass(), m_swapchain, m_surface);

        {
            VkCommandPoolCreateInfo commandPoolCreateInfo { };
            commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            commandPoolCreateInfo.pNext = nullptr;
            commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
            commandPoolCreateInfo.queueFamilyIndex = m_base->GetQueueFamily();

            UVKE_ASSERT(vkCreateCommandPool(m_base->GetDevice(), &commandPoolCreateInfo, nullptr, &m_commandPool));
        }

        m_stagingBuffer = std::make_shared<StagingBuffer>(m_base->GetPhysicalDevice(), m_base->GetDevice(), m_vertexBuffer->GetSize());

        m_stagingBuffer->Map(m_vertexBuffer->GetVertices().data());
        m_stagingBuffer->Copy(m_commandPool, m_surface->GetQueue(0), m_vertexBuffer->GetBuffer(), m_vertexBuffer->GetSize());

        m_stagingBuffer.reset();

        m_stagingBuffer = std::make_shared<StagingBuffer>(m_base->GetPhysicalDevice(), m_base->GetDevice(), m_indexBuffer->GetSize());

        m_stagingBuffer->Map(m_indexBuffer->GetIndices().data());
        m_stagingBuffer->Copy(m_commandPool, m_surface->GetQueue(0), m_indexBuffer->GetBuffer(), m_indexBuffer->GetSize());

        m_stagingBuffer.reset();

        {
            VkCommandBufferAllocateInfo commandBufferAllocateInfo { };
            commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            commandBufferAllocateInfo.pNext = nullptr;
            commandBufferAllocateInfo.commandPool = m_commandPool;
            commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            commandBufferAllocateInfo.commandBufferCount = 1;

            UVKE_ASSERT(vkAllocateCommandBuffers(m_base->GetDevice(), &commandBufferAllocateInfo, &m_commandBuffer));
        }

        {
            VkSemaphoreCreateInfo semaphoreCreateInfo { };
            semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
            semaphoreCreateInfo.pNext = nullptr;
            semaphoreCreateInfo.flags = 0;

            UVKE_ASSERT(vkCreateSemaphore(m_base->GetDevice(), &semaphoreCreateInfo, nullptr, &m_availableSemaphore));
            UVKE_ASSERT(vkCreateSemaphore(m_base->GetDevice(), &semaphoreCreateInfo, nullptr, &m_finishedSemaphore));

            VkFenceCreateInfo fenceCreateInfo { };
            fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            fenceCreateInfo.pNext = nullptr;
            fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

            UVKE_ASSERT(vkCreateFence(m_base->GetDevice(), &fenceCreateInfo, nullptr, &m_fence));
        }
    }

    Renderer::~Renderer() {
        vkDeviceWaitIdle(m_base->GetDevice());

        vkDestroyFence(m_base->GetDevice(), m_fence, nullptr);
        vkDestroySemaphore(m_base->GetDevice(), m_finishedSemaphore, nullptr);
        vkDestroySemaphore(m_base->GetDevice(), m_availableSemaphore, nullptr);

        vkDestroyCommandPool(m_base->GetDevice(), m_commandPool, nullptr);

        m_framebuffer.reset();

        m_pipeline.reset();

        m_uniformBuffer.reset();
        m_indexBuffer.reset();
        m_vertexBuffer.reset();

        m_swapchain.reset();
        m_surface.reset();
    }

    void Renderer::Render() {
        vkQueueWaitIdle(m_surface->GetQueue(1));

        unsigned int index = 0;
        VkResult result = vkAcquireNextImageKHR(m_base->GetDevice(), m_swapchain->GetSwapchain(), std::numeric_limits<unsigned long long>::infinity(), m_availableSemaphore, VK_NULL_HANDLE, &index);
        if(result == VK_ERROR_OUT_OF_DATE_KHR) {
            m_swapchain->Recreate(*m_window.get(), m_framebuffer->GetFramebuffers(), m_pipeline->GetRenderPass());
        } else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            UVKE_FATAL("Swapchain Acquire Image Error");
        }
        
        UniformBufferObject ubo { };
        ubo.model = Identity<float>();
        ubo.model = Scale<float>(ubo.model, vec3f(2.0f, 2.0f, 2.0f));
        ubo.model = Rotate<float>(ubo.model, vec3f(0.0f, 0.0f, 1.0f), std::chrono::duration<float, std::chrono::seconds::period>(std::chrono::steady_clock::now() - m_clock.GetStart()).count() * Radians(90.0f) * 4);

        ubo.view = LookAt<float>(vec3f(0.0f, 0.0f, -2.0f), vec3f(0.0f, 0.0f, 0.0f), vec3f(0.0f, 1.0f, -2.0f));

        if(glfwGetKey(m_window->GetWindow(), GLFW_KEY_SPACE) == GLFW_PRESS) {
            ubo.projection = Perspective<float>(Radians(45.0f), (m_window->GetWindowProps()->size.x / m_window->GetWindowProps()->size.y), 0.1f, 1000.0f);
            ubo.projection.data[1][1] *= -1;
        } else {
            ubo.projection = Ortho<float>(-1.0f, 1.0f, 1.0f, -1.0f, -150.0f, 100.0f);
        }

        m_uniformBuffer->Update(ubo);

        vkWaitForFences(m_base->GetDevice(), 1, &m_fence, VK_TRUE, std::numeric_limits<unsigned long long>::infinity());
        vkResetFences(m_base->GetDevice(), 1, &m_fence);

        vkResetCommandBuffer(m_commandBuffer, 0);
        RecordCommandBuffer(m_commandBuffer, index);
        
        VkSubmitInfo submitInfo { };
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.pNext = nullptr;
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &m_availableSemaphore;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_commandBuffer;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &m_finishedSemaphore;

        UVKE_ASSERT(vkQueueSubmit(m_surface->GetQueue(0), 1, &submitInfo, m_fence));

        VkPresentInfoKHR presentInfo { };
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.pNext = nullptr;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &m_finishedSemaphore;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &m_swapchain->GetSwapchain();
        presentInfo.pImageIndices = &index;
        presentInfo.pResults = nullptr;

        result = vkQueuePresentKHR(m_surface->GetQueue(1), &presentInfo);
        if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_framebufferRecreated) {
            m_framebufferRecreated = false;
            m_swapchain->Recreate(*m_window.get(), m_framebuffer->GetFramebuffers(), m_pipeline->GetRenderPass());
        } else if(result != VK_SUCCESS) {
            UVKE_FATAL("Framebuffer Recreation Error");
        }

        if(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - m_clock.GetStart()) >= std::chrono::seconds(1)) {
            m_clock.Restart();
        }
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
};