#pragma once

#include <cinttypes>
#include "keyboard.h"
#include "renderer.h"

namespace ch8 {

#define CHIP8_CLOCK_SPEED 16 // Ticks or ms

	/*
	Central processing unit of the Chip8.
	*/
	class Cpu {

		uint16_t	_opcode;			// Code of the operation execute
		uint8_t		_memory[4096];		// 4K-bytes memory
		uint16_t	_stack[16];			// 16 level stack
		uint8_t		_V[16];				// 16 registers
		uint16_t	_SP;				// Stack pointer
		uint16_t	_I;					// Address register
		uint16_t	_PC;				// Program counter

		bool		_draw_flag = true;	// If the display should be updated
		bool		_wait_flag = false;	// If the cpu is waiting for an input

		uint8_t		_delay_timer;		// Timer for the events
		uint8_t		_audio_timer;		// Timer for the audio
		uint32_t	_frame_start;		// Start of the current frame

		/* Opcode operations */

		uint8_t	inline	_x();		// Largest 4 bits of instruction
		uint8_t	inline	_y();		// Second smallest 4 bits of instruction
		uint8_t	inline	_n();		// Smallest 4 bits of instruction
		uint8_t	inline	_nn();		// Smallest byte of instruction
		uint16_t inline	_nnn();		// Smallest 12 bits of instruction

	public:

		Keyboard	_inputs;			// Input module
		Renderer	_visuals;			// Visual module
		// Audio

		Cpu();

		bool loadProgram(const char* rom_path);
		void cycle();

		void test_cycle(uint16_t opcode);
		bool needToDraw();
	};

	uint8_t Cpu::_x() {

		return (_opcode & 0x0F00) >> 8;
	}

	uint8_t Cpu::_y() {

		return (_opcode & 0x00F0) >> 4;
	}

	uint8_t Cpu::_n() {

		return _opcode & 0x000F;
	}

	uint8_t Cpu::_nn() {
		return _opcode & 0x00FF;
	}

	uint16_t Cpu::_nnn() {

		return _opcode & 0x0FFF;
	}

	const static unsigned char _CHIP8_FONTSET[80] =
	{
	  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	  0x20, 0x60, 0x20, 0x20, 0x70, // 1
	  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};
}