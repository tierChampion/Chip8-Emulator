#pragma once

#include <SDL/SDL.h>
#include <cstdint>
#include <iostream>

namespace ch8 {

#define CHIP8_SCREEN_W 64
#define CHIP8_SCREEN_H 32

	/*
	Visual system of the Chip8
	*/
	class Renderer {

		uint8_t*		_screen;		// Screen raster

		SDL_Renderer*	_renderer;		// Rendering object
		SDL_Window*		_window;		// Window object
		SDL_Texture*	_texture;		// Screen texture to render

	public:

		Renderer();

		~Renderer();

		void clearScreen();
		bool setPixel(uint8_t x, uint8_t y);
		void render();

	};

}
