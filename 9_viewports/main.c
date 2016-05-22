// Viewports example

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WIDTH 640
#define HEIGHT 480

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;
SDL_Rect top_left_viewport;
SDL_Rect top_right_viewport;
SDL_Rect bottom_viewport;

bool load_texture(SDL_Texture **texture, char *filename){
	*texture = NULL;
	SDL_Surface *loaded_surface = NULL;
	char filepath[strlen(filename + 5)];
	strcpy(filepath, "res/");
	strcat(filepath, filename);
	loaded_surface = IMG_Load(filepath);
	if(loaded_surface == NULL) {
		printf("Error loading image: %s\n", SDL_GetError());
		return false;
	}
	*texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
	if(texture == NULL) {
		printf("Unable to create texture from surface: %s\n", SDL_GetError());
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

	// Initialize Renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(renderer == NULL) {
		printf("Could not init renderer: %s\n", SDL_GetError());
		return false;
	}
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

	// Initialize SDL_image
	int img_flags = IMG_INIT_PNG | IMG_INIT_JPG;
	if(!(IMG_Init(img_flags) & img_flags)) {
		printf("SDL_image failed to init: %s\n", IMG_GetError());
		return false;
	}

	return true;
}

bool init_viewports() {

	top_left_viewport.x = 0;
	top_left_viewport.y = 0;
	top_left_viewport.w = WIDTH / 2;
	top_left_viewport.h = HEIGHT / 2;

	top_right_viewport.x = WIDTH / 2;
	top_right_viewport.y = 0;
	top_right_viewport.w = WIDTH / 2;
	top_right_viewport.h = HEIGHT / 2;

	bottom_viewport.x = 0;
	bottom_viewport.y = HEIGHT / 2;
	bottom_viewport.w = WIDTH;
	bottom_viewport.h = HEIGHT / 2;

	return true;
}

// Load resources
bool load_res() {
	load_texture(&texture, "ut_campus.jpg");
	return true;
}

// Free resources and shut down SDL
void quit() {
	// Destroy window and assoicated surface
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	IMG_Quit();
	SDL_Quit();
}

int main() {

	init();
	load_res();
	init_viewports();

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	SDL_RenderSetViewport(renderer, &top_left_viewport);
	SDL_RenderCopy(renderer, texture, NULL, NULL);

	SDL_RenderSetViewport(renderer, &top_right_viewport);
	SDL_RenderCopy(renderer, texture, NULL, NULL);

	SDL_RenderSetViewport(renderer, &bottom_viewport);
	SDL_RenderCopy(renderer, texture, NULL, NULL);

	SDL_RenderPresent(renderer);

	SDL_Delay(3000);

	quit();

	return 0;
}