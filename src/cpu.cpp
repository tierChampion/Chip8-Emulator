#include "cpu.h"

#include <iostream>
#include <fstream>

namespace ch8 {


	Cpu::Cpu() {

		// Init pointers. Program starts at 0x200.
		_PC = 0x200;
		_opcode = 0;
		_I = 0;
		_SP = 0;
		_delay_timer = 0;
		_audio_timer = 0;
		_frame_start = SDL_GetTicks();

		// Init registers and stack
		for (int i = 0; i < 16; i++) {
			_V[i] = 0;
			_stack[i] = 0;
		}

		// Load native font set
		for (int i = 0; i < 80; i++) {
			_memory[i] = _CHIP8_FONTSET[i];
		}
	}

	bool Cpu::needToDraw() {
		bool temp = _draw_flag;
		_draw_flag = false;
		return temp;
	}

	/**
	* Load the program into memory.
	* @param rom_path - Path to the rom to load.
	*/
	bool Cpu::loadProgram(const char* rom_path) {

		std::ifstream file(rom_path, std::ios::binary);

		if (file.is_open()) {

			// Determine the size of the program
			file.seekg(0, std::ios::end);
			std::streampos file_size = file.tellg();
			file.seekg(0, std::ios::beg);

			// Load it into memory
			file.read((char*)&_memory[0x200], file_size);

			file.close();
		}
		else {

			std::perror("Failed to open the program");

			return false;
		}

		return true;
	}

	void Cpu::cycle() {

		// Ensure the proper clock speed for the timers
		uint32_t current_time = SDL_GetTicks();
		if (current_time - _frame_start < CHIP8_TICKS_PER_FRAME) {
			SDL_Delay(CHIP8_TICKS_PER_FRAME - (current_time - _frame_start));
		}

		_frame_start = SDL_GetTicks();

		// Fetch the next opcode. Stored in Big Endian.
		_opcode = _memory[_PC] << 8 | _memory[_PC + 1];

		// decode the opcode and execute it
		switch (_opcode & 0xF000) {

		case 0x0000: {
			switch (_opcode & 0x00FF) {
			case 0x00E0: {	// Clears the screen

				_visuals.clearScreen();
				_draw_flag = true;

				_PC += 2;

				break;
			}
			case 0x00EE: {	// Return from a subroutine

				_SP--; // TODO Possible underflow? 
				_PC = _stack[_SP];

				_PC += 2;

				break;
			}

			default: {
				std::cerr << "Unknown Opcode encountered: " << std::hex << _opcode << std::endl;
			}
			}

			break;
		}

		case 0x1000: {	// Jump to an address

			_PC = _nnn();

			break;
		}

		case 0x2000: {	// Calls a subroutine

			// Keep the current in the stack
			_stack[_SP] = _PC;
			_SP++;
			// Move to the subroutine
			_PC = _nnn();

			break;
		}

		case 0x3000: {	// Conditionnal skip of the next instruction if equal

			if (_V[_x()] == _nn())
				_PC += 2;

			_PC += 2;

			break;
		}

		case 0x4000: {	// Conditionnal skip of the next instruction if not equal

			if (_V[_x()] != _nn())
				_PC += 2;

			_PC += 2;

			break;
		}

		case 0x5000: {	// Conditionnal skip of the next instruction if registers are equal

			if (_V[_x()] == _V[_y()])
				_PC += 2;

			_PC += 2;

			break;
		}

		case 0x9000: {	// Conditionnal skip of the next instruction if registers are not equal

			if (_V[_x()] != _V[_y()])
				_PC += 2;

			_PC += 2;

			break;
		}

		case 0x6000: {	// Store a value in a specific register

			_V[_x()] = _nn();
			_PC += 2;

			break;
		}

		case 0x7000: {	// Addition without carry

			_V[_x()] += _nn();

			_PC += 2;

			break;
		}

		case 0x8000: {	// Register arithmetic

			switch (_opcode & 0x800F) {

			case 0x8000: {	// Assignation

				_V[_x()] = _V[_y()];

				_PC += 2;

				break;
			}

			case 0x8001: {	// Bitwise OR

				_V[_x()] |= _V[_y()];

				_V[0xF] = 0;

				_PC += 2;

				break;
			}

			case 0x8002: {	// Bitwise AND

				_V[_x()] &= _V[_y()];

				_V[0xF] = 0;
				_PC += 2;

				break;
			}

			case 0x8003: {	// Bitwise XOR

				_V[_x()] ^= _V[_y()];

				_V[0xF] = 0;
				_PC += 2;

				break;
			}

			case 0x8004: {	// Addition

				uint16_t sum = _V[_x()] + _V[_y()];

				// Detection for overflow
				uint8_t overflow = (sum > 0xFF);

				_V[_x()] = sum;

				_V[0xF] = overflow;

				_PC += 2;

				break;
			}

			case 0x8005: {	// Substraction

				// Detection for underflow (0 when there is)
				uint8_t underflow = (_V[_y()] <= _V[_x()]);

				_V[_x()] = _V[_x()] - _V[_y()];

				_V[0xF] = underflow;

				_PC += 2;

				break;
			}

			case 0x8006: {	// Right shift, stores extra bit

				_V[_x()] = _V[_y()]; // Hold quirk

				// Extra crushed bit
				uint8_t carry = _V[_x()] & 0x1;

				_V[_x()] >>= 1;

				_V[0xF] = carry;

				_PC += 2;

				break;
			}

			case 0x8007: {	// Inverse substraction

				// Detection for underflow (0 when there is)
				uint8_t underflow = (_V[_x()] <= _V[_y()]);

				_V[_x()] = _V[_y()] - _V[_x()];

				_V[0xF] = underflow;

				_PC += 2;

				break;
			}

			case 0x800E: {	// Left shift, stores extra bit

				_V[_x()] = _V[_y()]; // Hold quirk

				// Extra crushed bit
				uint8_t carry = (_V[_x()] & 0x80) >> 7;

				_V[_x()] <<= 1;

				_V[0xF] = carry;

				_PC += 2;

				break;
			}

			default: {
				std::cerr << "Unknown Opcode encountered: " << std::hex << _opcode << std::endl;
			}
			}

			break;
		}

		case 0xA000: {	// Store a value in the address register

			_I = _nnn();
			_PC += 2;

			break;
		}

		case 0xB000: {	// Jump to location closed to the one in register 0

			_PC = _V[0] + _nnn();

			break;
		}

		case 0xC000: {	// Set a register to a random value

			// Random number between 0 and 255.
			uint8_t random = rand() % 0x100;

			_V[_x()] = random & _nn();

			_PC += 2;

			break;
		}

		case 0xD000: { // Draw a sprite at a specific location : TODO

			_V[0xF] = 0;

			for (int i = 0; i < _n(); i++) {

				// Get the row of pixels to render
				uint8_t row = _memory[_I + i];

				for (int j = 0; j < 8; j++) {

					// If bit will change the pixel
					if (row & (1 << (7 - j))) {
						_V[0xF] |= _visuals.setPixel((_V[_x()] % CHIP8_SCREEN_W) + j,
							(_V[_y()] % CHIP8_SCREEN_H) + i);
					}
				}
			}

			_PC += 2;

			_draw_flag = true;

			break;
		}

		case 0xE000: {	// Key press operations

			switch (_opcode & 0xE0FF) {

			case 0xE09E: { // Skip next instruction if key is pressed

				if (_inputs.isPressed(_V[_x()])) _PC += 2;

				_PC += 2;

				break;
			}

			case 0xE0A1: { // Skip next instruction if key is not pressed

				if (!_inputs.isPressed(_V[_x()])) _PC += 2;

				_PC += 2;

				break;
			}

			default: {
				std::cerr << "Unknown Opcode encountered: " << std::hex << _opcode << std::endl;
			}
			}

			break;
		}

		case 0xF000: { // Miscellanious operations

			switch (_opcode & 0xF0FF) {

			case 0xF007: {	// Getter for the delay timer

				_V[_x()] = _delay_timer;

				_PC += 2;

				break;
			}

			case 0xF00A: {	// Await key press

				if (!_wait_flag) {
					_inputs.wait();
					_wait_flag = true;
				}
				// Received a new input
				else if (!_inputs.isWaiting()) {
					_V[_x()] = _inputs.getWaited();

					_PC += 2;
				}

				break;
			}

			case 0xF015: {	// Delay timer setter

				_delay_timer = _V[_x()];

				_PC += 2;

				break;
			}

			case 0xF018: {	// Sound timer setter

				_audio_timer = _V[_x()];

				_PC += 2;

				break;

			}

			case 0xF01E: {	// Increment the address register

				_I += _V[_x()];

				_PC += 2;

				break;

			}

			case 0xF029: {	// Store sprite data

				// Location of hex character # in memory is 10 * #
				_I = 5 * _V[_x()];

				_PC += 2;

				break;
			}

			case 0xF033: { // Stores the BCD representation of a value

				uint16_t val = _V[_x()];

				_memory[_I] = val / 100;			// Hundreds
				_memory[_I + 1] = (val / 10) % 10;	// Tens
				_memory[_I + 2] = val % 10;			// Ones

				_PC += 2;

				break;
			}

			case 0xF055: {	// Store registers 0 to x in memory

				for (uint8_t i = 0; i < _x() + 1; i++) {

					_memory[_I + i] = _V[i];
				}

				_I += _x() + 1; // Simulate the old hardware

				_PC += 2;

				break;
			}

			case 0xF065: {	// Load registers 0 to x from memory

				for (uint8_t i = 0; i < _x() + 1; i++) {

					_V[i] = _memory[_I + i];
				}

				_I += _x() + 1; // Simulate the old hardware

				_PC += 2;

				break;
			}

			default: {
				std::cerr << "Unknown Opcode encountered: " << std::hex << _opcode << std::endl;
			}
			}

			break;
		}

		default: {

			std::cerr << "Unknown Opcode encountered: " << std::hex << _opcode << std::endl;
		}

		}

		// Update the timers
		if (_delay_timer > 0) _delay_timer--;
		if (_audio_timer > 0) _audio_timer--;

		// Update the audio timer
		if (_audio_timer > 0) {
			Audio::playFrequency(440);
		}
		else if (_audio_timer == 0) {
			Audio::stop();
		}
	}
}