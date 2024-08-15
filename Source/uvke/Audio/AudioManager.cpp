#include "AudioManager.hpp"

namespace uvke {
    namespace priv {
        typedef struct {
            float left;
            float right;
        } paTestData;
    };

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

    void AudioManager::Play() {
        static priv::paTestData data;
        PaStream *stream;
        PaError error = Pa_OpenDefaultStream( &stream, 0, 2, paFloat32, 44100, 256, [](const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData) -> int {
            priv::paTestData *data = (priv::paTestData*)userData; 
            float *out = (float*)outputBuffer;
            (void) inputBuffer;
            
            for(unsigned int i=0; i < framesPerBuffer; i += 2) {
                out[i] = data->left;
                out[i + 1] = data->right;
                
                data->left = std::sin(time(0));
                data->right = std::sin(time(0) + 180);
            }

            return 0;
        }, &data);

        UVKE_ASSERT_PA(error);

        error = Pa_StartStream(stream);
        UVKE_ASSERT_PA(error);
    }
};