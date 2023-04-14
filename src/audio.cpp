#include "audio.h"
#include <iostream>

namespace ch8 {

	const AudioState Audio::_state = AudioState();

	void Audio::playFrequency(float freq) {
		Audio::_frequency = freq;
		SDL_PauseAudioDevice(_state._deviceId, 0);
	}

	void Audio::stop() {
		SDL_PauseAudioDevice(_state._deviceId, 1);
	}

	void Audio::audioCallback(void* userData, uint8_t* stream, int len) {

		len /= sizeof(uint8_t);

		for (int i = 0; i < len; i++) {
			stream[i] = Audio::_AMPLITUDE * sin(Audio::_time);

			Audio::_time += Audio::_frequency * (2 * M_PI) / (float)SAMPLING_FREQ;

			if (Audio::_time >= 2 * M_PI) Audio::_time -= 2 * M_PI;
		}
	}

	void Audio::closeAudio() {
		SDL_CloseAudioDevice(_state._deviceId);
	}
}