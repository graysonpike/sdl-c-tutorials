// Program to create a blank window

#include <stdio.h>
#include <SDL2/SDL.h>

#define WIDTH 640
#define HEIGHT 480

int main(int argc, char *argv[]) {

	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error: Failed to init SDL2: %s\n", SDL_GetError());
    }

	SDL_Window *window;
    SDL_Surface *surface;

	window = SDL_CreateWindow(
        "SDL2 Test",                  // window title
        SDL_WINDOWPOS_UNDEFINED,      // initial x position
        SDL_WINDOWPOS_UNDEFINED,      // initial y position
        WIDTH,                        // width, in pixels
        HEIGHT,                       // height, in pixels
        SDL_WINDOW_OPENGL             // flags - see below
    );

    if (window == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    surface = SDL_GetWindowSurface(window);

    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0, 0, 255));

    SDL_UpdateWindowSurface(window);

    SDL_Delay(3000);

    SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}