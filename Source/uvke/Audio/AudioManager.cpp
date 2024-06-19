#include "AudioManager.hpp"

namespace uvke {
    AudioManager::AudioManager() {
        int error = Pa_Initialize();
        if(error != paNoError) {
            UVKE_LOG("Audio Manager Error - " + std::string(Pa_GetErrorText(error)));
            UVKE_ASSERT(-1);
        }

        UVKE_LOG_ADDRESS("Audio Manager Created");
    }

    AudioManager::~AudioManager() {
        int error = Pa_Terminate();
        if(error != paNoError) {
            UVKE_LOG("Audio Manager Error - " + std::string(Pa_GetErrorText(error)));
            UVKE_ASSERT(-1);
        }

        UVKE_LOG_ADDRESS("Audio Manager Destroyed");
    }
};