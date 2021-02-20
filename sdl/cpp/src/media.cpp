#include "media.hpp"

/*
 * =============================================================================
 * MediaObject
 * =============================================================================
 */
 
void MediaObject::set(SDL_Texture *texture) {
    if (this->texture != nullptr) {
        this->free();
    }
    this->texture = texture;
}

void MediaObject::free() {
//    printf("[POTENTIAL BUG] Free Called\n");
    SDL_DestroyTexture(this->texture);
}

/*
 * =============================================================================
 * MediaClipObject
 * =============================================================================
 */

void MediaClipObject::set(SDL_Texture *texture) {
    if (this->texture != nullptr) {
        this->free();
    }
    this->texture = texture;
    SDL_QueryTexture(this->texture, nullptr, nullptr, &w, &h);
}

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

    try {
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

        //SDL_SetRenderDrawColor(this->r, 0x00, 0x00, 0x00, 0xFF);

        if ((ret = TTF_Init()) < 0) {
            this->sdl_err_msg = TTF_GetError();
            throw ret;
        }

        this->font = TTF_OpenFont(font_path, 14);

        if (!this->font) {
            this->sdl_err_msg = TTF_GetError();
            throw -1;
        }
    } catch (int err) {
        fail_flag = true;
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

const char *MediaState::get_err()
{
    return sdl_err_msg;
}

bool MediaState::fail()
{
    return fail_flag;
}

void MediaState::print_err()
{
    fputs("[SDL] ", stderr);
    fputs(sdl_err_msg, stderr);
    fputc('\n', stderr);
}

void MediaState::display_err()
{
    print_err();
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", sdl_err_msg, nullptr);
}

/*
 * =============================================================================
 * MediaGraphics
 * =============================================================================
 */

void MediaGraphics::text(MediaObjectRef k, const char *str, MediaColor c)
{
    SDL_Surface *t = TTF_RenderText_Solid(this->m.font, str, c);
    SDL_GetClipRect(t, &k.dest_rect);
    SDL_Texture *ttx = SDL_CreateTextureFromSurface(this->m.r, t);
    SDL_FreeSurface(t);
    k.texture = ttx;
}

void MediaGraphics::text(MediaObjectRef k, const char *str)
{
    // printf("At: %s, %d, %s\n", __PRETTY_FUNCTION__, __LINE__, __FILE__);
    this->text(k, str, (SDL_Color) {255, 255, 255, 255});
}

void MediaGraphics::text(MediaObjectRef k, std::string str, MediaColor c)
{
    MediaRect dims;
    SDL_Surface *t = TTF_RenderText_Solid(this->m.font, str.c_str(), c);
    SDL_GetClipRect(t, &dims);
    k.set_size(dims.w, dims.h);
    SDL_Texture *ttx = SDL_CreateTextureFromSurface(this->m.r, t);
    SDL_FreeSurface(t);
    k.set(ttx);
}

void MediaGraphics::text(MediaObjectRef k, std::string str)
{
    // printf("At: %s, %d, %s\n", __PRETTY_FUNCTION__, __LINE__, __FILE__);
    this->text(k, str, (SDL_Color) {255, 255, 255, 255});
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
