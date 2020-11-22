
#include <stdio.h>
#include <stdint.h>
#include "media.h"

#define GAME_DEFAULT_SCREEN_WIDTH 800
#define GAME_DEFAULT_SCREEN_HEIGHT 600
#define GAME_DEFAULT_FPS (1000/60)

int main(int argc, char **argv)
{
    MediaState s = {0};
    MediaRect pos = {0, 0, 100, 100};
    uint64_t framecount = 0;
    char str[4096];
    int velocity[2] = {0, 0};
    int ground = 1;
    int jump = 0;
    m_init(&s, "Game", "font.otb");
    uint64_t old = 0, new = 0;
    int seconds = 0;
    MediaTimer t;
    m_timer_init(&t, 1000);
    
    while (s.active) {
        new = SDL_GetTicks();
        s.delta = new - old;
        old = new;
        m_loop_start(&s);
        while (SDL_PollEvent(&s.e)) {
            switch (s.e.type) {
            case SDL_QUIT:
                s.active = 0;
                break;
            }

            if (s.e.key.repeat != 0)
                continue;

            /* Key Repeat Sequences */

            switch (s.e.type) {
            case SDL_KEYUP:
                printf("u %d\n", s.e.key.keysym.sym);
                switch (s.e.key.keysym.sym) {
                case SDLK_UP:
                    jump = 0;
                    break;
                case SDLK_DOWN:
                    velocity[1] -= 2;
                    break;
                case SDLK_LEFT:
                    velocity[0] += 4;
                    break;
                case SDLK_RIGHT:
                    velocity[0] -= 4;
                    break;
                }
                break;
            case SDL_KEYDOWN:
                printf("d %d\n", s.e.key.keysym.sym);
                switch (s.e.key.keysym.sym) {
                case SDLK_UP:
                    jump = 1;
                    break;
                case SDLK_DOWN:
                    if (!ground)
                        velocity[1] += 10;
                    break;
                case SDLK_LEFT:
                    velocity[0] -= 4;
                    break;
                case SDLK_RIGHT:
                    velocity[0] += 4;
                    break;
                }
                break;
            }
        }

        if (ground && jump) {
            velocity[1] -= 20;
            ground = 0;
        }

        // Update
        if ((pos.y + pos.h + velocity[1]) >= 600) {
            velocity[1] = 0;
            pos.y = 600 - pos.h;
            ground = 1;
        } else {
            velocity[1] += 3;
        }

        pos.x += velocity[0];
        pos.y += velocity[1];

        m_timer_update(&s, &t);

        if (m_timer_done(&t)) {
            seconds += 1;
        }

        SDL_RenderClear(s.renderer);
            sprintf(str, "velx = %d vely = %d FPS: %f delta = %u seconds = %d",
                velocity[0], velocity[1], m_get_fps(&s), s.delta, seconds);
            MediaObject k = m_text(&s, str);
            m_set_color(&s, 255, 255, 255, 255);
            m_rect(&s, &pos);
            m_paint(&s, &k, NULL);
            m_set_color(&s, 0, 0, 0, 255);
            m_objfree(&k);
        SDL_RenderPresent(s.renderer);

        framecount++;
        m_loop_end(&s);
        // printf("fps_start = %ld fps_end = %ld, fps = %.4f\n", fps_start, fps_end, fps);
    }

    m_close(&s);
    return 0;
}
