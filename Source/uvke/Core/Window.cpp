#include "Window.hpp"

namespace uvke {
    namespace priv {
        void glfwErrorCallback(int error, const char* description) { UVKE_LOG("GLFW Error - " + std::to_string(error) + " - " + description); }
    };

    Window::Window(const WindowProps& windowProps) {
        glfwSetErrorCallback(priv::glfwErrorCallback);

        SetWindowProps(windowProps);

        UVKE_LOG("Window Created");
    }

    Window::~Window() {
        if(m_window != nullptr) {
            glfwDestroyWindow(m_window);
        }

        UVKE_LOG("Window Destroyed");
    }

    void Window::CreatePlatformSurface(VkInstance instance, VkSurfaceKHR* surface) {
        glfwCreateWindowSurface(instance, m_window, nullptr, surface);
    }

    void Window::Update() {
        vec2i size(0, 0);
        glfwGetFramebufferSize(m_window, &size.x, &size.y);
        if(m_windowProps->size.x != size.x || m_windowProps->size.y != size.y) {
            m_windowProps->size = size;
        }
        
        glfwPollEvents();
    }

    void Window::Wait() {
        glfwWaitEvents();
    }

    void Window::SetWindowProps(const WindowProps& windowProps) {
        m_windowProps = std::make_shared<WindowProps>(windowProps);
        std::string title = "";

        if(windowProps.style == Style::None) {
            glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        } else {
            if(windowProps.style & Style::Titlebar) {
                title = windowProps.title;
            }

            if(windowProps.style & Style::Resizable) {
                glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
            } else {
                glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
            }
        }

        m_window = glfwCreateWindow(windowProps.size.x, windowProps.size.y, title.c_str(), nullptr, nullptr);

        if(windowProps.style & Style::Fullscreen) {
            int count = 0;
            GLFWmonitor** monitors = glfwGetMonitors(&count);
            glfwSetWindowMonitor(m_window, monitors[0], 0, 0, windowProps.size.x, windowProps.size.y, 60);
            delete[] monitors;
        }
    }

    std::shared_ptr<WindowProps> Window::GetWindowProps() {
        return m_windowProps;
    }

    GLFWwindow* Window::GetWindow() {
        return m_window;
    }

    bool Window::GetStatus() {
        return !glfwWindowShouldClose(m_window);
    }
};