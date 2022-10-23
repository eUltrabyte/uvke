#include "uvke/uvke.hpp"

static int init = uvke::priv::Init();

auto main(int argc, char** argv) -> int {
    UVKE_LOG("test");
    UVKE_TRACE();
    UVKE_FATAL("test");

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    uvke::Window window(uvke::WindowProps("uvke test", { 1280, 720 }, uvke::Style::Default));

    while(window.IsOpen()) {

        window.Update();
    }

    return uvke::priv::Deinit();
}