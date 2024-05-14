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
#elif __linux__
    #define UVKE_PLATFORM_LINUX
    // todo
#elif __ANDROID__
    #define UVKE_PLATFORM_ANDROID
    // todo
#else
    #error "uvke Doesn't Support This Platform"
#endif

#ifdef __SSE__
    #define UVKE_MATH_USE_SIMD
#elif __AVX__
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

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#include <portaudio.h>

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