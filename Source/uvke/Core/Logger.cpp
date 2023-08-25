#include "Logger.hpp"

namespace uvke {
    Logger::Logger(std::string_view filename) {
        m_file.open(filename.data());
    }

    Logger::~Logger() {
        if(m_file.is_open()) {
            m_file.close();
        }
    }

    void Logger::Log(std::string_view message, Severity severity, bool save) {
        switch(severity) {
            case Severity::Debug: { m_file << "[ uvke ] [ debug ] " << message << "\n"; } break;
            case Severity::Trace: { m_file << "[ uvke ] [ trace ] " << message << "\n"; } break;
            case Severity::Fatal: { m_file << "[ uvke ] [ fatal ] " << message << "\n"; } break;
            default: { m_file << "[ uvke ] [ debug ] " << message << "\n"; } break;
        }
    }

    void Logger::Log(std::string_view message, Severity severity) {
        switch(severity) {
            case Severity::Debug: { std::cout << "\x1b[36m[ uvke ] \x1b[35m[ debug ] \x1b[37m" << message << "\x1b[0m\n"; } break;
            case Severity::Trace: { std::cout << "\x1b[36m[ uvke ] \x1b[32m[ trace ] \x1b[37m" << message << "\x1b[0m\n"; } break;
            case Severity::Fatal: { std::cerr << "\x1b[36m[ uvke ] \x1b[31m[ fatal ] \x1b[37m" << message << "\x1b[0m\n"; } break;
            default: { std::cout << "\x1b[36m[ uvke ] \x1b[35m[ debug ] \x1b[37m" << message << "\x1b[0m\n"; } break;
        }
    }

    void Logger::Log(std::string_view message, Severity severity, std::string_view filename) {
        std::ofstream file(filename.data());
        switch(severity) {
            case Severity::Debug: { file << "[ uvke ] [ debug ] " << message << "\n"; } break;
            case Severity::Trace: { file << "[ uvke ] [ trace ] " << message << "\n"; } break;
            case Severity::Fatal: { file << "[ uvke ] [ fatal ] " << message << "\n"; } break;
            default: { file << "[ uvke ] [ debug ] " << message << "\n"; } break;
        }
        file.close();
    }
};