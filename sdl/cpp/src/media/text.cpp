#include "text.hpp"

namespace media {

void Text::text(ObjectRef k, const char *str, Color c)
{
    Rect dims;
    SDL_Surface *t;

    // Empty strings are undefined behaviour.
    if (!str || *str == '\0') {
        t = TTF_RenderText_Solid(font, " ", c);
    } else {
        t = TTF_RenderText_Solid(font, str, c);
    }
    
    SDL_GetClipRect(t, &dims);
    PRINTRECT(dims);
    k.set_rect(dims);
    Texture *ttx = SDL_CreateTextureFromSurface(m.r, t);
    int a, b;
    SDL_QueryTexture(ttx, nullptr, nullptr, &a, &b);

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

};
