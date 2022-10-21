#pragma once
#ifndef UVKE_CORE_HEADER
#define UVKE_CORE_HEADER

#include "uvkepch.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Core/Platform.hpp"
#include "Core/Logger.hpp"

#ifdef UVKE_DEBUG
    #define UVKE_TRACE() { std::cerr << __FILE__ << __LINE__ << __FUNCTION__; }
#else
    #define UVKE_TRACE()
#endif

#define UVKE_BYTE(x) (1 << x)
#define UVKE_ASSERT(x) if(!x) { UVKE_TRACE(); }

#endif