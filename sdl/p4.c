#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define GAME_DEFAULT_SCREEN_WIDTH 800
#define GAME_DEFAULT_SCREEN_HEIGHT 600



typedef struct SDLWindowState {
    SDL_Window *window;
    SDL_Surface *main_surface;
    SDL_Event e;
    int active;
} SDLWindowState;

/*
typedef struct MediaDriver {
    void *driver_state;
    
} MediaDriver;
*/

int media_sdl_init(SDLWindowState *s, char *window_name)
{
    int ret;

    if ((ret = SDL_Init(SDL_INIT_VIDEO)) < 0)
        return ret;

    s->window = SDL_CreateWindow(
        window_name,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        GAME_DEFAULT_SCREEN_WIDTH,
        GAME_DEFAULT_SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!s->window)
        return -1;

    s->main_surface = SDL_GetWindowSurface(s->window);

    s->active = 1;

    return 0;
}

void media_sdl_print_error() {
    fprintf(stderr, "SDL_Error: %s\n", SDL_GetError());
}

void media_sdl_close(SDLWindowState *s) {
    SDL_FreeSurface(s->main_surface);
    SDL_DestroyWindow(s->window);
    SDL_Quit();
}

int main(int argc, char **argv)
{
    SDLWindowState s;

    if (media_sdl_init(&s, "Hello") < 0) {
        media_sdl_print_error();
        return 1;
    }
    
    SDL_Surface *hello;
    hello = SDL_LoadBMP("quote.bmp");
    SDL_FillRect(s.main_surface, NULL, SDL_MapRGB(s.main_surface->format, 0x00, 0x00, 0x00));
    SDL_BlitSurface(hello, NULL, s.main_surface, NULL);
    SDL_UpdateWindowSurface(s.window);

    while (s.active) {
        while (SDL_PollEvent(&s.e)) {
            switch (s.e.type) {
            case SDL_QUIT:
                s.active = 0;
                break;

            case SDL_KEYDOWN:
                printf("%d\n", s.e.key.keysym.sym);
                break;
            }
        }
    }

    SDL_FreeSurface(hello);
    media_sdl_close(&s);
    return 0;
}
