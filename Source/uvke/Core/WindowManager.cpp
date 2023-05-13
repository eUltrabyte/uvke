#include "WindowManager.hpp"

namespace uvke {
    WindowManager::WindowManager() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        UVKE_LOG("Window Manager Created");
    }

    WindowManager::~WindowManager() {
        glfwTerminate();

        UVKE_LOG("Window Manager Destroyed");
    }

    void WindowManager::Push(std::shared_ptr<Window> window) {
        m_windows.emplace_back(window);
    }
    
    void WindowManager::Erase() {
        m_windows.erase(m_windows.begin());
    }
};