#ifndef SCENE_GAME_H
#define SCENE_GAME_H
#include "scene.hpp"
#include "media.hpp"
#include "ui.hpp"
#include <vector>
#include <string>
#include <stdlib.h>

class GameScene : public Scene {
    private:
        MediaState &m;
        MediaGraphics &g;
        UITopLevel ui;
        SceneState &s;
        UILabel *counter;
        UILabel *info, *info2;
        int counter_val = 0;
        MediaTimer timer;

        MediaRect player = {0, 0, 40, 40};
        int xvel = 0, yvel = 0;
        int xpvel = 0, ypvel = 0;
        int xaccn = 0;
        int yaccn = 0;
        int cap = 10;
        int friction = 1;
        MediaRect bullet = {0, 0, 10, 10};
        bool firing = false;
        bool motion = false;

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
    counter = &c.add<UILabel>("0");
    info    = &c.add<UILabel>("");
    info2    = &c.add<UILabel>("");
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
    if (m.e.key.repeat == 0) {
        switch (m.e.type) {
        case SDL_KEYDOWN:
            printf("%d\n", m.e.key.keysym.sym);
            switch (m.e.key.keysym.sym) {
            case SDLK_RIGHT:
                xaccn = 2;
                motion = true;
                break;

            case SDLK_LEFT:
                xaccn = -2;
                motion = true;
                break;

            case SDLK_UP:
                yaccn = -2;
                motion = true;
                break;

            case SDLK_DOWN:
                yaccn = 2;
                motion = true;
                break;

            case SDLK_SPACE:
                if (firing == false) {
                    bullet = Util::rect_align(player, bullet, CENTER, 0, 0);
                    firing = true;
                }
                break;
            }
            break;

        case SDL_KEYUP:
            printf("%d\n", m.e.key.keysym.sym);
            switch (m.e.key.keysym.sym) {
            case SDLK_RIGHT:
                xaccn = 0;
                motion = false;
                break;

            case SDLK_LEFT:
                xaccn = 0;
                motion = false;
                break;

            case SDLK_UP:
                yaccn = 0;
                motion = false;
                break;

            case SDLK_DOWN:
                yaccn = 0;
                motion = false;
                break;
            }
            break;
        }
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

    /*info->set_label("Xvel: " + std::to_string(xvel) + " " +\
                    "Yvel: " + std::to_string(yvel) + " " +\
                    "Xaccn: " + std::to_string(xaccn) + " " +\
                    "Yaccn: " + std::to_string(yaccn) + " " +\
                    "Px: "   + std::to_string(player.x) + " " +\
                    "Py: "   + std::to_string(player.y));
    info2->set_label("M: " + std::to_string(motion));
    */
    if (abs(xvel) < cap && motion)
        xvel += xaccn;
    if (abs(yvel) < cap && motion)
        yvel += yaccn;
    
    player.x += xvel;
    player.y += yvel;
    
    if (!motion) {
        if (xvel > 0) xvel -= friction;
        if (xvel < 0) xvel += friction;
        if (yvel > 0) yvel -= friction;
        if (yvel < 0) yvel += friction;
        if (xvel > 0) xvel -= friction;
        if (xvel < 0) xvel += friction;
        if (yvel > 0) yvel -= friction;
        if (yvel < 0) yvel += friction;
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
