#include <uvke/uvke.hpp>
#include <uvke/Graphics/Renderer.hpp>

static int init = uvke::priv::Init();

auto main(int argc, char** argv) -> int {
    uvke::Window window(uvke::WindowProps("uvke test", { 1280, 720 }, uvke::Style::Default));
    uvke::Renderer renderer(window);

    while(window.IsOpen() && view.IsOpen()) {
        window.Update();
        renderer.Render();
    }

    return uvke::priv::Deinit();
}
