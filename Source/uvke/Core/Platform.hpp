#pragma once
#ifndef UVKE_PLATFORM_HEADER
#define UVKE_PLATFORM_HEADER

#include "../uvkepch.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stb/stb_image.h>

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
    #include <Windows.h>
#endif

namespace uvke {
    namespace priv {
        extern int UVKE_API Init();
        extern int UVKE_API Deinit();
    };
    
    extern unsigned int UVKE_API GetSupportedVulkan();
};

#endif