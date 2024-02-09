#include <uvke/Core/App.hpp>

class Windows : public uvke::App {
public:
    Windows()
        : m_base(std::make_unique<uvke::Base>("uvke App")) {
        m_windows.emplace_back(std::make_unique<uvke::Window>(uvke::WindowProps("uvke Windows 1", { 1280, 720 }, uvke::Style::Default)));
        m_windows.emplace_back(std::make_unique<uvke::Window>(uvke::WindowProps("uvke Windows 2", { 640, 360 }, uvke::Style::Default)));
    }

    virtual ~Windows() {
        m_base.release();
        m_base.reset();

        for(auto& window : m_windows) {
            window.release();
            window.reset();
        }

        m_windows.clear();
    }

    virtual void Run() override {
        m_isRunning = true;

        while(m_isRunning) {
            Update();
        }
    }

    virtual void Update() override {
        for(auto& window : m_windows) {
            window->Update();

            switch(m_event.GetType()) {
                case uvke::EventType::Closed: { window->Close(); Shutdown(); } break;
                default: break;
            }

            window->PollEvents(m_event);
        }
    }

    virtual void Render() override { }

    virtual void Shutdown() override {
        m_isRunning = false;
    }

private:
    uvke::Event m_event;
    std::vector<std::unique_ptr<uvke::Window>> m_windows;
    std::unique_ptr<uvke::Base> m_base;
    
};

auto main(int argc, char** argv) -> int {
    Windows windows;
    windows.Run();
    return 0;
};