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

    private:
        std::vector<std::shared_ptr<Window>> m_windows;

    };
};

#endif