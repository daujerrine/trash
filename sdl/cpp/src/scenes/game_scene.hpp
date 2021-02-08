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

        MediaRect player;
        std::vector<MediaPoint> enemies;

    public:
        GameScene(MediaState &m, MediaGraphics &g, SceneState &s):
            m(m), g(g), s(s), timer(1000),
            ui(m, g, "top", 0, (MediaRect) {0, 0, 800, 600}) {}
        ~GameScene() {};
        void init();
        void draw();
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
}

void GameScene::close()
{
}

#endif
