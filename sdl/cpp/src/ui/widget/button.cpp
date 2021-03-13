#include "button.hpp"


namespace media {

namespace ui {

using namespace util;

void Button::draw()
{
    switch (state) {
    case UI_WIDGET_NORMAL:
        p.box(dims);
        g.paint(o_label);
        break;

    case UI_WIDGET_ACTIVE:
        p.fbox(dims);
        g.paint(o_label);
        break;

    case UI_WIDGET_DOWN:
        g.set_color(255, 255, 255, 255);
        g.frect(dims);
        SDL_SetTextureColorMod(o_label.texture, 0, 0, 0);
        o_label.dest_rect.y += 1;
        g.paint(o_label);
        SDL_SetTextureColorMod(o_label.texture, 255, 255, 255);
        o_label.dest_rect.y -= 1;
        break;
    }
}

bool Button::event()
{
    if (clicked_flag == true)
        clicked_flag = false;
    switch (m.e.type) {
    case SDL_MOUSEMOTION:
        if (point_in_rect(m.e.motion.x, m.e.motion.y, dims) && state != UI_WIDGET_DOWN) {
            state = UI_WIDGET_ACTIVE;
            //clicked_flag = false;
        } else if (state != UI_WIDGET_DOWN) {
            state = UI_WIDGET_NORMAL;
            //clicked_flag = false;
        }
        break;

    case SDL_MOUSEBUTTONDOWN:
        if (point_in_rect(m.e.button.x, m.e.button.y, dims)) {
            state = UI_WIDGET_DOWN;
            //clicked_flag = false;
        }
        break;

    case SDL_MOUSEBUTTONUP:
        if (point_in_rect(m.e.button.x, m.e.button.y, dims) && state == UI_WIDGET_DOWN) {
            state = UI_WIDGET_ACTIVE;
            printf("Here\n");
            clicked_flag = true;
        } else if (point_in_rect(m.e.button.x, m.e.button.y, dims)) {
            state = UI_WIDGET_ACTIVE;
            //clicked_flag = false;
        } else {
            state = UI_WIDGET_NORMAL;
            //clicked_flag = false;
        }
        break;
    }

    return true;
}

bool Button::update()
{
    //return false;
    return true;
}

void Button::refresh()
{
    o_label.overflow_x(dims);
    o_label.align(dims, properties.content_align, properties.padding, properties.padding);
}

};

};
