
#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define GAME_DEFAULT_SCREEN_WIDTH 800
#define GAME_DEFAULT_SCREEN_HEIGHT 600
#define GAME_DEFAULT_FPS (1000/60)

typedef struct SDLWindowState {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event e;
    TTF_Font *font;
    int active;
} SDLWindowState;

SDL_Color color_white = { 255, 255, 255, 255 };
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
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    if (!s->window)
        return -1;

    s->renderer = SDL_CreateRenderer(
        s->window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!s->renderer)
        return -1;

    SDL_SetRenderDrawColor( s->renderer, 0x00, 0x00, 0x00, 0xFF);

    if ((ret = TTF_Init()) < 0)
        return ret;

    s->font = TTF_OpenFont("font.otb", 14);

    if (!s->font)
        return -1;

    s->active = 1;

    return 0;
}

void media_sdl_print_error()
{
    fprintf(stderr, "SDL_Error: %s\n", SDL_GetError());
}

SDL_Texture *media_sdl_surface_to_texture(SDLWindowState *s, SDL_Surface *sf)
{
    SDL_Texture *t = NULL;
    t = SDL_CreateTextureFromSurface(s->renderer, sf);
    return t;
}

void media_sdl_close(SDLWindowState *s)
{
    SDL_DestroyRenderer(s->renderer);
    SDL_DestroyWindow(s->window);
    SDL_Quit();
}

int main(int argc, char **argv)
{
    SDLWindowState s;
    uint64_t fps_start, fps_end, fps_elapsed;
    SDL_Rect k;
    char str[4096];
    float fps;
    if (media_sdl_init(&s, "Hello") < 0) {
        media_sdl_print_error();
        return 1;
    }
    
    SDL_Surface *hello;
    SDL_Texture *hello_txr;
    hello = SDL_LoadBMP("quote.bmp");
    hello_txr = media_sdl_surface_to_texture(&s, hello);
    if (!hello_txr) {
        printf("Failed to make texture");
        return 1;
    }

    while (s.active) {
        fps_start = SDL_GetTicks();

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
        SDL_RenderClear(s.renderer);
            sprintf(str, "FPS: %f", fps);
            SDL_Surface *t = TTF_RenderText_Solid(s.font, str, color_white);
            SDL_GetClipRect(t, &k);
            SDL_Texture *ttx = SDL_CreateTextureFromSurface(s.renderer, t);
            SDL_RenderCopy(s.renderer, ttx, NULL, &k);
        SDL_RenderPresent(s.renderer);

        fps_end = SDL_GetTicks();
        fps_elapsed = fps_end - fps_start;

        if (fps_elapsed < GAME_DEFAULT_FPS)
            SDL_Delay(GAME_DEFAULT_FPS - fps_elapsed);

        fps = 1000 / (float) (SDL_GetTicks() - fps_start);

        // printf("fps_start = %ld fps_end = %ld, fps = %.4f\n", fps_start, fps_end, fps);
    }

    SDL_FreeSurface(hello);
    media_sdl_close(&s);
    return 0;
}
