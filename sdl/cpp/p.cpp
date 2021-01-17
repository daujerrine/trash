#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "media.hpp"

int main() {
    MediaState m;
    MediaGraphics g(m);

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

        g.clear();
            g.set_color(255, 255, 255, 255);
            g.line(10, 10, 400, 400);
            g.set_color(0, 0, 0, 255);
        g.present();

        m.loop_end();
    }
    return 0;
}
