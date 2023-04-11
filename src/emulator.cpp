#include <SDL/SDL.h>
#include <iostream>

#include "cpu.h"
#include "keyboard.h"
#include "renderer.h"

using namespace ch8;

int main(int argc, char* argv[]) {

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {

		std::cerr << "Couldn't initialize SDL: " << SDL_GetError() << std::endl;
		exit(1);
	}

	Cpu cpu;

	bool res = cpu.loadProgram("roms/PONG");

	if (!res) return -1;

	Renderer r(0);

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

		if (k.isPressed(0x0)) std::cout << (r.setPixel(0, 0)) << std::endl;
		r.render();
	}

	//// Game Loop ////

	// init graphics, keybaord, cpu

	// load game and sprites or whatever

	// loop

		// emulate a cycle
		// render if needed
		// store key press

	SDL_Quit();

	return 0;
}