#include "Window.hpp"

namespace uvke {
    Window::Window(const WindowProps& windowProps) {
        glfwSetErrorCallback([](int error, const char* description) {
            UVKE_LOG("GLFW Error - " + std::string(description));
            std::exit(error);
        });

        SetWindowProps(windowProps);

        glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
            Window* target = static_cast<Window*>(glfwGetWindowUserPointer(window));
            target->GetEvent().SetType(EventType::Closed);
        });

        glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
            Window* target = static_cast<Window*>(glfwGetWindowUserPointer(window));
            target->GetEvent().SetType(EventType::Resized);
            target->GetEvent().SetSize({ width, height });
        });

        glfwSetWindowFocusCallback(m_window, [](GLFWwindow* window, int focused) {
            Window* target = static_cast<Window*>(glfwGetWindowUserPointer(window));
            if(focused) {
                target->GetEvent().SetType(EventType::Focused);
                target->GetEvent().SetFocused(true);
            } else {
                target->GetEvent().SetType(EventType::Unfocused);
                target->GetEvent().SetFocused(false);
            }
        });

        glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            Window* target = static_cast<Window*>(glfwGetWindowUserPointer(window));
            if(action == GLFW_PRESS) {
                target->GetEvent().SetType(EventType::KeyPressed);
            } else if(action == GLFW_RELEASE) {
                target->GetEvent().SetType(EventType::KeyReleased);
            } else if(action == GLFW_REPEAT) {
                target->GetEvent().SetType(EventType::KeyRepeated);
            }

            target->GetEvent().SetKey({ key, scancode, mods });
        });

        glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset) {
            Window* target = static_cast<Window*>(glfwGetWindowUserPointer(window));
            target->GetEvent().SetType(EventType::MouseScrolled);
            target->GetEvent().SetScroll({ xOffset, yOffset });
        });

        glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double x, double y) {
            Window* target = static_cast<Window*>(glfwGetWindowUserPointer(window));
            target->GetEvent().SetType(EventType::MouseMoved);
            target->GetEvent().SetPosition({ x, y });
        });

        glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
            Window* target = static_cast<Window*>(glfwGetWindowUserPointer(window));
            if(action == GLFW_PRESS) {
                target->GetEvent().SetType(EventType::ButtonPressed);
            } else if(action == GLFW_RELEASE) {
                target->GetEvent().SetType(EventType::ButtonReleased);
            }

            target->GetEvent().SetMouse({ button, mods });
        });

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

    void Window::PollEvents(Event& event) {
        glfwPollEvents();
        event = m_event;
    }

    void Window::Update() {
        vec2i size(0, 0);
        glfwGetFramebufferSize(m_window, &size.x, &size.y);
        if(m_windowProps->size.x != size.x || m_windowProps->size.y != size.y) {
            m_windowProps->size = size;
        }
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

        glfwSetWindowUserPointer(m_window, this);
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

    Event& Window::GetEvent() {
        return m_event;
    }

    int Window::GetKey(int key) {
        return glfwGetKey(m_window, key);
    }

    int Window::GetButton(int button) {
        return glfwGetMouseButton(m_window, button);
    }
};