#pragma once
#ifndef UVKE_CORE_HEADER
#define UVKE_CORE_HEADER

#include "uvkepch.hpp"

#include "Core/Platform.hpp"
#include "Core/Logger.hpp"

#ifdef UVKE_DEBUG
    #define UVKE_LOG(x) { uvke::Log(std::string(x), uvke::Severity::Debug); }
    #define UVKE_TRACE() { uvke::Log(std::string(__FILE__) + " - " + std::to_string(__LINE__) + " - " + std::string(__FUNCTION__), uvke::Severity::Trace); }
    #define UVKE_FATAL(x) { uvke::Log(std::string(x), uvke::Severity::Fatal); }
#else
    #define UVKE_LOG(x)
    #define UVKE_TRACE()
    #define UVKE_FATAL(x)
#endif

#define UVKE_BYTE(x) (1 << x)
#define UVKE_ASSERT(x) if(x <= -1) { UVKE_TRACE(); }

#include "Utils/Vec2.hpp"
#include "Utils/Vec3.hpp"
#include "Utils/Vec4.hpp"
#include "Utils/Clock.hpp"

#include "Core/Window.hpp"

#endif