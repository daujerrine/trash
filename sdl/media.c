#include "media.h"

int m_init(MediaState *s, char *window_name, char *font_path)
{
    int ret;
    
    s->max_fps = (1000/60);
    s->main_w = 800;
    s->main_h = 600;

    if ((ret = SDL_Init(SDL_INIT_VIDEO)) < 0) {
        s->err_msg = SDL_GetError();
        return ret;
    }

    s->window = SDL_CreateWindow(
        window_name,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        s->main_w,
        s->main_h,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    if (!s->window) {
        s->err_msg = SDL_GetError();
        return -1;
    }

    s->renderer = SDL_CreateRenderer(
        s->window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!s->renderer) {
        s->err_msg = SDL_GetError();
        return -1;
    }

    SDL_SetRenderDrawColor(s->renderer, 0x00, 0x00, 0x00, 0xFF);

    if ((ret = TTF_Init()) < 0) {
        s->err_msg = TTF_GetError();
        return ret;
    }

    s->font = TTF_OpenFont(font_path ? font_path : "font.otb", 14);

    if (!s->font) {
        s->err_msg = TTF_GetError();
        return -1;
    }

    s->active = 1;

    return 0;
}

void m_printerr(MediaState *s)
{
    fprintf(stderr, "Error: %s\n", s->err_msg);
}

MediaObject m_text(MediaState *s, const char *str)
{
    MediaObject m;
    SDL_Surface *t = TTF_RenderText_Solid(s->font, str, (SDL_Color) {255, 255, 255, 255});
    SDL_GetClipRect(t, &m.clip_rect);
    SDL_Texture *ttx = SDL_CreateTextureFromSurface(s->renderer, t);
    SDL_FreeSurface(t);
    m.texture = ttx;
    return m;
}

void m_objfree(MediaObject *m)
{
    SDL_DestroyTexture(m->texture);
}

void m_close(MediaState *s)
{
    SDL_DestroyRenderer(s->renderer);
    SDL_DestroyWindow(s->window);
    TTF_CloseFont(s->font);
    SDL_Quit();
}

