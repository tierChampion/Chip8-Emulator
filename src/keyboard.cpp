#include "keyboard.h"

namespace ch8 {


	Keyboard::Keyboard() {

		_awaited_key = 0x00;
		_waiting = false;

		for (int i = 0; i < CHIP8_KEY_COUNT; i++) {
			_keys[i] = 0x00;
		}
	}


	/**
	* Event for the pressing.
	* @param event - SDL key event
	*/
	void Keyboard::onKeyDown(SDL_KeyboardEvent* event) {

		// Check if it is a repeat key
		if (event->repeat != 0)
			return;

		auto pair = _KEY_MAP.find(event->keysym.scancode);

		// Check if the input is valid
		if (pair != _KEY_MAP.end()) {

			_keys[pair->second] = 0xFF;

			if (_waiting) {
				_awaited_key = pair->second;
				_waiting = false;
			}
		}
	}

	/**
	* Event for the releasing.
	* @param event - SDL key event
	*/
	void Keyboard::onKeyUp(SDL_KeyboardEvent* event) {

		auto pair = _KEY_MAP.find(event->keysym.scancode);

		// Check if the input is valid
		if (pair != _KEY_MAP.end()) {

			_keys[pair->second] = 0x00;
		}
	}

	bool Keyboard::isPressed(uint8_t keycode) const {
		return _keys[keycode];
	}

	bool Keyboard::isWaiting() const {
		return _waiting;
	}

	uint8_t Keyboard::getWaited() const {
		return _awaited_key;
	}

	void Keyboard::wait() {
		_waiting = true;
	}

	/**
	* Operator to print the key states.
	* @param os - Stream to print to
	* @param kb - Keyboard to print
	*/
	std::ostream& operator<<(std::ostream& os, const Keyboard& kb) {
		os << "[ ";

		for (uint8_t key : kb._keys) {
			os << ((key > 0x00) ? 1 : 0) << " ";
		}

		return os << "]\n";
	}
}