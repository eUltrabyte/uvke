#pragma once
#ifndef UVKE_WINDOW_HEADER
#define UVKE_WINDOW_HEADER

#include "../uvke.hpp"

namespace uvke {
    enum UVKE_API Style {
        None = 0,
        Fullscreen = UVKE_BYTE(0),
        Titlebar = UVKE_BYTE(1),
        Resizable = UVKE_BYTE(2),
        Default = UVKE_BYTE(1) | UVKE_BYTE(2),
    };

    struct UVKE_API WindowProps {
    public:
        std::string title;
        vec2u size;
        unsigned style;

        WindowProps(std::string_view a = "uvke window", const vec2u& b = vec2u(1280, 720), unsigned c = Style::Default) : title(a), size(b), style(c) {  }
    };

    class UVKE_API Window {
    public:
        Window(const WindowProps& windowProps = WindowProps());
        virtual ~Window();

        virtual void CreateSurface(VkInstance instance, VkSurfaceKHR* surface);

        virtual void Update();
    
        virtual bool IsOpen();
    
        virtual std::shared_ptr<WindowProps> GetWindowProps();
    
    private:
        std::shared_ptr<WindowProps> m_windowProps;
        GLFWwindow* m_window;
    
    };
};

#endif