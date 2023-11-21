#include "SyncManager.hpp"

namespace uvke {
    SyncManager::SyncManager(Base* base)
        : m_base(base){
        m_frame = 0;
        m_size = 2;
        m_availableSemaphores.resize(m_size);
        m_finishedSemaphores.resize(m_size);
        m_fences.resize(m_size);

        VkSemaphoreCreateInfo semaphoreCreateInfo { };
        semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        semaphoreCreateInfo.pNext = nullptr;
        semaphoreCreateInfo.flags = 0;

        VkFenceCreateInfo fenceCreateInfo { };
        fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceCreateInfo.pNext = nullptr;
        fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for(auto i = 0; i < m_size; ++i) {
            UVKE_ASSERT(vkCreateSemaphore(m_base->GetDevice(), &semaphoreCreateInfo, nullptr, &m_availableSemaphores[i]));
            UVKE_ASSERT(vkCreateSemaphore(m_base->GetDevice(), &semaphoreCreateInfo, nullptr, &m_finishedSemaphores[i]));
            UVKE_ASSERT(vkCreateFence(m_base->GetDevice(), &fenceCreateInfo, nullptr, &m_fences[i]));
        }

        UVKE_LOG_ADDRESS("Sync Manager Created");
    }

    SyncManager::~SyncManager() {
        for(auto i = 0; i < m_size; ++i) {
            vkDestroyFence(m_base->GetDevice(), m_fences[i], nullptr);
            vkDestroySemaphore(m_base->GetDevice(), m_finishedSemaphores[i], nullptr);
            vkDestroySemaphore(m_base->GetDevice(), m_availableSemaphores[i], nullptr);
        }

        m_fences.clear();
        m_finishedSemaphores.clear();
        m_availableSemaphores.clear();

        UVKE_LOG_ADDRESS("Sync Manager Destroyed");
    }

    void SyncManager::Update() {
        m_frame = (m_frame + 1) % m_size;
    }

    void SyncManager::Recreate() {
        for(auto i = 0; i < m_size; ++i) {
            vkDestroyFence(m_base->GetDevice(), m_fences[i], nullptr);
            vkDestroySemaphore(m_base->GetDevice(), m_finishedSemaphores[i], nullptr);
            vkDestroySemaphore(m_base->GetDevice(), m_availableSemaphores[i], nullptr);
        }

        m_fences.clear();
        m_finishedSemaphores.clear();
        m_availableSemaphores.clear();

        m_availableSemaphores.resize(m_size);
        m_finishedSemaphores.resize(m_size);
        m_fences.resize(m_size);

        VkSemaphoreCreateInfo semaphoreCreateInfo { };
        semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        semaphoreCreateInfo.pNext = nullptr;
        semaphoreCreateInfo.flags = 0;

        VkFenceCreateInfo fenceCreateInfo { };
        fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceCreateInfo.pNext = nullptr;
        fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for(auto i = 0; i < m_size; ++i) {
            UVKE_ASSERT(vkCreateSemaphore(m_base->GetDevice(), &semaphoreCreateInfo, nullptr, &m_availableSemaphores[i]));
            UVKE_ASSERT(vkCreateSemaphore(m_base->GetDevice(), &semaphoreCreateInfo, nullptr, &m_finishedSemaphores[i]));
            UVKE_ASSERT(vkCreateFence(m_base->GetDevice(), &fenceCreateInfo, nullptr, &m_fences[i]));
        }

        UVKE_LOG("Sync Manager Recreated");
    }

    void SyncManager::WaitForDevice() {
        vkDeviceWaitIdle(m_base->GetDevice());
    }

    void SyncManager::WaitForQueue(VkQueue queue) {
        vkQueueWaitIdle(queue);
    }
    
    void SyncManager::WaitForFence(unsigned int index) {
        if(index < 0 || index >= m_fences.size()) {
            vkWaitForFences(m_base->GetDevice(), 1, &m_fences[0], VK_TRUE, std::numeric_limits<unsigned long long>::infinity());
        } else {
            vkWaitForFences(m_base->GetDevice(), 1, &m_fences[index], VK_TRUE, std::numeric_limits<unsigned long long>::infinity());
        }
    }

    void SyncManager::ResetFence(unsigned int index) {
        if(index < 0 || index >= m_fences.size()) {
            vkResetFences(m_base->GetDevice(), 1, &m_fences[0]);
        } else {
            vkResetFences(m_base->GetDevice(), 1, &m_fences[index]);
        }
    }

    void SyncManager::SetBase(Base* base) {
        m_base = base;
    }

    void SyncManager::SetFrame(unsigned int frame) {
        m_frame = frame;
    }

    void SyncManager::SetSize(unsigned int size) {
        m_size = size;
    }
    
    void SyncManager::SetAvailableSemaphores(std::vector<VkSemaphore> semaphores) {
        m_availableSemaphores = semaphores;
    }
    
    void SyncManager::SetFinishedSemaphores(std::vector<VkSemaphore> semaphores) {
        m_finishedSemaphores = semaphores;
    }
    
    void SyncManager::SetFences(std::vector<VkFence> fences) {
        m_fences = fences;
    }

    unsigned int& SyncManager::GetFrame() {
        return m_frame;
    }
    
    unsigned int& SyncManager::GetSize() {
        return m_size;
    }
    
    std::vector<VkSemaphore>& SyncManager::GetAvailableSemaphores() {
        return m_availableSemaphores;
    }

    VkSemaphore& SyncManager::GetAvailableSemaphore(unsigned int index) {
        if(index < 0 || index >= m_availableSemaphores.size()) {
            return m_availableSemaphores[0];
        } else {
            return m_availableSemaphores[index];
        }
    }
    
    std::vector<VkSemaphore>& SyncManager::GetFinishedSemaphores() {
        return m_finishedSemaphores;
    }

    VkSemaphore& SyncManager::GetFinishedSemaphore(unsigned int index) {
        if(index < 0 || index >= m_finishedSemaphores.size()) {
            return m_finishedSemaphores[0];
        } else {
            return m_finishedSemaphores[index];
        }
    }
    
    std::vector<VkFence>& SyncManager::GetFences() {
        return m_fences;
    }

    VkFence& SyncManager::GetFence(unsigned int index) {
        if(index < 0 || index >= m_fences.size()) {
            return m_fences[0];
        } else {
            return m_fences[index];
        }
    }
};