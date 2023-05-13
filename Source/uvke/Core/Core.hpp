#pragma once
#ifndef UVKE_PLATFORM_CORE_HEADER
#define UVKE_PLATFORM_CORE_HEADER

#include "../uvkepch.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#ifdef UVKE_SHARED_LIBRARY
    #ifdef UVKE_SHARED_BUILD
        #ifdef _MSC_VER
            #define UVKE_API __declspec(dllexport)
        #else
            #define UVKE_API __attribute__((visibility("default")))
        #endif
    #else
        #ifdef _MSC_VER
            #define UVKE_API __declspec(dllimport)
        #else
            #define UVKE_API __attribute__((visibility("hidden")))
        #endif
    #endif
#else
    #define UVKE_API
#endif

#ifdef _WIN32
    #ifdef _WIN64
        #define UVKE_PLATFORM_WINDOWS
        #include <Windows.h>
    #else
        #error "uvke Doesn't Support 32-bit Platform"
    #endif
#elif __linux__
    #define UVKE_PLATFORM_LINUX
    // todo
#elif __ANDROID__
    #define UVKE_PLATFORM_ANDROID
    // todo
#else
    #error "uvke Doesn't Support This Platform"
#endif

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

namespace uvke {
    class UVKE_API Core {
    public:
        Core();
        virtual ~Core() = default;

        static unsigned int GetSupportedVulkan();

    };
};

#endif