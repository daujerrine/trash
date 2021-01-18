#include <cstdio>
#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "media.hpp"

using std::to_string;

int main() {
    MediaState m;
    MediaGraphics g(m);

    MediaObject k;
    MediaRect q = {40, 40, 300, 200};

    while (m.active) {
        m.loop_start();

        while (SDL_PollEvent(&m.e)) {
            switch (m.e.type) {
            case SDL_QUIT:
                m.active = 0;
                break;

            case SDL_KEYDOWN:
                printf("%d\n", m.e.key.keysym.sym);
                break;
            }
        }
        g.text(k, "FPS: " + to_string(m.get_fps()));
        //SDL_Surface *t = TTF_RenderText_Solid(m.font, ("quack" + to_string(m.get_fps())).c_str(), (SDL_Color) {255, 255, 255, 255} ); NULLCHECK(t);
        //SDL_Texture *ttx = SDL_CreateTextureFromSurface(m.r, t); NULLCHECK(ttx);
        //SDL_FreeSurface(t);
        g.clear();
        g.set_color(128, 255, 255, 255);
        g.paint(k, scale_rect(k, 5, 5));
        //SDL_RenderCopy(m.r, ttx, nullptr, &q);
        g.line(10, 10, 400, 400);
        g.set_color(0, 0, 0, 255);
        g.present();
        //SDL_DestroyTexture(ttx);
        m.loop_end();
    }
    return 0;
}
