#include <cstdio>
#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "media.hpp"
#include "ui.hpp"

using std::to_string;

int main() {
    MediaState m;
    MediaGraphics g(m);
    UIState u(m, g);
    MediaObject k;
    MediaRect q = {40, 40, 300, 200};
    MediaRect dims = {100, 100, 400, 400};
    UILabel &label1 = u.add<UILabel>("Out Of Memory");
    u.add<UILabel>("==================");
    u.add<UILabel>("This prograaaaaaaaaaaaaam has crashed due to the lack of");
    u.add<UILabel>("free memory in your system.");
    u.geo.grid(1, 2);
    u.add<UILabel>("asas");
    u.add<UILabel>("asas");
    u.add<UILabel>("asas");
    u.add<UILabel>("asas");
    u.geo.grid(1, 4);
    u.add<UILabel>("asas");
    u.add<UILabel>("asas");
    u.add<UILabel>("asas");
    u.add<UILabel>("asas");
    u.refresh();
    while (m.active) {
        m.loop_start();

        while (SDL_PollEvent(&m.e)) {
            switch (m.e.type) {
            case SDL_QUIT:
                m.active = 0;
                break;

            case SDL_KEYDOWN:
                printf("%d\n", m.e.key.keysym.sym);
                switch (m.e.key.keysym.sym) {
                case SDLK_RIGHT:
                    PRINT_LINE;
                    dims.w += 5;
                    u.geo.set_container_dim(dims);
                    u.refresh();
                    break;

                case SDLK_LEFT:
                    PRINT_LINE;
                    dims.w -= 5;
                    u.geo.set_container_dim(dims);
                    u.refresh();
                    break;

                case SDLK_UP:
                    PRINT_LINE;
                    dims.h--;
                    u.geo.set_container_dim(dims);
                    u.refresh();
                    break;

                case SDLK_DOWN:
                    PRINT_LINE;
                    dims.h++;
                    u.geo.set_container_dim(dims);
                    u.refresh();
                    break;
                }
                break;
            }
        }
        g.text(k, "FPS: " + to_string(m.get_fps()));
        //SDL_Surface *t = TTF_RenderText_Solid(m.font, ("quack" + to_string(m.get_fps())).c_str(), (SDL_Color) {255, 255, 255, 255} ); NULLCHECK(t);
        //SDL_Texture *ttx = SDL_CreateTextureFromSurface(m.r, t); NULLCHECK(ttx);
        //SDL_FreeSurface(t);
        g.clear();
        g.set_color(128, 255, 255, 255);
        g.paint(k);
        //SDL_RenderCopy(m.r, ttx, nullptr, &q);
        g.line(10, 10, 400, 400);
        g.set_color(0, 0, 0, 255);
        //PRINT_LINE
        u.draw();
        //PRINT_LINE
        g.present();
        //SDL_DestroyTexture(ttx);
        m.loop_end();
    }
    return 0;
}
