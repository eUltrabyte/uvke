#include "Window.hpp"

namespace uvke {
    Window::Window(const WindowProps& windowProps)
        : m_windowProps(std::make_shared<WindowProps>(windowProps)) {
        std::string temporaryTitle = "";

        if(windowProps.style == Style::None) {
            glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        } else {
            if(windowProps.style & Style::Titlebar) {
                temporaryTitle = windowProps.title;
            }

            if(windowProps.style & Style::Resizable) {
                glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
            } else {
                glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
            }
        }

        m_window = glfwCreateWindow(windowProps.size.x, windowProps.size.y, temporaryTitle.c_str(), nullptr, nullptr);
        UVKE_ASSERT(m_window);

        int count = 0;
        GLFWmonitor** monitors = glfwGetMonitors(&count);
        UVKE_ASSERT(monitors);

        if(windowProps.style & Style::Fullscreen) {
            glfwSetWindowMonitor(m_window, monitors[0], 0, 0, windowProps.size.x, windowProps.size.y, 60);
        }
    }

    Window::~Window() {
        glfwDestroyWindow(m_window);
    }

    void Window::Update() {
        glfwPollEvents();
    }

    bool Window::IsOpen() {
        return !glfwWindowShouldClose(m_window);
    }

    std::shared_ptr<WindowProps> Window::GetWindowProps() {
        return m_windowProps;
    }
};