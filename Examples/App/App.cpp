#include "uvke/uvke.hpp"

static int init = uvke::priv::Init();

auto main(int argc, char** argv) -> int {
    UVKE_LOG("test");
    UVKE_TRACE();
    UVKE_FATAL("test");

    /* VkApplicationInfo appInfo { };
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = nullptr;
    appInfo.applicationVersion = VK_MAKE_API_VERSION(1, 1, 0, 0);
    appInfo.pApplicationName = "uvke";
    appInfo.engineVersion = VK_MAKE_API_VERSION(1, 1, 0, 0);
    appInfo.pEngineName = "uvke";
    appInfo.apiVersion = uvke::GetSupportedVulkan(); */

    uvke::Window window(uvke::WindowProps("uvke test", { 1280, 720 }, uvke::Style::Default));

    while(window.IsOpen()) {

        window.Update();
    }

    return uvke::priv::Deinit();
}