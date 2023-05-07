#include "Presentation.hpp"

namespace uvke {
    Presentation::Presentation(std::shared_ptr<Base> base, std::shared_ptr<Swapchain> swapchain, std::shared_ptr<SyncManager> syncManager)
        : m_base(base), m_swapchain(swapchain), m_syncManager(syncManager), m_index(0), m_submitInfo({ }), m_presentInfo({ }) {
        m_submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        m_submitInfo.pNext = nullptr;

        m_presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        m_presentInfo.pNext = nullptr;
        m_presentInfo.swapchainCount = 1;
        m_presentInfo.pSwapchains = &m_swapchain->GetSwapchain();
        m_presentInfo.pResults = nullptr;
    }

    void Presentation::AcquireNextImage(std::shared_ptr<Window> window, std::shared_ptr<Pipeline> pipeline, std::shared_ptr<Framebuffer> framebuffer) {
        m_result = vkAcquireNextImageKHR(m_base->GetDevice(), m_swapchain->GetSwapchain(), std::numeric_limits<uint64_t>::infinity(), m_syncManager->GetAvailableSemaphore(m_syncManager->GetFrame()), VK_NULL_HANDLE, &m_index);
        if(m_result == VK_ERROR_OUT_OF_DATE_KHR) {
            m_swapchain->Recreate(window, pipeline->GetRenderPass());
            framebuffer->Recreate();
        } else if(m_result != VK_SUCCESS && m_result != VK_SUBOPTIMAL_KHR) {
            UVKE_FATAL("Swapchain Acquire Image Error");
        }
    }
    
    void Presentation::Submit(std::shared_ptr<CommandBuffer> commandBuffer, std::shared_ptr<Surface> surface) {
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        m_submitInfo.pWaitDstStageMask = waitStages;
        m_submitInfo.waitSemaphoreCount = 1;
        m_submitInfo.pWaitSemaphores = &m_syncManager->GetAvailableSemaphore(m_syncManager->GetFrame());
        m_submitInfo.commandBufferCount = 1;
        m_submitInfo.pCommandBuffers = &commandBuffer->GetCommandBuffer(m_syncManager->GetFrame());
        m_submitInfo.signalSemaphoreCount = 1;
        m_submitInfo.pSignalSemaphores = &m_syncManager->GetFinishedSemaphore(m_syncManager->GetFrame());

        UVKE_ASSERT(vkQueueSubmit(surface->GetQueue(0), 1, &m_submitInfo, m_syncManager->GetFence(m_syncManager->GetFrame())));
    }
    
    void Presentation::Present(std::shared_ptr<Window> window, std::shared_ptr<Surface> surface, std::shared_ptr<Pipeline> pipeline, std::shared_ptr<Framebuffer> framebuffer) {
        m_presentInfo.waitSemaphoreCount = 1;
        m_presentInfo.pWaitSemaphores = &m_syncManager->GetFinishedSemaphore(m_syncManager->GetFrame());
        m_presentInfo.pImageIndices = &m_index;

        m_result = vkQueuePresentKHR(surface->GetQueue(1), &m_presentInfo);
        if(m_result == VK_ERROR_OUT_OF_DATE_KHR || m_result == VK_SUBOPTIMAL_KHR || m_swapchain->IsRecreated()) {
            m_swapchain->Recreate(window, pipeline->GetRenderPass());
            framebuffer->Recreate();
        } else if(m_result != VK_SUCCESS) {
            UVKE_FATAL("Framebuffer Recreation Error");
        }
    }

    unsigned int& Presentation::GetIndex() {
        return m_index;
    }

    VkResult& Presentation::GetResult() {
        return m_result;
    }
    
    VkSubmitInfo& Presentation::GetSubmitInfo() {
        return m_submitInfo;
    }
    
    VkPresentInfoKHR& Presentation::GetPresentInfo() {
        return m_presentInfo;
    }
};