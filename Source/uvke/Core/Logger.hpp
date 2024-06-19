#pragma once
#ifndef UVKE_LOGGER_HEADER
#define UVKE_LOGGER_HEADER

#include "../uvke.hpp"

namespace uvke {
    enum UVKE_API Severity {
        Debug,
        Trace,
        Fatal,
    };

    extern void Log(std::string_view message, Severity severity = Severity::Debug);
};

#endif