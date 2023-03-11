#pragma once
#ifndef UVKE_CORE_HEADER
#define UVKE_CORE_HEADER

#include "uvkepch.hpp"

#include "Core/Platform.hpp"
#include "Core/Logger.hpp"

#ifdef UVKE_DEBUG
    #define UVKE_LOG(x) { uvke::Logger::Log(std::string(x), uvke::Severity::Debug); }
    #define UVKE_LOG_FILE(x) { uvke::Logger::Log(std::string(x), uvke::Severity::Debug, "log.txt"); }
    #define UVKE_TRACE() { uvke::Logger::Log(std::string(__FILE__) + " - " + std::to_string(__LINE__) + " - " + std::string(__FUNCTION__), uvke::Severity::Trace); }
    #define UVKE_FATAL(x) { uvke::Logger::Log(std::string(x), uvke::Severity::Fatal); }
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
// add multi rendering support?
// asset manager class
// reimplement stb_image with own library
// add some gui ( imgui or own )
// add profiler
// reimplement windows with own library
// add something like vulkan wrapper as alternative for vulkansdk

#endif