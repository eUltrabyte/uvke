#pragma once
#ifndef UVKE_WINDOW_MANAGER_HEADER
#define UVKE_WINDOW_MANAGER_HEADER

#include "../uvke.hpp"
#include "Window.hpp"

namespace uvke {
    class UVKE_API WindowManager {
    public:
        WindowManager();
        virtual ~WindowManager();

        virtual void Push(std::shared_ptr<Window> window);
        virtual void Erase();

        static WindowManager& Get() {
            if(m_instance == nullptr) {
                m_instance = new WindowManager;
            }

            return *m_instance;
        }

    private:
        static WindowManager* m_instance;
        std::vector<std::shared_ptr<Window>> m_windows;

    };
};

#endif