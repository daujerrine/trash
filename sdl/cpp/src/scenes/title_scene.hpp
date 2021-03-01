#ifndef SCENE_TITLE_H
#define SCENE_TITLE_H

#include "scene.hpp"
#include "media.hpp"
#include "ui.hpp"

class TitleScene : public Scene {
    private:
        MediaState &m;
        MediaGraphics &g;
        SceneState &s;
        UITopLevel ui;

    public:
        TitleScene(MediaState &m, MediaGraphics &g, SceneState &s):
            m(m), g(g), s(s),
            ui(m, g, "top", 0, (MediaRect) {0, 0, 800, 600}) {}
        ~TitleScene() {};
        void init();
        void draw();
        void event();
        void update();
        void close();
};

void TitleScene::init()
{
    ui.geo.add(CENTER, 0, 0);
    ui.add<UILabel>("Shoot Game");
    ui.refresh();
    init_flag = true;
}


void TitleScene::draw()
{
    ui.draw();
}


void TitleScene::event()
{
    switch (m.e.type) {
    case SDL_KEYUP:
        switch (m.e.key.keysym.sym) {
        case SDLK_SPACE:
            s = SCENE_GAME;
        }
        break;
    }
    ui.event();
}

void TitleScene::update()
{
    ui.update();
}

void TitleScene::close()
{
    init_flag = false;
}

#endif
