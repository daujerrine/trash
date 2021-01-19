
#include <stdio.h>
#include <stdint.h>
#include "media.h"
#include "ui.h"

#define GAME_DEFAULT_SCREEN_WIDTH 800
#define GAME_DEFAULT_SCREEN_HEIGHT 600
#define GAME_DEFAULT_FPS (1000/60)

int main(int argc, char **argv)
{
    MediaState s = {0};
    MediaRect pos = {0, 0, 100, 100};
    MediaObject msg;
    uint64_t framecount = 0;
    uint64_t old = 0, new = 0;
    int seconds = 0;
    char str[4096];
    MediaTimer t;
    UIState ui;
    
    m_timer_init(&t, 1000);
    m_init(&s, "Game", "font.otb");

    ui_init(&ui, &s, ui_sticky_rect(800, 600, 400, 200, BOTTOMRIGHT, 20, 20));
    ui_add_widget(&ui, UI_LABEL, "a", 0);
    ui_add_widget(&ui, UI_BUTTON, "b", 0);
    ui_grid(&ui, 2, 2);
    ui_add_widget(&ui, UI_LABEL, "c", 0);
    ui_add_widget(&ui, UI_BUTTON, "d", 0);
    ui_add_widget(&ui, UI_LABEL, "e", 0);
    ui_add_widget(&ui, UI_BUTTON, "f", 0);
    ui_add_widget(&ui, UI_LABEL, "g", 0);
    ui_add_widget(&ui, UI_BUTTON, "h", 0);
    ui_grid(&ui, 1, 10);
    ui_add_widget(&ui, UI_BUTTON, "+", 0);
    ui_add_widget(&ui, UI_BUTTON, "-", 0);
    ui_add_widget(&ui, UI_BUTTON, "*", 0);
    ui_add_widget(&ui, UI_BUTTON, "/", 0);
    ui_add_widget(&ui, UI_BUTTON, "%", 0);
    ui_refresh_layout(&ui);
    
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
        }

        ui_update_widgets(&ui);

        m_timer_update(&s, &t);

        if (m_timer_done(&t)) {
            seconds += 1;
        }

        SDL_RenderClear(s.renderer);
            sprintf(str, "FPS: %f delta = %u seconds = %d", m_get_fps(&s), s.delta, seconds);
            msg = m_text(&s, str);
            m_set_color(&s, 255, 255, 255, 255);
            m_paint(&s, &msg, &(MediaRect) {0, 0, 20, 80});
            ui_draw_widgets(&ui);
            m_set_color(&s, 0, 0, 0, 255);
            m_objfree(&msg);
        SDL_RenderPresent(s.renderer);

        framecount++;
        m_loop_end(&s);
    }

    m_close(&s);
    return 0;
}

