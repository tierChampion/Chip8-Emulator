
#include "octo_chip.h"

using namespace ch8;

int main(int argc, char* argv[])
{

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {

		std::cerr << "Couldn't initialize SDL: " << SDL_GetError() << std::endl;
		exit(1);
	}

	Cpu cpu;

	// Name of the rom to load. Default is PONG
	std::string rom = "PONG";

	if (argc > 1) {
		rom = std::string(argv[1]);
	}

	if (!cpu.loadProgram((std::string(DIRECTORY_PATH) + std::string("\\roms\\") + rom).c_str())) {
		return -1;
	}

	SDL_Event event;
	bool quit = false;

	while (!quit) {

		cpu.cycle();

		if (cpu.needToDraw()) {
			cpu._visuals.render();
		}

		// Poll the events
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT: quit = true; break;
			case SDL_KEYDOWN: cpu._inputs.onKeyDown(&event.key); break;
			case SDL_KEYUP: cpu._inputs.onKeyUp(&event.key); break;
			default: break;
			}
		}
	}

	Audio::closeAudio();
	SDL_Quit();

	return 0;
}
