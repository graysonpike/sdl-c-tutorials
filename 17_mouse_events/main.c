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
#define IMG_WIDTH 320
#define IMG_HEIGHT 320

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *green = NULL;
SDL_Texture *light_green = NULL;
SDL_Texture *red = NULL;
SDL_Rect panels[4];

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
	load_texture(&green, "green.bmp", true);
	load_texture(&light_green, "light_green.bmp", true);
	load_texture(&red, "red.bmp", true);
	for(int i = 0; i < 4; i++) {
		panels[i].w = 320;
		panels[i].h = 240;
	}
	panels[0].x = 0;
	panels[0].y = 0;
	panels[1].x = 320;
	panels[1].y = 0;
	panels[2].x = 0;
	panels[2].y = 240;
	panels[3].x = 320;
	panels[3].y = 240;
}

bool collide_point_rect(int x, int y, SDL_Rect *rect) {
	if(x >= rect->x && x <= rect->x + rect->w) {
		if(y >= rect->y && y <= rect->y + rect->h) {
			return true;
		}
	}
	return false;
}

// Free resources and shut down SDL
void quit() {
	// Destroy window and assoicated surface
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyTexture(green);
	SDL_DestroyTexture(light_green);
	SDL_DestroyTexture(red);
	IMG_Quit();
	SDL_Quit();
}

int main(){

	init();
	load_res();

	SDL_Event event;
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	
	bool loop = true;
	int mouse_x = -1, mouse_y = -1;
	bool mouse_down = false;

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
			if(event.type == SDL_MOUSEMOTION) {
				SDL_GetMouseState(&mouse_x, &mouse_y);
			}
			if(event.type == SDL_MOUSEBUTTONDOWN) {
				mouse_down = true;
			}
			if(event.type == SDL_MOUSEBUTTONUP) {
				mouse_down = false;
			}
		}
		SDL_RenderClear(renderer);
		for(int i = 0; i < 4; i++) {
			if(collide_point_rect(mouse_x, mouse_y, &panels[i])) {
				if(mouse_down) {
					SDL_RenderCopy(renderer, light_green, NULL, &panels[i]);
				} else {
					SDL_RenderCopy(renderer, green, NULL, &panels[i]);
				}
			} else {
				SDL_RenderCopy(renderer, red, NULL, &panels[i]);
			}
		}
		SDL_RenderPresent(renderer);

	}

	quit();

	return 0;
}
