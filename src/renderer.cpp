#include "renderer.h"

namespace ch8 {

	Renderer::Renderer() {

		_window = SDL_CreateWindow("Chip8 emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 320, 0);
		_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);

		_texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGB332, SDL_TEXTUREACCESS_STREAMING,
			CHIP8_SCREEN_W, CHIP8_SCREEN_H);

		int pad;

		SDL_LockTexture(_texture, NULL, (void**)&_screen, &pad);

		clearScreen();
	}


	Renderer::~Renderer() {

		SDL_DestroyTexture(_texture);
		SDL_DestroyRenderer(_renderer);
		SDL_DestroyWindow(_window);
	}

	/**
	* Set all the pixels on the screen to black.
	*/
	void Renderer::clearScreen() {

		for (int i = 0; i < CHIP8_SCREEN_W * CHIP8_SCREEN_H; i++) {
			_screen[i] = 0x00;
		}
	}

	/**
	* Change the value of the pixel at the desired location.
	* @param x - Horizontal position of the pixel
	* @param y - Vertical position of the pixel
	*/
	uint8_t Renderer::setPixel(uint8_t x, uint8_t y) {

		// Check if within bounds
		if ((int)x >= CHIP8_SCREEN_W || (int)y >= CHIP8_SCREEN_H) {
			return 0x00;
		}

		// XOR the new pixel
		_screen[y * CHIP8_SCREEN_W + x] ^= 0xFF;

		// Return if the pixel was turned off
		return (uint8_t)(_screen[y * CHIP8_SCREEN_W + x] == 0x0);
	}

	/**
	* Display the screen on the SDL window.
	*/
	void Renderer::render() {

		SDL_RenderClear(_renderer);

		SDL_UnlockTexture(_texture);

		SDL_RenderCopy(_renderer, _texture, NULL, NULL);

		SDL_RenderPresent(_renderer);

		int pad;

		SDL_LockTexture(_texture, NULL, (void**)&_screen, &pad);
	}
}