#include "Event.hpp"

namespace uvke {
    Event::Event() { }

    void Event::SetType(const EventType& type) {
        m_type = type;
    }

    void Event::SetSize(const vec2i& size) {
        m_size = size;
    }
    
    void Event::SetFocused(const bool& focused) {
        m_focused = focused;
    }
    
    void Event::SetKey(const vec3i& key) {
        m_key = key;   
    }
    
    void Event::SetScroll(const vec2d& scroll) {
        m_scroll = scroll;
    }
    
    void Event::SetPosition(const vec2d& position) {
        m_position = position;
    }
    
    void Event::SetMouse(const vec2i& mouse) {
        m_mouse = mouse;
    }
        
    EventType& Event::GetType() {
        return m_type;
    }
        
    vec2i& Event::GetSize() {
        return m_size;
    }
        
    bool& Event::GetFocused() {
        return m_focused;
    }
        
    vec3i& Event::GetKey() {
        return m_key;
    }
        
    vec2d& Event::GetScroll() {
        return m_scroll;
    }
        
    vec2d& Event::GetPosition() {
        return m_position;
    }
    
    vec2i& Event::GetMouse() {
        return m_mouse;
    }
};