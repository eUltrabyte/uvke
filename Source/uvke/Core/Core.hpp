#pragma once
#ifndef UVKE_PLATFORM_CORE_HEADER
#define UVKE_PLATFORM_CORE_HEADER

#include "../uvkepch.hpp"

#include <vulkan/vulkan.h>
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
    #define STB_IMAGE_STATIC
    #define UVKE_API
#endif

#ifdef _WIN32
    #ifdef _WIN64
        #define UVKE_PLATFORM_WINDOWS
        #include <Windows.h>
    #else
        #error "uvke Doesn't Support 32-bit Platform"
    #endif
#elif defined(__linux__)
    #define UVKE_PLATFORM_LINUX
    #include <unistd.h>
#elif defined(__ANDROID__)
    #define UVKE_PLATFORM_ANDROID
    // todo
#elif defined(__EMSCRIPTEN__)
    #define UVKE_PLATFORM_EMSCRIPTEN
    // todo
#else
    #error "uvke Doesn't Support This Platform"
#endif

#if defined(__SSE__) || defined(__AVX__)
    #define UVKE_MATH_USE_SIMD
#endif

#ifdef UVKE_MATH_USE_SIMD
    #include <x86intrin.h>

    namespace uvke {
        namespace simd {
            #ifdef UVKE_MATH_USE_SIMD
                using vec4 = __m128;
            #endif
        };
    };
#endif

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "Logger.hpp"

namespace uvke {
    class UVKE_API Core {
    public:
        Core();
        virtual ~Core() = default;

        static unsigned int GetVulkanVersion();

        static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
        static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugUtilsMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

    };
};

#endif