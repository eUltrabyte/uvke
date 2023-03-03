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

    class UVKE_API Logger {
    public:
        Logger(std::string_view filename);
        virtual ~Logger();

        virtual void Log(std::string_view message, Severity severity, bool save);

        static void Log(std::string_view message, Severity severity = Severity::Debug);
        static void Log(std::string_view message, Severity severity, std::string_view filename);

    private:
        std::ofstream m_file;

    };
};

#endif