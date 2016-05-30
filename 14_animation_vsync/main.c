// Example of an animated sprite using a sprite sheet with vsync
// Up = increase alpha mod
// Down = decrease alpha mod

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WIDTH 102
#define HEIGHT 148
#define NUM_FRAMES 6
#define FRAME_DELAY 8

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *walking_sheet = NULL;

SDL_Rect frames[NUM_FRAMES];

bool load_texture(SDL_Texture **texture, char *filename, bool color_key){
	*texture = NULL;
	SDL_Surface *loaded_surface = NULL;
	// Combline path and filename
	char filepath[strlen(filename + 5)];
	strcpy(filepath, "res/");
	strcat(filepath, filename);
	// Load surface and convert to texture
	loaded_surface = IMG_Load(filepath);
	if(loaded_surface == NULL) {
		printf("Error loading image: %s\n", IMG_GetError());
		return false;
	}
	SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGBA(loaded_surface->format, 255, 0, 255, 255));
	*texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
	if(*texture == NULL) {
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
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(renderer == NULL) {
		printf("Could not init renderer: %s\n", SDL_GetError());
		return false;
	}

	// Initialize SDL_image
	int img_flags = IMG_INIT_PNG | IMG_INIT_JPG;
	if(!(IMG_Init(img_flags) & img_flags)) {
		printf("SDL_image failed to init: %s\n", IMG_GetError());
		return false;
	}

	return true;
}

void load_res() {
	load_texture(&walking_sheet, "walking.bmp", true);
	for(int i = 0; i < NUM_FRAMES; i++) {
		frames[i].w = 102;
		frames[i].h = 148;
		frames[i].x = 102 * i;
		frames[i].y = 0;
	}
}

// Free resources and shut down SDL
void quit() {
	// Destroy window and assoicated surface
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyTexture(walking_sheet);
	IMG_Quit();
	SDL_Quit();
}

int main(){

	init();
	load_res();

	SDL_Event event;
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	
	bool loop = true;
	int frame_counter = 0;

	while(loop) {
		while(SDL_PollEvent(&event) != 0) {
			if(event.type == SDL_QUIT) {
				loop = false;
			}
			if(event.type == SDL_KEYDOWN) {
				switch(event.key.keysym.sym) {
					case SDLK_ESCAPE:
						loop = false;
						break;
					default:
						break;
				}
			}
		}
		frame_counter += 1;
		if(frame_counter / FRAME_DELAY >= NUM_FRAMES) {
			frame_counter = 0;
		}
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, walking_sheet, &frames[frame_counter / FRAME_DELAY], NULL);
		SDL_RenderPresent(renderer);

	}

	quit();

	return 0;
}
