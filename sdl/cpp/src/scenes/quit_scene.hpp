#ifndef SCENE_QUIT_H
#define SCENE_QUIT_H

#include "scene.hpp"
#include "media.hpp"
#include "ui.hpp"

class QuitScene : public Scene {
    private:
        MediaState &m;
        MediaGraphics &g;
        SceneState &s;
        UITopLevel ui;
        UIFrame *f;
        UIButton *yes;
        UIButton *no;

    public:
        bool quitmode = true;
        QuitScene(MediaState &m, MediaGraphics &g, SceneState &s):
            m(m), g(g), s(s),
            ui(m, g, "top", 0, (MediaRect) {0, 0, 800, 600}) {}
        ~QuitScene() {};
        void init();
        void draw();
        void event();
        void update();
        void close();
};

void QuitScene::init()
{
    ui.geo.add(CENTER, 0, 0);
    f = &ui.add<UIFrame>("asd", 0, (MediaRect) {0, 0, 400, 100});
    f->add<UILabel>("Are you sure you want to quit?");
    f->geo.add(1, 2);
    yes = &f->add<UIButton>("yes");
    no = &f->add<UIButton>("no");
    f->geo.add(1, 6);
    f->add<UIButton>("vUp");
    f->add<UIButton>("vDn");
    f->add<UIButton>("vLf");
    f->add<UIButton>("x");
    f->add<UIButton>("y");
    f->add<UIButton>("z");
    f->add<UIButton>("z");
    f->add<UIButton>("z");
    ui.refresh();
    init_flag = true;
}


void QuitScene::draw()
{
    ui.draw();
}


void QuitScene::event()
{
    ui.event();

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
    ui.update();
}

void QuitScene::close()
{
    init_flag = false;
}

#endif
