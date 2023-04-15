#pragma once
#ifndef UVKE_WINDOW_HEADER
#define UVKE_WINDOW_HEADER

#include "../uvke.hpp"
#include "Event.hpp"

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

        WindowProps(std::string_view title = "uvke Window", const vec2u& size = vec2u(1280, 720), unsigned style = Style::Default) : title(title), size(size), style(style) {  }
        ~WindowProps() = default;
    };

    class UVKE_API Window {
    public:
        Window(const WindowProps& windowProps = WindowProps());
        virtual ~Window();

        virtual void CreatePlatformSurface(VkInstance instance, VkSurfaceKHR* surface);

        virtual void PollEvents(Event& event);
        virtual void Update();
        virtual void Wait();

        virtual void SetWindowProps(const WindowProps& windowProps);
    
        virtual std::shared_ptr<WindowProps> GetWindowProps();
        virtual GLFWwindow* GetWindow();
        virtual bool GetStatus();
        virtual Event& GetEvent();
        virtual int GetKey(int key);
        virtual int GetButton(int button);
    
    private:
        std::shared_ptr<WindowProps> m_windowProps;
        GLFWwindow* m_window;
        Event m_event;
    
    };
};

#endif