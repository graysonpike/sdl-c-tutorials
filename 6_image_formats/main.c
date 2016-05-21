// Example of extenstion libraries and other image formats
// PNG transparency doesn't work for some reason I think

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WIDTH 640
#define HEIGHT 480

SDL_Window *window = NULL;
SDL_Surface *window_surface = NULL;
SDL_Surface *logo_surface = NULL;

// Load an image to a specified surface (must only be called after window_surface is initialized)
bool load_optimized_png_image(SDL_Surface **surface, char *filename) {
	// Load image from png file
	SDL_Surface *loaded_surface = NULL;
	char filepath[strlen(filename + 5)];
	strcpy(filepath, "res/");
	strcat(filepath, filename);
	loaded_surface = IMG_Load(filepath);
	if(loaded_surface == NULL) {
		printf("Error loading image: %s\n", IMG_GetError());
		return false;
	}
	// Optimize image
	*surface = SDL_ConvertSurface(loaded_surface, window_surface->format, 0);
	if(*surface == NULL) {
		printf("Error optimizing image %s: %s\n", filepath, SDL_GetError());
	}
	SDL_FreeSurface(loaded_surface);
	return true;
}

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
	window_surface = SDL_GetWindowSurface(window);

	// Initialize SDL_image
	int img_flags = IMG_INIT_PNG;
	if(!(IMG_Init(img_flags) & img_flags)) {
		printf("SDL_image failed to init: %s\n", IMG_GetError());
		return false;
	}

	return true;
}

// Load resources
bool load_res() {
	load_optimized_png_image(&logo_surface, "ut_logo.png");
	return true;
}

// Free resources and shut down SDL
void quit() {
	SDL_DestroyWindow(window);
	SDL_FreeSurface(logo_surface);
	SDL_Quit();
}

// Utility function to blit a surface to another surface at a given coord offset
void apply_surface(int x, int y, SDL_Surface *src, SDL_Surface *dest) {
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	SDL_BlitSurface(src, NULL, dest, &offset);
}

int main() {

	init();
	load_res();

	SDL_FillRect(window_surface, NULL, SDL_MapRGB(window_surface->format, 50, 50, 50));
	// Drawing the two images over each other to demonstrate that PNG transparency doesn't work here
	apply_surface(0, 0, logo_surface, window_surface);
	apply_surface(50, 50, logo_surface, window_surface);

	SDL_UpdateWindowSurface(window);

	SDL_Delay(3000);

	quit();

	return 0;
}