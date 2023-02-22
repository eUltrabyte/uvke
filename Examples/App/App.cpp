#include <uvke/Graphics/Renderer.hpp>

static int init = uvke::priv::Init();

auto main(int argc, char** argv) -> int {
    uvke::Window window(uvke::WindowProps("uvke Test", { 1280, 720 }, uvke::Style::Default));
    uvke::Base base(window.GetWindowProps()->title);
    uvke::Renderer renderer(std::make_shared<uvke::Base>(base), std::make_shared<uvke::Window>(window));

    while(window.GetStatus()) {
        window.Update();
        renderer.Render();
    }

    return uvke::priv::Deinit();
}