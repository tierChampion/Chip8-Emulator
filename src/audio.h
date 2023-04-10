#pragma once

#include <SDL/SDL.h>
#include <cstdint>

namespace ch8 {

	/*
	Audio system for the Chip8
	*/
	class Audio {

		float				_time;				// Timing counter
		const float			_frequency = 440;	// Frequency of the sound

		SDL_AudioDeviceID	_deviceId;			// Audio device in use
		SDL_AudioSpec		_spec, _aspec;		// Specifications of the audio device

	public:

		Audio();

		static void audioCallback(void* userData, uint8_t* stream, int len);

		void playFrequency(float freq);
		void stop();

	};

}
