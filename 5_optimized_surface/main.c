// Exmaple of optimized surfaces and soft streching surfaces

#include <stdbool.h>
#include <SDL2/SDL.h>

#define WIDTH 640
#define HEIGHT 480

SDL_Window *window = NULL;
SDL_Surface *window_surface = NULL;
SDL_Surface *contra_surface = NULL;

// Load an image to a specified surface (must only be called after window_surface is initialized)
bool load_optimized_image(SDL_Surface **surface, char *filename) {
	// Load image from bitmap file
	SDL_Surface *loaded_surface = NULL;
	char filepath[strlen(filename + 5)];
	strcpy(filepath, "res/");
	strcat(filepath, filename);
	loaded_surface = SDL_LoadBMP(filepath);
	if(loaded_surface == NULL) {
		printf("Error loading image: %s\n", SDL_GetError());
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
	return true;
}

// Load resources
bool load_res() {

	return true;
}

// Free resources and shut down SDL
void quit() {
	// Destroy window and assoicated surface
	SDL_DestroyWindow(window);
	SDL_FreeSurface(contra_surface);
	SDL_Quit();
}

int main() {

	init();
	load_res();

	// Display image (streched to screen width and height)
	SDL_Rect strech_rect;
	strech_rect.x = 0;
	strech_rect.y = 0;
	strech_rect.w = WIDTH;
	strech_rect.h = HEIGHT;
	SDL_BlitSurface(contra_surface, NULL, window_surface, NULL);

	//SDL_BlitScaled(contra_surface, NULL, window_surface, &strech_rect);
	//SDL_FillRect(window_surface, NULL, SDL_MapRGB(window_surface->format, 255, 255, 255));
	SDL_UpdateWindowSurface(window);

	SDL_Delay(3000);

	quit();

	return 0;
}