#include "Core.hpp"

namespace uvke {
    Core::Core() {
        #ifdef UVKE_PLATFORM_WINDOWS
            void* handle = GetStdHandle(STD_OUTPUT_HANDLE);
            unsigned long mode = 0;
            GetConsoleMode(handle, &mode);
            mode |= 0x0004;
            SetConsoleMode(handle, mode);
        #endif
    }

    unsigned int Core::GetSupportedVulkan() {
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