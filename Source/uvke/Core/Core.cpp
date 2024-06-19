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

    unsigned int Core::GetVulkanVersion() {
        unsigned int version = VK_API_VERSION_1_0;
        auto EnumerateInstanceVersion = PFN_vkEnumerateInstanceVersion(vkGetInstanceProcAddr(nullptr, "vkEnumerateInstanceVersion"));
        if(EnumerateInstanceVersion != nullptr) {
            EnumerateInstanceVersion(&version);
        } else {
            return 0;
        }

        return version;
    }

    VkResult Core::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
        auto FunctionPointer = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if(FunctionPointer != nullptr) {
            return FunctionPointer(instance, pCreateInfo, pAllocator, pDebugMessenger);
        } else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }
    
    void Core::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
        auto FunctionPointer = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (FunctionPointer != nullptr) {
            return FunctionPointer(instance, debugMessenger, pAllocator);
        }
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL Core::DebugUtilsMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
        switch(messageSeverity) {
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: {
                UVKE_LOG(pCallbackData->pMessage);
            } break;

            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: {
                UVKE_LOG(pCallbackData->pMessage);
            } break;

            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: {
                UVKE_FATAL(pCallbackData->pMessage);
            } break;
        }

        return VK_FALSE;
    }
};