#pragma once
#ifndef UVKE_EVENT_HEADER
#define UVKE_EVENT_HEADER

#include "../uvke.hpp"

namespace uvke {
    enum class UVKE_API EventType {
        Closed,
        Resized,
        Focused,
        Unfocused,
        KeyPressed,
        KeyReleased,
        KeyRepeated,
        MouseScrolled,
        MouseMoved,
        ButtonPressed,
        ButtonReleased,
    };

    class UVKE_API Event {
    public:
        Event();
        virtual ~Event() = default;

        virtual void SetType(const EventType& type);
        virtual void SetSize(const vec2i& size);
        virtual void SetFocused(const bool& focused);
        virtual void SetKey(const vec3i& key);
        virtual void SetScroll(const vec2d& scroll);
        virtual void SetPosition(const vec2d& position);
        virtual void SetMouse(const vec2i& mouse);

        virtual EventType& GetType();
        virtual vec2i& GetSize();
        virtual bool& GetFocused();
        virtual vec3i& GetKey();
        virtual vec2d& GetScroll();
        virtual vec2d& GetPosition();
        virtual vec2i& GetMouse();

    private:
        EventType m_type;
        vec2i m_size;
        bool m_focused;
        vec3i m_key;
        vec2d m_scroll;
        vec2d m_position;
        vec2i m_mouse;

    };
};

#endif