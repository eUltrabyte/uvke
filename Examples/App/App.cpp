#include <uvke/Graphics/Renderer.hpp>
#include <uvke/Core/Entry.hpp>

uvke::App* Create(int argc, char** argv) {
    uvke::Window window(uvke::WindowProps("uvke Test", { 1280, 720 }, uvke::Style::Default));
    uvke::Base base(window.GetWindowProps()->title);
    uvke::Renderer renderer(std::make_shared<uvke::Base>(base), std::make_shared<uvke::Window>(window));

    uvke::Event event;
    while(window.GetStatus()) {
        window.PollEvents(event);
        switch(event.GetType()) {
            case uvke::EventType::Closed: { UVKE_LOG("Event - Closed"); } break;
            case uvke::EventType::Resized: { UVKE_LOG("Event - Resized - " + std::to_string(event.GetSize().x) + "/" + std::to_string(event.GetSize().y)); } break;
            case uvke::EventType::Focused: { UVKE_LOG("Event - Focused - " + std::to_string(event.GetFocused())); } break;
            case uvke::EventType::Unfocused: { UVKE_LOG("Event - Unfocused - " + std::to_string(event.GetFocused())); } break;
            case uvke::EventType::KeyPressed: { UVKE_LOG("Event - KeyPressed - " + std::to_string(event.GetKey().x) + "/" + std::to_string(event.GetKey().y) + "/" + std::to_string(event.GetKey().z)); } break;
            case uvke::EventType::KeyReleased: { UVKE_LOG("Event - KeyReleased - " + std::to_string(event.GetKey().x) + "/" + std::to_string(event.GetKey().y) + "/" + std::to_string(event.GetKey().z)); } break;
            case uvke::EventType::KeyRepeated: { UVKE_LOG("Event - KeyRepeated - " + std::to_string(event.GetKey().x) + "/" + std::to_string(event.GetKey().y) + "/" + std::to_string(event.GetKey().z)); } break;
            case uvke::EventType::MouseScrolled: { UVKE_LOG("Event - MouseScrolled - " + std::to_string(event.GetScroll().x) + "/" + std::to_string(event.GetScroll().y)); } break;
            case uvke::EventType::MouseMoved: { UVKE_LOG("Event - MouseMoved - " + std::to_string(event.GetPosition().x) + "/" + std::to_string(event.GetPosition().y)); } break;
            case uvke::EventType::ButtonPressed: { UVKE_LOG("Event - ButtonPressed - " + std::to_string(event.GetMouse().x) + "/" + std::to_string(event.GetMouse().y)); } break;
            case uvke::EventType::ButtonReleased: { UVKE_LOG("Event - ButtonReleased - " + std::to_string(event.GetMouse().x) + "/" + std::to_string(event.GetMouse().y)); } break;
        }

        window.Update();
        renderer.Render();
    }

    return nullptr;
}