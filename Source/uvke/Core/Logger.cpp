#include "Logger.hpp"

namespace uvke {
    void Log(std::string_view message, Severity severity) {
        #ifdef UVKE_LOGFILE
            std::ofstream file("log.txt", std::ios::app);
        #endif

        switch(severity) {
            case Severity::Debug: {
                #ifdef UVKE_LOGFILE
                    file << "[ uvke ] [ debug ] " << message << std::endl;
                #endif

                std::cout << "\x1b[38;5;82m[ uvke ] \x1b[38;5;39m[ debug ] \x1b[37m" << message << "\x1b[0m\n";
            } break;

            case Severity::Trace: {
                #ifdef UVKE_LOGFILE
                    file << "[ uvke ] [ trace ] " << message << std::endl;
                #endif

                std::cout << "\x1b[38;5;82m[ uvke ] \x1b[38;5;70m[ trace ] \x1b[37m" << message << "\x1b[0m\n";
            } break;

            case Severity::Fatal: {
                #ifdef UVKE_LOGFILE
                    file << "[ uvke ] [ fatal ] " << message << std::endl;
                #endif

                std::cerr << "\x1b[38;5;82m[ uvke ] \x1b[38;5;124m[ fatal ] \x1b[37m" << message << "\x1b[0m\n";
            } break;

            default: {
                #ifdef UVKE_LOGFILE
                    file << "[ uvke ] [ debug ] " << message << std::endl;
                #endif
                
                std::cout << "\x1b[38;5;82m[ uvke ] \x1b[38;5;39m[ debug ] \x1b[37m" << message << "\x1b[0m\n";
            } break;
        }
    }
};