#pragma once

#include "../include/SDL/SDL.h"
#include <cstdint>

namespace ch8 {

#define SAMPLING_FREQ 48000

	struct AudioState;

	/*
	Audio system for the Chip8
	*/
	class Audio {

		static constexpr float		_AMPLITUDE = 50;	// Amplitude of the sine wave

		static const AudioState		_state;				// SDL Audio states 

		inline static float			_time = 0;			// Timing counter
		inline static float			_frequency = 440;	// Frequency of the sound

	public:

		static void audioCallback(void* userData, uint8_t* stream, int len);

		static void playFrequency(float freq);
		static void stop();

		static void closeAudio();
	};

	struct AudioState {

		SDL_AudioDeviceID		_deviceId;		// Audio device in use
		SDL_AudioSpec			_spec, _aspec;	// Specifications of the audio device

		AudioState() {
			if (SDL_Init(SDL_INIT_AUDIO) < 0) {
				SDL_Log("Problem when initializing audio: %s\n", SDL_GetError());
				exit(1);
			}

			SDL_zero(_spec);
			_spec.freq = SAMPLING_FREQ;
			_spec.format = AUDIO_S16SYS;
			_spec.channels = 1;
			_spec.samples = 512;
			_spec.callback = ch8::Audio::audioCallback;
			_spec.userdata = NULL;

			_deviceId = SDL_OpenAudioDevice(nullptr, 0, &_spec, &_aspec, SDL_AUDIO_ALLOW_ANY_CHANGE);

			if (_deviceId <= 1) {
				SDL_Log("Problem when opening the audio device: %s\n", SDL_GetError());
			}
		}
	};
}
