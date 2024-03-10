#include <uvke/Core/App.hpp>

class Test : public uvke::App {
public:
    Test()
        : m_window(std::make_unique<uvke::Window>(uvke::WindowProps("uvke Test", { 1280, 720 }, uvke::Style::Default))), m_base(std::make_unique<uvke::Base>("uvke App")), m_renderer(std::make_unique<uvke::Renderer>(m_base.get(), m_window.get())) {
    }

    virtual ~Test() {
        m_renderer.release();
        m_renderer.reset();

        m_base.release();
        m_base.reset();

        m_window.release();
        m_window.reset();
    }

    virtual void Run() override {
        m_isRunning = true;

        m_window->ChangeCursorVisibility(uvke::CursorType::Disabled);

        /* uvke::Sprite sprite({ 0.4f, 0.3f }, "Resource/Textures/uvke.png");
        sprite.SetPosition({ 0.0f, 0.0f, 0.0f });
        sprite.SetRotation(0.0f);

        sprite.Create(m_renderer.get());

        m_renderer->Push(&sprite); */

        /* uvke::Sprite sprite1({ 0.2f, 0.15f });
        sprite1.SetPosition({ 0.0f, 0.0f, -0.1f });
        sprite1.SetRotation(0.0f);
        sprite1.Create(m_renderer.get());

        unsigned int* imageData = new unsigned int[sprite1.GetTexture()->GetSize().x * sprite1.GetTexture()->GetSize().y];
        for(auto i = 0; i < sprite1.GetTexture()->GetSize().x * sprite1.GetTexture()->GetSize().y; ++i) {
            imageData[i] = 0xffff00ff;
        }

        sprite1.GetTexture()->SetData(m_renderer->GetCommandBuffer(), m_renderer->GetSurface()->GetQueue(0), { sprite1.GetTexture()->GetSize().x, sprite1.GetTexture()->GetSize().y }, imageData);

        m_renderer->Push(&sprite1); */

        uvke::MeshLoader meshLoader("Resource/Models/Cube.obj");

        uvke::Mesh model({ 0.0f, 0.0f, 0.0f }, "Resource/Textures/Cube.png");
        model.SetPosition({ 0.0f, 0.0f, 0.0f });
        model.SetRotation(180.0f, { 1.0f, 0.0f, 0.0f });
        model.SetMesh(&meshLoader);

        model.Create(m_renderer.get());

        m_renderer->Push(&model);

        uvke::Sprite triangle;
        triangle.SetVertices({ { { -1.0f, 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },
                               { { 0.0f, -1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
                               { { 1.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } } });
        triangle.SetIndices({ 0, 1, 2 });
        triangle.SetPosition({ 0.0f, 0.0f, 10.0f });

        triangle.Create(m_renderer.get());

        m_renderer->Push(&triangle);

        while(m_isRunning) {
            Update();
            Render();
        }
    }

    virtual void Update() override {
        m_window->Update();

        switch(m_event.GetType()) {
            case uvke::EventType::Closed: {
                m_window->Close(); Shutdown();
            } break;

            case uvke::EventType::KeyReleased: {
                if(m_event.GetKey().x == GLFW_KEY_ESCAPE) {
                    m_window->ChangeCursorVisibility(uvke::CursorType::Shown);
                }
            } break;

            default: break;
        }

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
            default: break;
        } */

        m_window->PollEvents(m_event);

        m_renderer->Update();
    }

    virtual void Render() override {
        m_renderer->Render();
    }

    virtual void Shutdown() override {
        m_isRunning = false;
    }

private:
    uvke::Event m_event;
    std::unique_ptr<uvke::Window> m_window;
    std::unique_ptr<uvke::Base> m_base;
    std::unique_ptr<uvke::Renderer> m_renderer;

};

auto main(int argc, char** argv) -> int {
    Test test;
    test.Run();
    return 0;
};