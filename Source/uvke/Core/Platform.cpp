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
            return 1;
        }

        int Deinit() {
            glfwTerminate();
            std::cin.get();
            return 1;
        }
    };
};