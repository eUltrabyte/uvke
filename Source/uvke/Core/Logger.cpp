#include "Logger.hpp"

namespace uvke {
    void Log(std::string_view message, Severity severity) {
        switch(severity) {
            case Severity::Debug: { std::cout << "[ uvke ] \x1b[35m[ debug ] " << message << "\x1b[0m\n"; } break;
            case Severity::Trace: { std::cout << "[ uvke ] \x1b[32m[ trace ] " << message << "\x1b[0m\n"; } break;
            case Severity::Fatal: { std::cerr << "[ uvke ] \x1b[31m[ fatal ] " << message << "\x1b[0m\n"; } break;
            default: { std::cout << "[ uvke ] \x1b[35m[ debug ] " << message << "\x1b[0m\n"; } break;
        }
    }

    void Log(std::string_view message, Severity severity, std::string_view filename) {
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