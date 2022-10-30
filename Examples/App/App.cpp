#include "uvke/uvke.hpp"

static int init = uvke::priv::Init();

auto main(int argc, char** argv) -> int {
    VkApplicationInfo appInfo { };
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = nullptr;
    appInfo.applicationVersion = VK_MAKE_API_VERSION(1, 1, 0, 0);
    appInfo.pApplicationName = "uvke";
    appInfo.engineVersion = VK_MAKE_API_VERSION(1, 1, 0, 0);
    appInfo.pEngineName = "uvke";
    appInfo.apiVersion = uvke::GetSupportedVulkan();

    std::vector<const char*> layers = {
        "VK_LAYER_KHRONOS_validation"
    };

    std::vector<const char*> extensions;
    {
        unsigned int glfwExtensionsCount = 0;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);
        extensions = std::vector<const char*>(glfwExtensions, glfwExtensions + glfwExtensionsCount);
    }

    VkInstance instance;
    {
        VkInstanceCreateInfo instanceCreateInfo { };
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pNext = nullptr;
        instanceCreateInfo.flags = 0;
        instanceCreateInfo.pApplicationInfo = &appInfo;
        instanceCreateInfo.enabledLayerCount = layers.size();
        instanceCreateInfo.ppEnabledLayerNames = layers.data();
        instanceCreateInfo.enabledExtensionCount = extensions.size();
        instanceCreateInfo.ppEnabledExtensionNames = extensions.data();

        UVKE_ASSERT(vkCreateInstance(&instanceCreateInfo, nullptr, &instance));
    }

    for(auto i = 0; i < layers.size(); ++i) {
        UVKE_LOG(layers.at(i));
    }

    for(auto i = 0; i < extensions.size(); ++i) {
        UVKE_LOG(extensions.at(i));
    }

    std::vector<VkPhysicalDevice> physicalDevices;
    {
        unsigned int physicalDevicesCount = 0;
        vkEnumeratePhysicalDevices(instance, &physicalDevicesCount, nullptr);
        physicalDevices = std::vector<VkPhysicalDevice>(physicalDevicesCount);
        vkEnumeratePhysicalDevices(instance, &physicalDevicesCount, physicalDevices.data());
    }

    std::vector<VkPhysicalDeviceProperties> physicalDevicesProperties(physicalDevices.size());
    std::vector<VkPhysicalDeviceFeatures> physicalDevicesFeatures(physicalDevices.size());
    for(auto i = 0; i < physicalDevices.size(); ++i) {
        vkGetPhysicalDeviceProperties(physicalDevices.at(i), &physicalDevicesProperties.at(i));
        vkGetPhysicalDeviceFeatures(physicalDevices.at(i), &physicalDevicesFeatures.at(i));
        UVKE_LOG("GPU #" + std::to_string(i) + " - " + physicalDevicesProperties.at(i).deviceName);
    }

    std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    VkDevice device;
    VkQueue queue;
    {
        float deviceQueuePriorities[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
        VkDeviceQueueCreateInfo deviceQueueCreateInfo { };
        deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        deviceQueueCreateInfo.pNext = nullptr;
        deviceQueueCreateInfo.flags = 0;
        deviceQueueCreateInfo.queueFamilyIndex = 0;
        deviceQueueCreateInfo.queueCount = 1;
        deviceQueueCreateInfo.pQueuePriorities = deviceQueuePriorities;

        VkDeviceCreateInfo deviceCreateInfo { };
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.pNext = nullptr;
        deviceCreateInfo.flags = 0;
        deviceCreateInfo.queueCreateInfoCount = 1;
        deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
        deviceCreateInfo.enabledLayerCount = 0;
        deviceCreateInfo.ppEnabledLayerNames = nullptr;
        deviceCreateInfo.enabledExtensionCount = deviceExtensions.size();
        deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
        deviceCreateInfo.pEnabledFeatures = &physicalDevicesFeatures.at(0);

        UVKE_ASSERT(vkCreateDevice(physicalDevices.at(0), &deviceCreateInfo, nullptr, &device));

        vkGetDeviceQueue(device, 0, 0, &queue);
    }

    uvke::Window window(uvke::WindowProps("uvke test", { 1280, 720 }, uvke::Style::Default));

    VkSurfaceKHR surface;
    window.CreateSurface(instance, &surface);

    while(window.IsOpen()) {
        window.Update();
    }

    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyDevice(device, nullptr);
    vkDestroyInstance(instance, nullptr);
    return uvke::priv::Deinit();
}