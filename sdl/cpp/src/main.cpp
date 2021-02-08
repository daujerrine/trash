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

using std::to_string;

Scene *scene_list[3];

int main() {
    MediaState m;
    SceneState s = SCENE_TITLE;

    if (m.fail()) {
        m.display_err();
        return 1;
    }

    MediaGraphics g(m);

    TitleScene title_scene(m, g, s);
    GameScene game_scene(m, g, s);
    game_scene.init();
    title_scene.init();

    scene_list[SCENE_GAME] = &game_scene;
    scene_list[SCENE_TITLE] = &title_scene;
    
    while (m.active) {
        m.loop_start();

        while (SDL_PollEvent(&m.e)) {
            switch (m.e.type) {
            case SDL_QUIT:
                m.active = false;
                break;
            }
        }

        scene_list[s]->update();

        g.clear();
        scene_list[s]->draw();
        g.present();
        //SDL_DestroyTexture(ttx);
        m.loop_end();

    }
    return 0;
}

/*
    MediaGraphics g(m);

    MediaObject k;
    MediaRect q = {40, 40, 300, 200};
    MediaRect dims = {100, 100, 400, 400};

    UITopLevel u(m, g, "top", 0, (MediaRect) {0, 0, 800, 600});
    u.geo.add(BOTTOMRIGHT, 10, 10);
    UIFrame &c = u.add<UIFrame>("sadasd", 0, (MediaRect) {0, 0, 400, 300});
    c.add<UILabel>("Out Of Memory");
    c.add<UILabel>("==================");
    c.add<UILabel>("This prograaaaaaaaaaaaaam has crashed due to the lack of");
    c.add<UILabel>("free memory in your system.");
    c.geo.add(1, 2);
    c.add<UILabel>("asas");
    c.add<UILabel>("asas");
    c.add<UILabel>("asas");
    c.add<UILabel>("asas");
    c.geo.add(1, 4);
    c.add<UILabel>("asas");
    c.add<UILabel>("asas");
    c.add<UILabel>("asas");
    c.add<UILabel>("asas");
    c.add<UILabel>("asas");
    c.add<UIButton>("asas");
    c.add<UIButton>("asas");

    u.geo.add(TOPRIGHT, 10, 10);
    u.add<UILabel>("TankGame 0.0.1");
    u.refresh();
    c.refresh();
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
                    dims.w += 5;
                    u.geo.set_container_dim(dims);
                    u.refresh();
                    break;

                case SDLK_LEFT:
                    dims.w -= 5;
                    u.geo.set_container_dim(dims);
                    u.refresh();
                    break;

                case SDLK_UP:
                    dims.h--;
                    u.geo.set_container_dim(dims);
                    u.refresh();
                    break;

                case SDLK_DOWN:
                    dims.h++;
                    u.geo.set_container_dim(dims);
                    u.refresh();
                    break;
                }
                break;
            }

            u.update();
        }

        g.text(k, "FPS: " + to_string(m.get_fps()));

        g.clear();
        u.draw();
        g.set_color(128, 255, 255, 255);
        g.paint(k);
        g.line(10, 10, 400, 400);
        g.present();
        //SDL_DestroyTexture(ttx);
        m.loop_end();
*/
