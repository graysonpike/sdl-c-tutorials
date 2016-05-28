// Color modulation example
// Space to toggle color modulation, esc to quit
// Q-A = More-Less Red
// W-S = More-Less Green
// E-D = More-Less Blue

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WIDTH 600
#define HEIGHT 600

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *color_wheel = NULL;

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
	SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGBA(loaded_surface->format, 0, 255, 255, 255));
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
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
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
	load_texture(&color_wheel, "color_wheel.png", false);
}

// Free resources and shut down SDL
void quit() {
	// Destroy window and assoicated surface
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyTexture(color_wheel);
	IMG_Quit();
	SDL_Quit();
}

// Clamp int to 0-255 range
Uint8 clamp(int x) {
	if(x < 0) {
		return 0;
	} else if(x > 255) {
		return 255;
	} else {
		return x;
	}
}

int main(){

	init();
	load_res();

	SDL_Event event;

	Uint8 red = 255, green = 255, blue = 255;
	bool color_on = true;

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, color_wheel, NULL, NULL);
	SDL_RenderPresent(renderer);

	bool loop = true;
	while(loop) {
		while(SDL_PollEvent(&event) != 0) {
			if(event.type == SDL_QUIT) {
				loop = false;
			}
			if(event.type == SDL_KEYDOWN) {
				switch(event.key.keysym.sym) {
					case SDLK_SPACE:
						color_on = !color_on;
						break;
					case SDLK_q:
						red += 32;
						break;
					case SDLK_a:
						red -= 32;
						break;
					case SDLK_w:
						green += 32;
						break;
					case SDLK_s:
						green -= 32;
						break;
					case SDLK_e:
						blue += 32;
						break;
					case SDLK_d:
						blue -= 32;
						break;
					case SDLK_r:
						red = 255;
						green = 255;
						blue = 255;
						break;
					case SDLK_ESCAPE:
						loop = false;
					default:
						break;
				}
				red = clamp(red);
				green = clamp(green);
				blue = clamp(blue);
				if(color_on) {
					SDL_SetTextureColorMod(color_wheel, red, green, blue);
					printf("< R%03d G%03d B%03d >\n", red, green, blue);
				} else {
					SDL_SetTextureColorMod(color_wheel, 255, 255, 255);
					printf("< R255 G255 B255 >\n");
				}
			}
		}
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, color_wheel, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	quit();

	return 0;
}
