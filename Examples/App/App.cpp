#include "uvke/uvke.hpp"

static int init = uvke::priv::Init();

auto main(int argc, char** argv) -> int {
    UVKE_LOG("test");
    UVKE_TRACE();
    UVKE_FATAL("test");

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "uvke App", nullptr, nullptr);

    for(; !glfwWindowShouldClose(window) ;) {
        glfwPollEvents();
    }

    return 0;
}