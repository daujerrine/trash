#ifndef SCENE_GAME_H
#define SCENE_GAME_H

#include "scene.hpp"
#include "media/media.hpp"
#include "ui/ui.hpp"

#include <vector>
#include <deque>
#include <string>
#include <stdlib.h>
#include <time.h>

using namespace media;

class GameScene : public Scene {
    private:
        State &m;
        Graphics &g;
        ui::TopLevel w;
        ui::Frame *c;
        SceneState &s;
        ui::Label *counter;
        ui::Label *info, *info2;
        int counter_val = 0;
        Timer timer;
        Timer motion_timer;
        Timer bullet_timer;

        Rect player = {0, 0, 40, 40};
        Rect enemy = {0, 0, 20, 20};
        Rect bullet_dims = {0, 0, 10, 10};

        Sound shoot_snd;
        Music song;

        int xvel = 0, yvel = 0;
        int xpvel = 0, ypvel = 0;
        int xaccn = 0;
        int yaccn = 0;
        int cap = 10;
        int friction = 1;
        std::deque<Rect> bullets;
        int num_bullets = 0;
        bool firing = false;
        bool motion = false;
        bool enemy_in = false;

    public:
        GameScene(State &m, Graphics &g, SceneState &s):
            m(m), g(g), s(s), timer(1000), motion_timer(10), bullet_timer(50),
            shoot_snd("assets/shoot.wav"),
            song("assets/song.xm"),
            w(m, g, "top", 0, (Rect) {0, 0, 800, 600}) {}
        ~GameScene() {};
        void init();
        void draw();
        void event();
        void update();
        void close();
};


void GameScene::init()
{
    srand(time(nullptr));
    w.geo.add(BOTTOMRIGHT, 0, 0);
    c = &w.add<ui::Frame>("Menu", 0, (Rect) {0, 0, 400, 100});
    c->add<ui::Label>("Game Scene");
    counter = &c->add<ui::Label>("0");
    info    = &c->add<ui::Label>("");
    info2    = &c->add<ui::Label>("");
    w.refresh();
    c->hide();
    song.set_volume(40);
    // song.play();
    init_flag = true;
}

void GameScene::draw()
{
    w.draw();
    g.set_color(255, 255, 255, 255);
    g.rect(player);
    g.set_color(255, 128, 0, 255);
    for (auto &i :bullets)
        g.frect(i);

    if (enemy_in == true) {
        g.set_color(0x41, 0x83, 0xF5, 0xFF);
        g.frect(enemy);
    }
}

void GameScene::event()
{
    if (m.e.key.repeat == 0) {
        switch (m.e.type) {
        case SDL_KEYUP:
            // printf("%d\n", m.e.key.keysym.sym);
            switch (m.e.key.keysym.sym) {
            case SDLK_RIGHT:
                xaccn -= 2;
                break;

            case SDLK_LEFT:
                xaccn -= -2;
                break;

            case SDLK_UP:
                yaccn -= -2;
                break;

            case SDLK_DOWN:
                yaccn -= 2;
                break;
            case SDLK_SPACE:
                firing = false;
                break;

            case SDLK_m:
                if (c->shown())
                    c->hide();
                else
                    c->show();
                break;
            }
            break;

        case SDL_KEYDOWN:
            switch (m.e.key.keysym.sym) {
            case SDLK_RIGHT:
                xaccn += 2;
                break;

            case SDLK_LEFT:
                xaccn += -2;
                break;

            case SDLK_UP:
                yaccn += -2;
                break;

            case SDLK_DOWN:
                yaccn += 2;
                break;

            case SDLK_SPACE:
                /*if (firing == false) {
                    bullet = Util::rect_align(player, bullet, CENTER, 0, 0);
                    if (num_bullets < 20) {
                        num_bullets++;
                    }
                    firing = true;
                }*/
                firing = true;
                break;
            }
            break;
        }
    }

    w.event();
}

void GameScene::update()
{
    // printf("called\n");
    timer.update(m.delta);
    motion_timer.update(m.delta);
    bullet_timer.update(m.delta);
    

    if (timer.done()) {
        counter_val++;
        counter->set_label(std::to_string(counter_val) + " delta: " + std::to_string(m.delta));
    }

    info->set_label("Xvel: " + std::to_string(xvel) + " " +    \
                    "Yvel: " + std::to_string(yvel) + " " +    \
                    "Xaccn: " + std::to_string(xaccn) + " " +  \
                    "Yaccn: " + std::to_string(yaccn) + " " +  \
                    "Px: "   + std::to_string(player.x) + " " +\
                    "Py: "   + std::to_string(player.y));
    info2->set_label("M: " + std::to_string(motion) + " Bs: " + std::to_string(bullets.size()));

    uint32_t x;

    if ((x = motion_timer.delay()) > 0) {
        if ((abs(xvel) < cap))
            xvel += xaccn;
        if ((abs(yvel) < cap))
            yvel += yaccn;
        
        player.x += xvel;
        player.y += yvel;
        
        //if (!motion) {
            if      (xvel > 0) xvel -= friction;
            else if (xvel < 0) xvel += friction;

            if      (yvel > 0) yvel -= friction;
            else if (yvel < 0) yvel += friction;
            //if (xvel > 0) xvel -= friction;
            //if (xvel < 0) xvel += friction;
            //if (yvel > 0) yvel -= friction;
            //if (yvel < 0) yvel += friction;
        //}
    }

    if (firing && num_bullets < 20 && bullet_timer.done()) {
        num_bullets++;
        bullets.push_back(Util::rect_align(player, bullet_dims, CENTER, 0, 0));
        shoot_snd.play(0);
    }
    
    for (auto &i: bullets) {
        i.y -= 10;
        if (bullets.front().y < 0) {
            bullets.pop_front();
            num_bullets--;
        }
    }

    if (enemy_in == false) {
        enemy.x = rand() % 800;
        enemy.y = -20;
        enemy_in = true;
    }  else {
        enemy.y += 10;
        if (enemy.y > 600) {
            enemy_in = false;
        }
    }

    w.update();
}

void GameScene::close()
{
}

#endif
