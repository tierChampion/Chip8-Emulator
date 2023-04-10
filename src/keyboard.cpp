#include "keyboard.h"

namespace ch8 {


	Keyboard::Keyboard() {

		_awaited_key = 0x0;
		_waiting = false;

		for (int i = 0; i < 16; i++) {
			_keys[i] = 0x0;
		}
	}


	void Keyboard::onKeyDown(SDL_KeyboardEvent* event) {

		// Check if it is a repeat key

		std::cout << "wow" << std::endl;

		if (event->repeat != 0)
			return;

		auto pair = _KEY_MAP.find(event->keysym.scancode);

		// Check if the input is valid
		if (pair != _KEY_MAP.end()) {

			_keys[pair->second] = 0xF;

			if (_waiting) {
				_awaited_key = pair->second;
				_waiting = false;
			}
		}
	}

	void Keyboard::onKeyUp(SDL_KeyboardEvent* event) {

		auto pair = _KEY_MAP.find(event->keysym.scancode);

		// Check if the input is valid
		if (pair != _KEY_MAP.end()) {

			_keys[pair->second] = 0x0;
		}
	}

	bool Keyboard::isPressed(uint8_t keycode) const {
		return _keys[keycode];
	}

	bool Keyboard::isWaiting() const {
		return _waiting;
	}

	void Keyboard::wait() {
		_waiting = true;
	}

	std::ostream& operator<<(std::ostream& os, const Keyboard& kb) {
		os << "[ ";

		for (uint8_t key : kb._keys) {
			os << key % 14 << " ";
		}

		return os << "]\n";
	}
}