#include <uvke/Core/App.hpp>

class Mesh : public uvke::App {
public:
    Mesh()
        : m_window(std::make_unique<uvke::Window>(uvke::WindowProps("uvke Mesh", { 1280, 720 }, uvke::Style::Default))), m_base(std::make_unique<uvke::Base>("uvke App")), m_renderer(std::make_unique<uvke::Renderer>(m_base.get(), m_window.get())) {
    }

    virtual ~Mesh() {
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

        uvke::MeshLoader meshLoader("Resource/Models/Monkey.obj");

        uvke::Mesh model;
        model.SetPosition({ 0.0f, 0.0f, 0.0f });
        model.SetRotation(180.0f, { 1.0f, 0.0f, 0.0f });
        model.SetMesh(&meshLoader);

        model.Create(m_renderer.get());

        m_renderer->Push(&model);

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
    Mesh mesh;
    mesh.Run();
    return 0;
};