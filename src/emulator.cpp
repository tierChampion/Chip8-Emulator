#include <SDL/SDL.h>
#include <iostream>

#include "cpu.h"
#include "keyboard.h"

using namespace ch8;

int main(int argc, char* argv[]) {

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {

		std::cerr << "Couldn't initialize SDL: " << SDL_GetError() << std::endl;
		exit(1);
	}

	SDL_Window* window = SDL_CreateWindow("Chip8 emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1080, 720, 0);

	Cpu cpu;

	bool res = cpu.loadProgram("roms/PONG");

	if (!res) return -1;

	cpu.test_cycle(0xC000);

	Keyboard k;

	SDL_Event event;

	bool quit = false;

	while (!quit) {

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT: quit = true; break;
			case SDL_KEYDOWN: k.onKeyDown(&event.key); break;
			case SDL_KEYUP: k.onKeyUp(&event.key); break;
			default: break;
			}
		}
	}

	//// Game Loop ////

	// init graphics, keybaord, cpu

	// load game and sprites or whatever

	// loop

		// emulate a cycle
		// render if needed
		// store key press

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}