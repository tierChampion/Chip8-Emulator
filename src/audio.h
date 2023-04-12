#pragma once

#include <SDL/SDL.h>
#include <cstdint>

namespace ch8 {

	/*
	Audio system for the Chip8
	*/
	class Audio {

		static constexpr float		_AMPLITUDE = 50;			// Amplitude of the sine wave
		static constexpr int		_SAMPLING_FREQ = 48000;		// Sampling frequency of the sine wave


		static SDL_AudioDeviceID	_deviceId;					// Audio device in use
		static SDL_AudioSpec		_spec, _aspec;				// Specifications of the audio device

	public:

		static float				_time;						// Timing counter
		static float				_frequency;					// Frequency of the sound

		Audio();

		static void audioCallback(void* userData, uint8_t* stream, int len);

		static void playFrequency(float freq);
		static void stop();

	};

}
