#include <uvke/Core/App.hpp>

class Example : public uvke::App {
public:
    Example()
        : m_window(std::make_unique<uvke::Window>(uvke::WindowProps("uvke Example", { 1280, 720 }, uvke::Style::Default))), m_base(std::make_unique<uvke::Base>("uvke App")), m_renderer(std::make_unique<uvke::Renderer>(m_base.get(), m_window.get())) {
    }

    virtual ~Example() {
        m_renderer.release();
        m_renderer.reset();

        m_base.release();
        m_base.reset();

        m_window.release();
        m_window.reset();
    }

    virtual void Run() override {
        m_isRunning = true;

        uvke::Camera camera;
        camera.SetYaw(90.0f);

        m_renderer->SetCamera(&camera);

        uvke::Sprite sprite({ 0.4f, 0.3f }, "Resource/Textures/uvke.png");
        sprite.SetPosition({ 0.0f, 0.0f, 0.0f });
        sprite.SetRotation(0.0f);

        sprite.Create(m_renderer.get());

        m_renderer->Push(&sprite);

        while(m_isRunning) {
            Update();
            Render();
        }
    }

    virtual void Update() override {
        m_window->Update();

        switch(m_event.GetType()) {
            case uvke::EventType::Closed: { m_window->Close(); Shutdown(); } break;
            default: break;
        }

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
    Example example;
    example.Run();
    return 0;
};