#include "container.hpp"

namespace media {

namespace ui {

bool TopLevel::event()
{
    bool no_refresh;

    switch (m.e.type) {
    case SDL_WINDOWEVENT:
        switch (m.e.window.event) {
            case SDL_WINDOWEVENT_RESIZED:
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                dims = { 0, 0, m.e.window.data1, m.e.window.data2 };
                refresh();
        }
        break;
    }

    return Container::event();
}

void Frame::draw()
{
    g.set_color(40, 40, 40, 255);
    g.frect(dims);
    g.set_color(255, 255, 255, 255);
    g.rect(dims);
    for (auto &i: widgets)
        i->draw();
}

};

};
