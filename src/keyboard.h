#pragma once

#include <cinttypes>
#include <iostream>
#include <map>
#include <SDL/SDL.h>

namespace ch8 {

#define CHIP8_KEY_COUNT 16

	// Mapping from Keyboard key to Chip8 keyboard
	const static std::map<SDL_Scancode, uint8_t> _KEY_MAP = {
			{SDL_SCANCODE_1, 0x1 },		// 1 -> 1
			{SDL_SCANCODE_2, 0x2 },		// 2 -> 2
			{SDL_SCANCODE_3, 0x3 },		// 3 -> 3
			{SDL_SCANCODE_4, 0xC },		// c -> 4
			{SDL_SCANCODE_Q, 0x4 },		// 4 -> q
			{SDL_SCANCODE_W, 0x5 },		// 5 -> w
			{SDL_SCANCODE_E, 0x6 },		// 6 -> e
			{SDL_SCANCODE_R, 0xD },		// d -> r
			{SDL_SCANCODE_A, 0x7 },		// 7 -> a
			{SDL_SCANCODE_S, 0x8 },		// 8 -> s
			{SDL_SCANCODE_D, 0x9 },		// 9 -> d
			{SDL_SCANCODE_F, 0xE },		// e -> f
			{SDL_SCANCODE_Z, 0xA },		// a -> z
			{SDL_SCANCODE_X, 0x0 },		// 0 -> x
			{SDL_SCANCODE_C, 0xB },		// b -> c
			{SDL_SCANCODE_V, 0xF },		// f -> v
	};

	/*
	Input system of the Chip8
	*/
	class Keyboard {

		uint8_t		_keys[CHIP8_KEY_COUNT];		// States of the keys
		uint8_t		_awaited_key;	// Index of the first inputed key after waiting
		bool		_waiting;		// State if waiting for a key press

	public:

		Keyboard();

		void onKeyDown(SDL_KeyboardEvent* event);
		void onKeyUp(SDL_KeyboardEvent* event);

		bool isPressed(uint8_t keycode) const;

		bool isWaiting() const;
		uint8_t getWaited() const;
		void wait();

		friend std::ostream& operator<<(std::ostream& os, const Keyboard& kb);

	};
}
