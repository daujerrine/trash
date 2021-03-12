#include "graphics.hpp"

namespace media {

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
