#pragma once

#include <SDL/SDL.h>
#include <cstdint>

namespace ch8 {

	/*
	Audio system for the Chip8
	*/
	class Audio {

		static constexpr float	_AMPLITUDE = 40000;
		static constexpr int	_SAMPLING_FREQ = 48000;


		SDL_AudioDeviceID		_deviceId;			// Audio device in use
		SDL_AudioSpec			_spec, _aspec;		// Specifications of the audio device

	public:

		static float			_time;				// Timing counter
		static constexpr float			_frequency = 440;	// Frequency of the sound

		Audio();

		static void audioCallback(void* userData, uint8_t* stream, int len);

		void playFrequency(float freq);
		void stop();

	};

}
