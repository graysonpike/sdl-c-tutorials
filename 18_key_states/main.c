// Mouse handling using the SDL event queue
// Mouse over one of the 4 panels to make it green
// Clicking on a panel will make it light green

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WIDTH 640
#define HEIGHT 480

enum {
	TEXTURE_DEFAULT,
	TEXTURE_UP,
	TEXTURE_DOWN,
	TEXTURE_LEFT,
	TEXTURE_RIGHT,
	NUM_SURFACES
};

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *key_press_textures[NUM_SURFACES];

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
	SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGBA(loaded_surface->format, 255, 255, 255, 255));
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
	load_texture(&key_press_textures[TEXTURE_DEFAULT], "press.bmp", false);
	load_texture(&key_press_textures[TEXTURE_UP], "up.bmp", false);
	load_texture(&key_press_textures[TEXTURE_DOWN], "down.bmp", false);
	load_texture(&key_press_textures[TEXTURE_LEFT], "left.bmp", false);
	load_texture(&key_press_textures[TEXTURE_RIGHT], "right.bmp", false);
}

// Free resources and shut down SDL
void quit() {
	// Destroy window and assoicated surface
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	for(int i = 0; i < NUM_SURFACES; i++) {
		SDL_DestroyTexture(key_press_textures[i]);
	}
	IMG_Quit();
	SDL_Quit();
}

int main(){

	init();
	load_res();

	SDL_Event event;
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	
	bool loop = true;
	int current_texture = TEXTURE_DEFAULT;

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
			const Uint8 *key_states = SDL_GetKeyboardState(NULL);
			if(key_states[SDL_SCANCODE_UP]) {
				current_texture = TEXTURE_UP;
			} else if(key_states[SDL_SCANCODE_DOWN]) {
				current_texture = TEXTURE_DOWN;
			} else if(key_states[SDL_SCANCODE_LEFT]) {
				current_texture = TEXTURE_LEFT;
			} else if(key_states[SDL_SCANCODE_RIGHT]) {
				current_texture = TEXTURE_RIGHT;
			} else {
				current_texture = TEXTURE_DEFAULT;
			}
		}
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, key_press_textures[current_texture], NULL, NULL);
		SDL_RenderPresent(renderer);

	}

	quit();

	return 0;
}
