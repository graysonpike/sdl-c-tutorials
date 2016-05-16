// Key press example

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define WIDTH 640
#define HEIGHT 480

enum {
	SURFACE_DEFAULT,
	SURFACE_UP,
	SURFACE_DOWN,
	SURFACE_LEFT,
	SURFACE_RIGHT,
	NUM_SURFACES
};

SDL_Window *window;
SDL_Surface *window_surface;
SDL_Surface *current_surface;
SDL_Surface *keyPressSurfaces[NUM_SURFACES];

// Load an image to a specified surface
bool load_image(SDL_Surface **surface, char *filename) {
	char filepath[strlen(filename + 5)];
	strcpy(filepath, "res/");
	strcat(filepath, filename);
	*surface = SDL_LoadBMP(filepath);
	if(surface == NULL) {
		printf("Error loading image: %s\n", SDL_GetError());
		return false;
	}
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
	return true;
}

// Load resources
bool load_res() {
	load_image(&keyPressSurfaces[0], "press.bmp");
	load_image(&keyPressSurfaces[1], "up.bmp");
	load_image(&keyPressSurfaces[2], "down.bmp");
	load_image(&keyPressSurfaces[3], "left.bmp");
	load_image(&keyPressSurfaces[4], "right.bmp");
	return true;
}

// Free resources and shut down SDL
void quit() {
	// Destroy window and assoicated surface
	SDL_DestroyWindow(window);
	for(int i = 0; i < NUM_SURFACES; i++) {
		SDL_FreeSurface(keyPressSurfaces[i]);
	}
	SDL_Quit();
}

int main() {

	init();
	load_res();

	bool loop = true;
	SDL_Event event;
	window_surface = SDL_GetWindowSurface(window);
	current_surface = keyPressSurfaces[SURFACE_DEFAULT];
	SDL_FillRect(window_surface, NULL, SDL_MapRGB(window_surface->format, 255, 255, 255));

	while(loop) {
		while(SDL_PollEvent(&event) != 0) {
			// If the 'X' button is pressed
			if(event.type == SDL_QUIT) {
				loop = false;
			}
			// If a key is pressed
			if(event.type == SDL_KEYDOWN) {
				switch(event.key.keysym.sym) {
					case SDLK_UP:
						current_surface = keyPressSurfaces[SURFACE_UP];
						break;
					case SDLK_DOWN:
						current_surface = keyPressSurfaces[SURFACE_DOWN];
						break;
					case SDLK_LEFT:
						current_surface = keyPressSurfaces[SURFACE_LEFT];
						break;
					case SDLK_RIGHT:
						current_surface = keyPressSurfaces[SURFACE_RIGHT];
						break;
					case SDLK_ESCAPE:
						loop = false;
						break;
					default:
						current_surface = keyPressSurfaces[SURFACE_DEFAULT];
						break;
				}
			}
		}
		// Blit current image to window surface
		SDL_BlitSurface(current_surface, NULL, window_surface, NULL);
		// Refresh window buffer
		SDL_UpdateWindowSurface(window);
	}

	quit();
	
	return 0;
}
