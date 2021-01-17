#include "media.hpp"

/*
 * =============================================================================
 * MediaState
 * =============================================================================
 */

MediaState::MediaState(
    int w,
    int h,
    int max_fps,
    const char *window_name,
    const char *font_path
)
{
    int ret;
    
    this->max_fps = (1000/max_fps);
    this->main_w = w;
    this->main_h = h;

    if ((ret = SDL_Init(SDL_INIT_VIDEO)) < 0) {
        this->sdl_err_msg = SDL_GetError();
        throw ret;
    }

    this->w = SDL_CreateWindow(
        window_name,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        this->main_w,
        this->main_h,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    if (!this->w) {
        this->sdl_err_msg = SDL_GetError();
        throw -1;
    }

    this->r = SDL_CreateRenderer(
        this->w,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!this->r) {
        this->sdl_err_msg = SDL_GetError();
        throw -1;
    }

    SDL_SetRenderDrawColor(this->r, 0x00, 0x00, 0x00, 0xFF);

    if ((ret = TTF_Init()) < 0) {
        this->sdl_err_msg = TTF_GetError();
        throw ret;
    }

    this->font = TTF_OpenFont(font_path, 14);

    if (!this->font) {
        this->sdl_err_msg = TTF_GetError();
        throw -1;
    }

    this->active = true;
}

MediaState::~MediaState()
{
    SDL_DestroyRenderer(this->r);
    SDL_DestroyWindow(this->w);
    TTF_CloseFont(this->font);
    SDL_Quit();
}

/*
 * =============================================================================
 * MediaTimer
 * =============================================================================
 */

MediaTimer::MediaTimer(uint32_t duration)
{
    this->duration = duration;
    this->time = duration;
}
