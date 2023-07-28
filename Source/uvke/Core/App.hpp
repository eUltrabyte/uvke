#pragma once
#ifndef UVKE_APP_HEADER
#define UVKE_APP_HEADER

#include "../uvke.hpp"
#include "../Window/WindowManager.hpp"

namespace uvke {
    class UVKE_API App {
    public:
        App();
        virtual ~App();

        virtual void Run() = 0;
        virtual void Update() = 0;
        virtual void Render() = 0;
        virtual void Shutdown() = 0;

    protected:
        bool m_isRunning;
        std::unique_ptr<Clock> m_clock;
        std::unique_ptr<Core> m_core;
        std::unique_ptr<WindowManager> m_windowManager;

    };
};

#endif