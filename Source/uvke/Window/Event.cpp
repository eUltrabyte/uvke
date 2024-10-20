#include "Event.hpp"

namespace uvke {
    Event::Event() { }

    void Event::SetType(const EventType& type) {
        m_type = type;
    }

    void Event::SetSize(const glm::ivec2& size) {
        m_size = size;
    }
    
    void Event::SetFocused(const bool& focused) {
        m_focused = focused;
    }
    
    void Event::SetKey(const glm::ivec3& key) {
        m_key = key;   
    }
    
    void Event::SetScroll(const glm::dvec2& scroll) {
        m_scroll = scroll;
    }
    
    void Event::SetPosition(const glm::dvec2& position) {
        m_position = position;
    }
    
    void Event::SetMouse(const glm::ivec2& mouse) {
        m_mouse = mouse;
    }
        
    EventType& Event::GetType() {
        return m_type;
    }
        
    glm::ivec2& Event::GetSize() {
        return m_size;
    }
        
    bool& Event::GetFocused() {
        return m_focused;
    }
        
    glm::ivec3& Event::GetKey() {
        return m_key;
    }
        
    glm::dvec2& Event::GetScroll() {
        return m_scroll;
    }
        
    glm::dvec2& Event::GetPosition() {
        return m_position;
    }
    
    glm::ivec2& Event::GetMouse() {
        return m_mouse;
    }
};