#pragma once

#include <SDL/SDL.h>
#include <cstdint>

namespace ch8 {

#define CHIP8_SCREEN_W 16
#define CHIP8_SCREEN_H 8

	/*
	Visual system of the Chip8
	*/
	class Renderer {

		uint8_t			_screen[CHIP8_SCREEN_W * CHIP8_SCREEN_H];	// Screen raster
		uint32_t		_frame_start;								// Start of the last frame

		SDL_Renderer*	_renderer;									// Rendering object
		SDL_Window*		_window;									// Window object
		SDL_Texture*	_texture;									// Screen texture to render

	public:

		Renderer(uint8_t scale);

		~Renderer();

		void clearScreen();
		bool setPixel(uint8_t x, uint8_t y);
		void render();

	};

}
