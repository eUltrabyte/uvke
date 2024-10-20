#pragma once
#ifndef UVKE_AUDIO_MANAGER_HEADER
#define UVKE_AUDIO_MANAGER_HEADER

#include "../uvke.hpp"
#include "../Core/Base.hpp"

#include <portaudio.h>

#define UVKE_ASSERT_PA(x) if(x != paNoError) { UVKE_LOG("Result - " + std::string(Pa_GetErrorText(x))); UVKE_TRACE(); }

namespace uvke {
    class UVKE_API AudioManager {
    public:
        AudioManager();
        virtual ~AudioManager();

        virtual void Play();

    };
};

#endif