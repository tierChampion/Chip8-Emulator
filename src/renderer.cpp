#include "renderer.h"

namespace ch8 {

	Renderer::Renderer(uint8_t scale) {

		_window = SDL_CreateWindow("Program", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1080, 720, 0);
		_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);

		//_texture = SDL_CreateTexture(_renderer, )

		//todo...
	}

	Renderer::~Renderer() {

	}

	void Renderer::clearScreen() {}

	bool Renderer::setPixel(uint8_t x, uint8_t y) { return false; }

	void render() {}

}