#pragma once
#ifndef UVKE_HELPER
#define UVKE_HELPER

#include "../uvke.hpp"
#include "../Core/Base.hpp"

namespace uvke {
    class UVKE_API Helper {
    public:
        Helper() = default;
        virtual ~Helper() = default;

        static VkDeviceSize GetRequirementsSize(Base* base, VkBuffer buffer);
        static VkDeviceSize GetRequirementsSize(Base* base, VkImage image);

        static int GetMemoryIndex(Base* base, VkBuffer buffer);
        static int GetMemoryIndex(Base* base, VkImage image);

    };
};

#endif