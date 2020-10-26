#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define GAME_DEFAULT_SCREEN_WIDTH 800
#define GAME_DEFAULT_SCREEN_HEIGHT 600

int main(int argc, char **argv)
{
    // The window we'll be rendering to
    SDL_Window *window = NULL;
    
    // The surface contained by the window
    SDL_Surface *screen_surface = NULL;
    SDL_Surface *hello = NULL;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    window = SDL_CreateWindow (
        "Sup",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        GAME_DEFAULT_SCREEN_WIDTH,
        GAME_DEFAULT_SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        printf("SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    screen_surface = SDL_GetWindowSurface(window);
    hello = SDL_LoadBMP("quote.bmp");
    SDL_FillRect(screen_surface, NULL, SDL_MapRGB(screen_surface->format, 0x00, 0x00, 0x00));
    SDL_BlitSurface(hello, NULL, screen_surface, NULL);
    SDL_UpdateWindowSurface(window);
    
    // Wait two seconds
    SDL_Delay( 2000 );

    //Destroy window
    SDL_DestroyWindow(window);

    //Quit SDL subsystems
    SDL_Quit();
    return 0;
}
