#ifndef SCENE_TITLE_H
#define SCENE_TITLE_H

#include "scene.hpp"
#include "media.hpp"
#include "ui.hpp"

using namespace media;

class TitleScene : public Scene {
    private:
        State &m;
        Graphics &g;
        SceneState &s;
        ui::TopLevel w;

    public:
        TitleScene(State &m, Graphics &g, SceneState &s):
            m(m), g(g), s(s),
            w(m, g, "top", 0, (Rect) {0, 0, 800, 600}) {}
        ~TitleScene() {};
        void init();
        void draw();
        void event();
        void update();
        void close();
};

void TitleScene::init()
{
    w.geo.add(CENTER, 0, 0);
    w.add<ui::Label>("Shoot Game");
    w.refresh();
    init_flag = true;
}


void TitleScene::draw()
{
    w.draw();
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
    w.event();
}

void TitleScene::update()
{
    w.update();
}

void TitleScene::close()
{
    init_flag = false;
}

#endif
