#ifndef SCENE_QT_H
#define SCENE_QT_H

#include "scene.hpp"
#include "media/media.hpp"
#include "ui/ui.hpp"

using namespace media;

class QuitScene : public Scene {
    private:
        State &m;
        Graphics &g;
        SceneState &s;
        ui::TopLevel w;
        ui::Frame *f;
        ui::Button *yes;
        ui::Button *no;

    public:
        bool quitmode = true;
        QuitScene(State &m, Graphics &g, SceneState &s):
            m(m), g(g), s(s),
            w(m, g, "top", 0, (Rect) {0, 0, 800, 600}) {}
        ~QuitScene() {};
        void init();
        void draw();
        void event();
        void update();
        void close();
};

void QuitScene::init()
{
    w.geo.add(CENTER, 0, 0);
    f = &w.add<ui::Frame>("asd", 0, (Rect) {0, 0, 400, 100});
    f->add<ui::Label>("Are you sure you want to quit?");
    f->geo.add(1, 2);
    yes = &f->add<ui::Button>("yes");
    no = &f->add<ui::Button>("no");
    f->geo.add(1, 6);
    f->add<ui::Button>("vUp");
    f->add<ui::Button>("vDn");
    f->add<ui::Button>("vLf");
    f->add<ui::Button>("x");
    f->add<ui::Button>("y");
    f->add<ui::Button>("z");
    f->add<ui::Button>("z");
    f->add<ui::Button>("z");
    w.refresh();
    init_flag = true;
}


void QuitScene::draw()
{
    w.draw();
}


void QuitScene::event()
{
    w.event();

    if (yes->is_down()) {
        m.active = false;
    }  else if (no->is_down()) {
        printf("active: %d\n", quitmode);
        this->quitmode = false;
        printf("active: %d\n", quitmode);
    }
}

void QuitScene::update()
{
    w.update();
}

void QuitScene::close()
{
    init_flag = false;
}

#endif
