#include "media.hpp"

namespace media {

/*
 * =============================================================================
 * Sound
 * =============================================================================
 */

int Sound::set_volume(int volume)
{
    return Mix_VolumeChunk(data, volume);
}

int Sound::play(int channel, int loops)
{
    loops -= 1;
    return Mix_PlayChannel(channel, data, loops);
}

int Sound::fade_in(int duration, int channel, int loops)
{
    loops -= 1;
    return Mix_FadeInChannel(channel, data, loops, duration);
}

void SoundControl::pause(int channel)
{
    Mix_Pause(channel);
}

void SoundControl::resume(int channel)
{
    Mix_Resume(channel);
}

void SoundControl::stop(int channel)
{
    Mix_HaltChannel(channel);
}

void SoundControl::expire(int channel, int ticks)
{
    Mix_ExpireChannel(channel, ticks);
}

void SoundControl::finished(void (*callback)(int channel))
{
    Mix_ChannelFinished(callback);
}

int SoundControl::paused(int channel)
{
    return Mix_Paused(channel);
}

int SoundControl::playing(int channel)
{
    return Mix_Playing(channel);
}

int SoundControl::fade_out(int duration, int channel)
{
    return Mix_FadeOutChannel(channel, duration);
}

/*
 * =============================================================================
 * Music
 * =============================================================================
 */

int Music::play(int loops)
{
    loops -= 1;
    return Mix_PlayMusic(data, loops);
}

int Music::fade_in(int duration, int loops)
{
    loops -= 1;
    return Mix_FadeInMusic(data, loops, duration);
}

/*
 * =============================================================================
 * MusicControl
 * =============================================================================
 */


int MusicControl::seek_to(double pos)
{
    return Mix_SetMusicPosition(pos);
}

int MusicControl::set_volume(int volume)
{
    return Mix_VolumeMusic(volume);
}
 
void MusicControl::rewind()
{
    Mix_RewindMusic();
}

void MusicControl::pause()
{
    Mix_PauseMusic();
}

void MusicControl::stop()
{
    Mix_HaltMusic();
}

void MusicControl::finished(void (*callback)())
{
    Mix_HookMusicFinished(callback);
}

int MusicControl::paused()
{
    return Mix_PausedMusic();
}

int MusicControl::playing()
{
    return Mix_PlayingMusic();
}

int MusicControl::fade_out(int duration)
{
    return Mix_FadeOutMusic(duration);
}

/*
 * =============================================================================
 * Object
 * =============================================================================
 */
 
void Object::set(SDL_Texture *texture) {
    if (this->texture != nullptr) {
        this->free();
    }
    this->texture = texture;
}

void Object::free() {
    SDL_DestroyTexture(this->texture);
}

/*
 * =============================================================================
 * ClipObject
 * =============================================================================
 */

void ClipObject::set(SDL_Texture *texture) {
    if (this->texture != nullptr) {
        this->free();
    }
    this->texture = texture;
    SDL_QueryTexture(this->texture, nullptr, nullptr, &w, &h);
}

/*
 * =============================================================================
 * Text
 * =============================================================================
 */

void Text::text(ObjectRef k, const char *str, Color c)
{
    Rect dims;
    SDL_Surface *t;

    printf("><><><><><>Rendering text %s\n", str);
    // Empty strings are undefined behaviour.
    if (!str || *str == '\0') {
        t = TTF_RenderText_Solid(font, " ", c);
    } else {
        printf("Normal\n");
        t = TTF_RenderText_Solid(font, str, c);
    }

    printf("%d %d %d %d\n", c.r, c.g, c.b, c.a);
    
    SDL_GetClipRect(t, &dims);
    PRINTRECT(dims);
    k.set_rect(dims);
    Texture *ttx = SDL_CreateTextureFromSurface(m.r, t);
    int a,b;
    SDL_QueryTexture(ttx, nullptr, nullptr, &a, &b);
    printf("(((((( %d %d\n", a, b);
    SDL_FreeSurface(t);
    k.set(ttx);
}

void Text::text(ObjectRef k, const char *str)
{
    this->text(k, str, (Color) {255, 255, 255, 255});
}

void Text::text(ObjectRef k, std::string str, Color c)
{
    this->text(k, str.c_str(), c);
}

void Text::text(ObjectRef k, std::string str)
{
    this->text(k, str.c_str());
}

void Text::wrap_text(ObjectRef k, const char *str, Color c, Rect wrap_rect)
{
    Rect dims;
    SDL_Surface *t;

    // Empty strings are undefined behaviour.
    if (!str || *str == '\0') {
        t = TTF_RenderText_Blended_Wrapped(font, " ", c, wrap_rect.w);
    } else {
        t = TTF_RenderText_Blended_Wrapped(font, str, c, wrap_rect.w);
    }
    
    SDL_GetClipRect(t, &dims);
    k.set_rect(dims);
    SDL_Texture *ttx = SDL_CreateTextureFromSurface(this->m.r, t);
    SDL_FreeSurface(t);
    k.texture = ttx;
}

void Text::wrap_text(ObjectRef k, const char *str, Rect wrap_rect)
{
    this->wrap_text(k, str, {255, 255, 255, 255}, wrap_rect);
}

void Text::wrap_text(ObjectRef k, std::string str, Color c, Rect wrap_rect)
{
    this->wrap_text(k, str.c_str(), c, wrap_rect);
}

void Text::wrap_text(ObjectRef k, std::string str, Rect wrap_rect)
{
    this->wrap_text(k, str.c_str(), {255, 255, 255, 255}, wrap_rect);
}

/*
 * =============================================================================
 * State
 * =============================================================================
 */

State::State(
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

State::~State()
{
    SDL_DestroyRenderer(this->r);
    SDL_DestroyWindow(this->w);
    TTF_CloseFont(this->font);
    Mix_CloseAudio();
    SDL_Quit();
}

const char *State::get_err()
{
    return sdl_err_msg;
}

bool State::fail()
{
    return fail_flag;
}

void State::print_err()
{
    fputs("[SDL] ", stderr);
    fputs(sdl_err_msg, stderr);
    fputc('\n', stderr);
}

void State::display_err()
{
    print_err();
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", sdl_err_msg, nullptr);
}

/*
 * =============================================================================
 * Graphics
 * =============================================================================
 */

void Graphics::text(ObjectRef k, const char *str, Color c)
{
    Rect dims;
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
    Texture *ttx = SDL_CreateTextureFromSurface(this->m.r, t);
    SDL_FreeSurface(t);
    k.set(ttx);
}

void Graphics::text(ObjectRef k, const char *str)
{
    this->text(k, str, (SDL_Color) {255, 255, 255, 255});
}

void Graphics::text(ObjectRef k, std::string str, Color c)
{
    text(k, str.c_str(), c);
}

void Graphics::text(ObjectRef k, std::string str)
{
    this->text(k, str, (SDL_Color) {255, 255, 255, 255});
}

void Graphics::image(ObjectRef k, std::string filepath)
{
    SDL_Surface *t = IMG_Load(filepath.c_str());
    SDL_Texture *ttx = SDL_CreateTextureFromSurface(m.r, t);
    Rect dims;
    
    SDL_GetClipRect(t, &dims);
    k.set_rect(dims);
    k.set(ttx);

    SDL_FreeSurface(t);
}

};
