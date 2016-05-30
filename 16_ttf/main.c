// Exampe of TTF font rendering in SDL2

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define WIDTH 500
#define HEIGHT 500

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *font_texture = NULL;
TTF_Font *font = NULL;
int font_width, font_height;

bool load_font_texture(SDL_Texture **texture, TTF_Font *font, char *text, SDL_Color text_color){
	*texture = NULL;
	SDL_Surface *loaded_surface = NULL;
	// Load surface and convert to texture
	// TTF_RenderText_Solid = quick & dirty
	// TTF_RenderText_Shaded = slow & antialiased, but with opaque box
	// TTF_RenderText_Blended = very slow & antialiased with alpha blending
	loaded_surface = TTF_RenderText_Blended(font, text, text_color);
	if(loaded_surface == NULL) {
		printf("Error loading font file: %s\n", TTF_GetError());
		return false;
	}
	*texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
	if(*texture == NULL) {
		printf("Unable to create texture from surface: %s\n", SDL_GetError());
	}
	font_width = loaded_surface->w;
	font_height = loaded_surface->h;
	SDL_FreeSurface(loaded_surface);
	return true;
}

bool load_font(TTF_Font **font, char *filename, int size) {
	// Combline path and filename
	char filepath[strlen(filename) + 5];
	strcpy(filepath, "res/");
	strcat(filepath, filename);
	*font = TTF_OpenFont(filepath, size);
	if(font == NULL) {
		printf("Error loading font: %s\n", TTF_GetError());
		return false;
	}
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

	if(TTF_Init() == -1) {
		printf("SDL_ttf failed to initialize: %s\n", TTF_GetError());
		return false;
	}

	return true;
}

void load_res() {
	load_font(&font, "open-sans/OpenSans-Regular.ttf", 28);
	SDL_Color color = {0, 0, 0, 255};
	load_font_texture(&font_texture, font, "Hello World", color); 
}

// Free resources and shut down SDL
void quit() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyTexture(font_texture);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
}

int main(){

	init();
	load_res();

	SDL_Event event;
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_Rect dst = {
		WIDTH/2 - font_width/2,
		HEIGHT/2 - font_height/2,
		font_width,
		font_height
	};
	
	bool loop = true;

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
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, font_texture, NULL, &dst);
		SDL_RenderPresent(renderer);

	}

	quit();

	return 0;
}
