// Rendering geometry

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define WIDTH 640
#define HEIGHT 480

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

// Initialize SDL (return true if successful, false otherwise)
bool init() {

	// Initialize SDL_video
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Error: Failed to init SDL2: %s\n", SDL_GetError());
		return false;
	}
	window = SDL_CreateWindow(
		"SDL2 Keypress Test",         // window title
		SDL_WINDOWPOS_UNDEFINED,      // initial x position
		SDL_WINDOWPOS_UNDEFINED,      // initial y position
		WIDTH,                        // width, in pixels
		HEIGHT,                       // height, in pixels
		SDL_WINDOW_OPENGL             // flags - see below
	);
	if (window == NULL) {
		printf("Could not create window: %s\n", SDL_GetError());
		return false;
	}

	// Initialize Renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(renderer == NULL) {
		printf("Could not init renderer: %s\n", SDL_GetError());
		return false;
	}
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	return true;
}

void quit() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}

int main() {

	init();

	// Clear screen
	SDL_RenderClear(renderer);

	// Render filled quad
	SDL_Rect fill_rect = {WIDTH/4.0, HEIGHT/4.0, WIDTH/2.0, HEIGHT/2.0};
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
	SDL_RenderFillRect(renderer, &fill_rect);

	// Render outlined quad
	SDL_Rect outline_rect = {WIDTH/6.0, HEIGHT/6.0, WIDTH * 2.0 / 3.0, HEIGHT * 2.0 / 3.0};
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderDrawRect(renderer, &outline_rect);

	// Render diagonal line
	SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
	SDL_RenderDrawLine(renderer, 0, 0, WIDTH, HEIGHT);

	// Render a dotted line (many points)
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	for(int i = 0; i < WIDTH; i += 4){
		SDL_RenderDrawPoint(renderer, i, HEIGHT / 2.0);
	}

	SDL_RenderPresent(renderer);

	SDL_Delay(3000);

	quit();

	return 0;
}