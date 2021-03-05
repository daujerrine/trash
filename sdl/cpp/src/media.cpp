#include "media.hpp"

/*
 * =============================================================================
 * MediaSample
 * =============================================================================
 */

int MediaSample::set_volume(int volume)
{
    return Mix_VolumeChunk(data, volume);
}

int MediaSample::play(int channel, int loops)
{
    loops -= 1;
    return Mix_PlayChannel(channel, data, loops);
}

int MediaSample::fade_in(int duration, int channel, int loops)
{
    loops -= 1;
    return Mix_FadeInChannel(channel, data, loops, duration);
}

void MediaSample::pause(int channel)
{
    Mix_Pause(channel);
}

void MediaSample::resume(int channel)
{
    Mix_Resume(channel);
}

void MediaSample::stop(int channel)
{
    Mix_HaltChannel(channel);
}

void MediaSample::expire(int channel, int ticks)
{
    Mix_ExpireChannel(channel, ticks);
}

void MediaSample::finished(void (*callback)(int channel))
{
    Mix_ChannelFinished(callback);
}

int MediaSample::paused(int channel)
{
    return Mix_Paused(channel);
}

int MediaSample::playing(int channel)
{
    return Mix_Playing(channel);
}

int MediaSample::fade_out(int duration, int channel)
{
    return Mix_FadeOutChannel(channel, duration);
}

/*
 * =============================================================================
 * MediaMusic
 * =============================================================================
 */

int MediaMusic::set_volume(int volume)
{
    return Mix_VolumeMusic(volume);
}

int MediaMusic::play(int loops)
{
    loops -= 1;
    return Mix_PlayMusic(data, loops);
}

int MediaMusic::fade_in(int duration, int loops)
{
    loops -= 1;
    return Mix_FadeInMusic(data, loops, duration);
}

int MediaMusic::seek_to(double pos)
{
    return Mix_SetMusicPosition(pos);
}

void rewind()
{
    Mix_RewindMusic();
}

void MediaMusic::pause()
{
    Mix_PauseMusic();
}

void MediaMusic::stop()
{
    Mix_HaltMusic();
}

void MediaMusic::finished(void (*callback)())
{
    Mix_HookMusicFinished(callback);
}

int MediaMusic::paused()
{
    return Mix_PausedMusic();
}

int MediaMusic::playing()
{
    return Mix_PlayingMusic();
}

int MediaMusic::fade_out(int duration)
{
    return Mix_FadeOutMusic(duration);
}



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
 * MediaText
 * =============================================================================
 */

void MediaText::text(MediaObjectRef k, const char *str, MediaColor c)
{
    MediaRect dims;
    SDL_Surface *t;

    // Empty strings are undefined behaviour.
    if (!str || *str == '\0') {
        t = TTF_RenderText_Solid(this->m.font, " ", c);
    } else {
        t = TTF_RenderText_Solid(this->m.font, str, c);
    }
    
    SDL_GetClipRect(t, &dims);
    k.set_size(dims.w, dims.h);
    SDL_Texture *ttx = SDL_CreateTextureFromSurface(this->m.r, t);
    SDL_FreeSurface(t);
    k.texture = ttx;
}

void MediaText::text(MediaObjectRef k, const char *str)
{
    this->text(k, str, {255, 255, 255, 255});
}

void MediaText::text(MediaObjectRef k, std::string str, MediaColor c)
{
    this->text(k, str.c_str(), c);
}

void MediaText::text(MediaObjectRef k, std::string str)
{
    this->text(k, str.c_str());
}

void MediaText::wrap_text(MediaObjectRef k, const char *str, MediaColor c, MediaRect wrap_rect)
{
    MediaRect dims;
    SDL_Surface *t;

    // Empty strings are undefined behaviour.
    if (!str || *str == '\0') {
        t = TTF_RenderText_Blended_Wrapped(font, " ", c, wrap_rect.w);
    } else {
        t = TTF_RenderText_Blended_Wrapped(font, str, c, wrap_rect.w);
    }
    
    SDL_GetClipRect(t, &dims);
    k.set_size(dims.w, dims.h);
    SDL_Texture *ttx = SDL_CreateTextureFromSurface(this->m.r, t);
    SDL_FreeSurface(t);
    k.texture = ttx;
}

void MediaText::wrap_text(MediaObjectRef k, const char *str, MediaRect wrap_rect)
{
    this->wrap_text(k, str, {255, 255, 255, 255}, wrap_rect);
}

void MediaText::wrap_text(MediaObjectRef k, std::string str, MediaColor c, MediaRect wrap_rect)
{
    this->wrap_text(k, str.c_str(), c, wrap_rect);
}

void MediaText::wrap_text(MediaObjectRef k, std::string str, MediaRect wrap_rect)
{
    this->wrap_text(k, str.c_str(), {255, 255, 255, 255}, wrap_rect);
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
    
    this->max_fps = (1000 / max_fps);
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

        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
            this->sdl_err_msg = IMG_GetError();
            throw -1;
        }
        
        if ((ret = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048)) < 0) {
            this->sdl_err_msg = Mix_GetError();
            throw ret;
        }

    } catch (int err) {
        display_err();
        throw err;
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
    MediaRect dims;
    SDL_Surface *t;

    // Empty strings are undefined behaviour.
    // printf("String to convert: '%s'\n", str);
    if (!str || *str == '\0') {
        t = TTF_RenderText_Solid(this->m.font, " ", c);
    } else {
        t = TTF_RenderText_Solid(this->m.font, str, c);
    }
    
    SDL_GetClipRect(t, &dims);
    //printf("LABELMAKE\n");
    //PRINTRECT(dims);
    k.set_rect(dims);
    SDL_Texture *ttx = SDL_CreateTextureFromSurface(this->m.r, t);
    SDL_FreeSurface(t);
    k.set(ttx);
}

void MediaGraphics::text(MediaObjectRef k, const char *str)
{
    this->text(k, str, (SDL_Color) {255, 255, 255, 255});
}

void MediaGraphics::text(MediaObjectRef k, std::string str, MediaColor c)
{
    text(k, str.c_str(), c);
}

void MediaGraphics::text(MediaObjectRef k, std::string str)
{
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
