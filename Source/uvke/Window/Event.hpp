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
        virtual void SetSize(const glm::ivec2& size);
        virtual void SetFocused(const bool& focused);
        virtual void SetKey(const glm::ivec3& key);
        virtual void SetScroll(const glm::dvec2& scroll);
        virtual void SetPosition(const glm::dvec2& position);
        virtual void SetMouse(const glm::ivec2& mouse);

        virtual EventType& GetType();
        virtual glm::ivec2& GetSize();
        virtual bool& GetFocused();
        virtual glm::ivec3& GetKey();
        virtual glm::dvec2& GetScroll();
        virtual glm::dvec2& GetPosition();
        virtual glm::ivec2& GetMouse();

    private:
        EventType m_type;
        glm::ivec2 m_size;
        bool m_focused;
        glm::ivec3 m_key;
        glm::dvec2 m_scroll;
        glm::dvec2 m_position;
        glm::ivec2 m_mouse;

    };
};

#endif