#ifndef SCENE_GAME_H
#define SCENE_GAME_H
#include "scene.hpp"
#include "media.hpp"
#include "ui.hpp"
#include <vector>
#include <string>

class GameScene : public Scene {
    private:
        MediaState &m;
        MediaGraphics &g;
        UITopLevel ui;
        SceneState &s;
        UILabel *counter;
        int counter_val = 0;
        MediaTimer timer;

        MediaRect player = {0, 0, 40, 40};
        MediaRect bullet = {0, 0, 10, 10};
        bool firing = false;

    public:
        GameScene(MediaState &m, MediaGraphics &g, SceneState &s):
            m(m), g(g), s(s), timer(1000),
            ui(m, g, "top", 0, (MediaRect) {0, 0, 800, 600}) {}
        ~GameScene() {};
        void init();
        void draw();
        void event();
        void update();
        void close();
};


void GameScene::init()
{
    ui.geo.add(BOTTOMRIGHT, 0, 0);
    UIFrame &c = ui.add<UIFrame>("Menu", 0, (MediaRect) {0, 0, 400, 100});
    c.add<UILabel>("Game Scene");
    c.add<UILabel>("You are now playing the game");
    counter = &c.add<UILabel>("0");
    ui.refresh();
}


void GameScene::draw()
{
    ui.draw();
    g.set_color(255, 255, 255, 255);
    g.rect(player);
    if (firing) {
        g.set_color(255, 128, 0, 255); 
        g.frect(bullet);
    }
}

void GameScene::event()
{
    switch (m.e.type) {
    case SDL_KEYDOWN:
        printf("%d\n", m.e.key.keysym.sym);
        switch (m.e.key.keysym.sym) {
        case SDLK_RIGHT:
            player.x += 5;
            break;

        case SDLK_LEFT:
            player.x -= 5;
            break;

        case SDLK_UP:
            player.y -= 5;
            break;

        case SDLK_DOWN:
            player.y += 5;
            break;

        case SDLK_SPACE:
            if (firing == false) {
                bullet = Util::rect_align(player, bullet, CENTER, 0, 0);
                firing = true;
            }
            break;
        }
        break;
    }
    ui.event();
}

void GameScene::update()
{
    // printf("called\n");
    ui.update();
    timer.update(m.delta);
    if (timer.done()) {
        counter_val++;
        counter->set_label(std::to_string(counter_val) + " delta: " + std::to_string(m.delta));
    }

    if (firing) {
        bullet.y -= 20;
        if (bullet.y < 0) {
            firing = false;
        }
    }
}

void GameScene::close()
{
}

#endif
