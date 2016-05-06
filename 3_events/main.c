// Program to display a bitmap image until a window close
// event is triggered.

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define WIDTH 640
#define HEIGHT 480

SDL_Window *window;
SDL_Surface *window_surface;
SDL_Surface *hello_surface;

// Initialize SDL (return true if successful, false otherwise)
bool init() {
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Error: Failed to init SDL2: %s\n", SDL_GetError());
		return false;
	}
	window = SDL_CreateWindow(
		"SDL2 Image Test",                  // window title
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
	return true;
}

// Load resources
bool load_res() {
	// Load bitmap to hello_surface Surface
	hello_surface = SDL_LoadBMP("res/contra.bmp");
	if(hello_surface == NULL) {
		printf("Error loading image: %s\n", SDL_GetError());
		return false;
	}
	return true;
}

// Free resources and shut down SDL
void quit() {
	// Destroy image surface
	SDL_FreeSurface(hello_surface);
	// Destroy window and assoicated surface
	SDL_DestroyWindow(window);
	SDL_Quit();
}

// Utility function to blit a surface to another surface at a given coord offset
void apply_surface(int x, int y, SDL_Surface *src, SDL_Surface *dest) {
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	SDL_BlitSurface(src, NULL, dest, &offset);
}

int main(int argc, char *argv[]) {

	init();
	load_res();
	window_surface = SDL_GetWindowSurface(window);

	// Fill window surface with white
	SDL_FillRect(window_surface, NULL, SDL_MapRGB(window_surface->format, 255, 255, 255));
	// Blit image to window surface
	apply_surface(100, 100, hello_surface, window_surface);

	SDL_UpdateWindowSurface(window);

	SDL_Event event;
	bool loop = true;
	while(loop) {
		// For each event in the queue
		while(SDL_PollEvent(&event) != 0) {
			if(event.type == SDL_QUIT) {
				loop = false;
			}
		}
	}

	quit();

	return 0;
}