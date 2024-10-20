#include "Window.hpp"

namespace uvke {
    Window::Window(const WindowProps& windowProps) {
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

        UVKE_LOG_ADDRESS("Window Created");
    }

    Window::~Window() {
        if(m_window != nullptr) {
            glfwDestroyWindow(m_window);
        }

        UVKE_LOG_ADDRESS("Window Destroyed");
    }

    void Window::CreatePlatformSurface(VkInstance instance, VkSurfaceKHR* surface) {
        glfwCreateWindowSurface(instance, m_window, nullptr, surface);
    }

    void Window::PollEvents(Event& event) {
        glfwPollEvents();
        event = m_event;
    }

    void Window::Update() {
        glm::ivec2 size(0, 0);
        glfwGetFramebufferSize(m_window, &size.x, &size.y);
        if(m_windowProps->size.x != size.x || m_windowProps->size.y != size.y) {
            m_windowProps->size = size;
        }
    }

    void Window::Wait() {
        glfwWaitEvents();
    }

    void Window::Close() {
        glfwSetWindowShouldClose(m_window, GLFW_TRUE);
    }

    void Window::ChangeCursorVisibility(CursorType cursorType) {
        switch(cursorType) {
            case CursorType::Shown: {
                glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            } break;

            case CursorType::Hidden: {
                glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            } break;

            case CursorType::Disabled: {
                glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            } break;
        }
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

        GLFWimage images[3];

        int width = 0, height = 0, channel = 0;
        stbi_uc* iconx16 = stbi_load("Resource/Textures/uvke-icon-x16.png", &width, &height, &channel, STBI_rgb_alpha);

        images[0].width = width;
        images[0].height = height;
        images[0].pixels = iconx16;

        stbi_uc* iconx32 = stbi_load("Resource/Textures/uvke-icon-x32.png", &width, &height, &channel, STBI_rgb_alpha);

        images[1].width = width;
        images[1].height = height;
        images[1].pixels = iconx32;

        stbi_uc* iconx48 = stbi_load("Resource/Textures/uvke-icon-x48.png", &width, &height, &channel, STBI_rgb_alpha);

        images[2].width = width;
        images[2].height = height;
        images[2].pixels = iconx48;

        glfwSetWindowIcon(m_window, 3, images);
        
        stbi_image_free(iconx48);
        stbi_image_free(iconx32);
        stbi_image_free(iconx16);

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

    glm::dvec2 Window::GetMouse() {
        double x = 0, y = 0;
        glfwGetCursorPos(m_window, &x, &y);
        return glm::dvec2(x, y);
    }
};