#include <cstdio>
#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "media.hpp"
#include "ui.hpp"
#include "scene.hpp"
#include "scenes/title_scene.hpp"
#include "scenes/game_scene.hpp"
#include "scenes/quit_scene.hpp"

using std::to_string;

Scene *scene_list[3];

int media_main() {
    SceneState s = SCENE_TITLE;
    bool quitmode = false;

    MediaState m;
    MediaGraphics g(m);

    TitleScene title_scene(m, g, s);
    GameScene  game_scene(m, g, s);
    QuitScene  quit_scene(m, g, s);

    game_scene.init();
    title_scene.init();
    quit_scene.init();

    scene_list[SCENE_GAME] = &game_scene;
    scene_list[SCENE_TITLE] = &title_scene;

    while (m.active) {
        m.loop_start();

        while (SDL_PollEvent(&m.e)) {
            switch (m.e.type) {
            case SDL_QUIT:
                // m.active = false;
                //printf("Quit Started\n");
                quitmode = true;
                quit_scene.quitmode = true;
                break;

            case SDL_KEYDOWN:
                switch (m.e.key.keysym.sym) {
                case SDLK_ESCAPE:
                    m.active = false;
                    break;
                }
                break;
            }

            if (!quitmode) {
                scene_list[s]->event();
                quit_scene.event();
            } else {
                quit_scene.event();
                if (quit_scene.quitmode == false) {
                    quitmode = false;
                }
                //printf("Quitmode value: %d\n", quitmode);
            }
        }

        if (!quitmode)
            scene_list[s]->update();
        else
            quit_scene.update();

        g.clear();
        scene_list[s]->draw();
        g.set_color(255, 255, 255, 255);
        g.rect({200, 200, 200, 200});

        if (quitmode)
            quit_scene.draw();
        g.present();
        m.loop_end();

    }
    return 0;
}

int main() {
    try {
        return media_main();
    } catch(int err_code) {
        printf("[MAIN] Exiting with error code %d\n", err_code);
    }

    return 1;
}
