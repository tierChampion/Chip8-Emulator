// OctoChip.cpp : Defines the entry point for the application.
//

#include "octo_chip.h"

using namespace ch8;

std::string getProjectDirectory(std::string exe_path) {

#ifdef __DEBUG__
	return exe_path.substr(0, exe_path.length() - std::string("out\\build\\x##-Debug\\bin\\OctoChip.exe").length());
#else
	return exe_path.substr(0, exe_path.length() - std::string("out\\build\\x##-Release\\bin\\OctoChip.exe").length());
#endif
}

int main(int argc, char* argv[])
{
	// TODO:
		// - More error detection for SDL calls

	std::string project_path = getProjectDirectory(argv[0]);

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {

		std::cerr << "Couldn't initialize SDL: " << SDL_GetError() << std::endl;
		exit(1);
	}

	Cpu cpu;

	bool res = cpu.loadProgram((project_path + std::string("\\roms\\PONG")).c_str());

	if (!res) return -1;

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
