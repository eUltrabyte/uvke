#include <uvke/Graphics/Renderer.hpp>
#include <uvke/Graphics/Sprite.hpp>
#include <uvke/Core/App.hpp>

class ExampleApp : public uvke::App {
public:
    ExampleApp()
        : m_window(std::make_shared<uvke::Window>(uvke::WindowProps("uvke Example App", { 1280, 720 }, uvke::Style::Default))), m_base(std::make_shared<uvke::Base>("uvke App")), m_renderer(std::make_shared<uvke::Renderer>(m_base, m_window)) {
    }

    virtual ~ExampleApp() {
        m_renderer.reset();
        m_base.reset();
        m_window.reset();
    }

    virtual void Run() override {
        m_isRunning = true;

        uvke::Sprite sprite({ 0.2f, 0.15f });
        sprite.SetPosition({ 0.0f, 0.0f });
        sprite.SetRotation(0.0f);
        sprite.Create(m_renderer);
        m_renderer->Push(std::make_shared<uvke::Sprite>(sprite));

        while(m_isRunning) {
            Update();
            Render();
        }
    }

    virtual void Update() override {
        // UVKE_LOG("App Timer - " + std::to_string(m_clock->GetElapsedTime().count()) + "ms");
        m_window->Update();

        /* switch(m_event.GetType()) {
            case uvke::EventType::Closed: { UVKE_LOG("Event - Closed"); } break;
            case uvke::EventType::Resized: { UVKE_LOG("Event - Resized - " + std::to_string(m_event.GetSize().x) + "/" + std::to_string(m_event.GetSize().y)); } break;
            case uvke::EventType::Focused: { UVKE_LOG("Event - Focused - " + std::to_string(m_event.GetFocused())); } break;
            case uvke::EventType::Unfocused: { UVKE_LOG("Event - Unfocused - " + std::to_string(m_event.GetFocused())); } break;
            case uvke::EventType::KeyPressed: { UVKE_LOG("Event - KeyPressed - " + std::to_string(m_event.GetKey().x) + "/" + std::to_string(m_event.GetKey().y) + "/" + std::to_string(m_event.GetKey().z)); } break;
            case uvke::EventType::KeyReleased: { UVKE_LOG("Event - KeyReleased - " + std::to_string(m_event.GetKey().x) + "/" + std::to_string(m_event.GetKey().y) + "/" + std::to_string(m_event.GetKey().z)); } break;
            case uvke::EventType::KeyRepeated: { UVKE_LOG("Event - KeyRepeated - " + std::to_string(m_event.GetKey().x) + "/" + std::to_string(m_event.GetKey().y) + "/" + std::to_string(m_event.GetKey().z)); } break;
            case uvke::EventType::MouseScrolled: { UVKE_LOG("Event - MouseScrolled - " + std::to_string(m_event.GetScroll().x) + "/" + std::to_string(m_event.GetScroll().y)); } break;
            case uvke::EventType::MouseMoved: { UVKE_LOG("Event - MouseMoved - " + std::to_string(m_event.GetPosition().x) + "/" + std::to_string(m_event.GetPosition().y)); } break;
            case uvke::EventType::ButtonPressed: { UVKE_LOG("Event - ButtonPressed - " + std::to_string(m_event.GetMouse().x) + "/" + std::to_string(m_event.GetMouse().y)); } break;
            case uvke::EventType::ButtonReleased: { UVKE_LOG("Event - ButtonReleased - " + std::to_string(m_event.GetMouse().x) + "/" + std::to_string(m_event.GetMouse().y)); } break;
        } */

        m_window->PollEvents(m_event);
    }

    virtual void Render() override {
        m_renderer->Render();
    }

    virtual void Shutdown() override {
        m_isRunning = false;
    }

private:
    uvke::Event m_event;
    std::shared_ptr<uvke::Window> m_window;
    std::shared_ptr<uvke::Base> m_base;
    std::shared_ptr<uvke::Renderer> m_renderer;

};

auto main(int argc, char** argv) -> int {
    ExampleApp example;
    example.Run();
    return 0;
};