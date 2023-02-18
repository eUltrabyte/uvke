#include "Platform.hpp"

namespace uvke {
    namespace priv {
        int Init() {
            #ifdef _WIN32
                void* handle = GetStdHandle(STD_OUTPUT_HANDLE);
                unsigned long mode = 0;
                GetConsoleMode(handle, &mode);
                mode |= 0x0004;
                SetConsoleMode(handle, mode);
            #endif

            glfwInit();
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            
            return 1;
        }

        int Deinit() {
            glfwTerminate();
            std::cin.get();
            return 1;
        }
    };

    unsigned int GetSupportedVulkan() {
        unsigned int version = VK_API_VERSION_1_0;
        auto EnumerateInstanceVersion = PFN_vkEnumerateInstanceVersion(vkGetInstanceProcAddr(nullptr, "vkEnumerateInstanceVersion"));
        if(EnumerateInstanceVersion != nullptr) {
            EnumerateInstanceVersion(&version);
        } else {
            return 0;
        }

        return version;
    }
};