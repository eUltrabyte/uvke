#include "WindowManager.hpp"

namespace uvke {
    WindowManager::WindowManager() {
        if(glfwInit() != GLFW_TRUE) {
            const char* description = "";
            glfwGetError(&description);
            UVKE_FATAL("GLFW Initialization Error - " + std::string(description));
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        glfwSetErrorCallback([](int error, const char* description) {
            UVKE_FATAL("GLFW Error - " + std::string(description));
            std::exit(error);
        });

        UVKE_LOG("Window Manager Created");
    }

    WindowManager::~WindowManager() {
        glfwTerminate();

        UVKE_LOG("Window Manager Destroyed");
    }
};