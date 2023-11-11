#include "App.hpp"

namespace uvke {
    App::App()
        : m_isRunning(false), m_core(std::make_unique<Core>()), m_windowManager(std::make_unique<WindowManager>()), m_clock(std::make_unique<Clock>()) { }

    App::~App() {
        m_windowManager.reset();

        m_core.reset();

        m_clock.reset();
    }
};