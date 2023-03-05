#pragma once
#ifndef UVKE_SYNC_MANAGER_HEADER
#define UVKE_SYNC_MANAGER_HEADER

#include "../uvke.hpp"

namespace uvke {
    class UVKE_API SyncManager {
    public:
        SyncManager(VkDevice device = nullptr);
        virtual ~SyncManager();

        virtual void Update();
        virtual void Recreate();

        virtual void WaitForDevice();
        virtual void WaitForQueue(VkQueue queue);
        virtual void WaitForFence(unsigned int index);
        virtual void ResetFence(unsigned int index);

        virtual void SetDevice(VkDevice device);
        virtual void SetFrame(unsigned int frame);
        virtual void SetSize(unsigned int size);
        virtual void SetAvailableSemaphores(std::vector<VkSemaphore> semaphores);
        virtual void SetFinishedSemaphores(std::vector<VkSemaphore> semaphores);
        virtual void SetFences(std::vector<VkFence> fences);

        virtual VkDevice& GetDevice();
        virtual unsigned int& GetFrame();
        virtual unsigned int& GetSize();
        virtual std::vector<VkSemaphore>& GetAvailableSemaphores();
        virtual VkSemaphore& GetAvailableSemaphore(unsigned int index);
        virtual std::vector<VkSemaphore>& GetFinishedSemaphores();
        virtual VkSemaphore& GetFinishedSemaphore(unsigned int index);
        virtual std::vector<VkFence>& GetFences();
        virtual VkFence& GetFence(unsigned int index);

    protected:
        VkDevice m_device;

    private:
        unsigned int m_frame;
        unsigned int m_size;
        std::vector<VkSemaphore> m_availableSemaphores;
        std::vector<VkSemaphore> m_finishedSemaphores;
        std::vector<VkFence> m_fences;

    };
};

#endif