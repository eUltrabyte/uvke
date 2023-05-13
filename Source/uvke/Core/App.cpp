#include "App.hpp"

namespace uvke {
    App::App()
        : m_isRunning(false), m_core(std::make_unique<Core>()), m_windowManager(std::make_unique<WindowManager>()) { }

    App::~App() {
        m_windowManager.reset();
        m_core.reset();
    }
};