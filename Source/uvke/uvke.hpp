#pragma once
#ifndef UVKE_CORE_HEADER
#define UVKE_CORE_HEADER

#include "uvkepch.hpp"

#include "Core/Platform.hpp"
#include "Core/Logger.hpp"

#ifdef UVKE_DEBUG
    #define UVKE_LOG(x) { uvke::Log(std::string(x), uvke::Severity::Debug); }
    #define UVKE_LOG_FILE(x) { uvke::Log(std::string(x), uvke::Severity::Debug, "log.txt"); }
    #define UVKE_TRACE() { uvke::Log(std::string(__FILE__) + " - " + std::to_string(__LINE__) + " - " + std::string(__FUNCTION__), uvke::Severity::Trace); }
    #define UVKE_FATAL(x) { uvke::Log(std::string(x), uvke::Severity::Fatal); }
#else
    #define UVKE_LOG(x)
    #define UVKE_LOG_FILE(x)
    #define UVKE_TRACE()
    #define UVKE_FATAL(x)
#endif

#define UVKE_BYTE(x) (1 << x)
#define UVKE_ASSERT(x) if(x <= -1) { UVKE_TRACE(); }

#include "Utils/Utils.hpp"
#include "Utils/Vec2.hpp"
#include "Utils/Vec3.hpp"
#include "Utils/Vec4.hpp"
#include "Utils/Mat4x4.hpp"
#include "Utils/Clock.hpp"
#include "Utils/File.hpp"

#include "Core/Window.hpp"

// TODO
// move vulkan swapchain from renderer to swapchain class
// move vulkan pipeline from renderer to pipeline class
// add more fences and frames in flight support
// change file class to support other types than std::vector<char>
// somehow fix main ( remove uvke::priv::init and uvke::priv::deinit )
// load and render picture
// add something like vulkan wrapper as alternative for vulkansdk
// reimplement windows with own library
// reimplement stb_image with own library

#endif