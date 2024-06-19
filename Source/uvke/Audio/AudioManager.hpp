#pragma once
#ifndef UVKE_AUDIO_MANAGER_HEADER
#define UVKE_AUDIO_MANAGER_HEADER

#include "../uvke.hpp"
#include "../Core/Base.hpp"

#include <portaudio.h>

namespace uvke {
    class UVKE_API AudioManager {
    public:
        AudioManager();
        virtual ~AudioManager();

    };
};

#endif