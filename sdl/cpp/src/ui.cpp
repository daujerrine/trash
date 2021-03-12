#include "ui.hpp"

namespace media {

namespace ui {

using Util::point_in_rect;

// --- Weird compiler obligations. Fixed in C++17.

constexpr const GridGeometry::GridEntry GridGeometry::default_grid;
constexpr const RelativeGeometry::GravityEntry RelativeGeometry::default_grav;

// --- End Weird compiler obligations.

/*
 * =============================================================================
 * Widgets
 * =============================================================================
 */


/*
 * -----------------------------------------------------------------------------
 * Label
 * -----------------------------------------------------------------------------
 */

inline Rect smaller(Rect in, Rect out)
{
    if (in.w > out.w)
        return {0, 0, out.w, out.h};
    else if (in.h > out.h)
        return {0, 0, out.w, out.h};
    else
        return {0, 0, in.w, in.h};
}

inline Rect smaller_dest(Rect in, Rect out)
{
    if (in.w > out.w)
        return out;
    else if (in.h > out.h)
        return out;
    else
        return in;
}

void Label::draw()
{
    /*printf("***\n");
    PRINTRECT(o_label.src_rect);
    PRINTRECT(o_label.dest_rect);
    PRINTRECT(dims);
    PRINTRECT(smaller(o_label.dest_rect, dims));
    PRINTRECT(smaller_dest(o_label.dest_rect, dims));
    printf("***\n");*/
    g.paint(o_label);
}

bool Label::event()
{
    return true;
}

bool Label::update()
{
    return true;
}

void Label::refresh()
{
    o_label.overflow_x(dims);
    o_label.align(dims, properties.content_align);
}

void Label::set_label(std::string label)
{
    this->label = label;
    g.text(o_label, label);
    refresh();
}

/*
 * -----------------------------------------------------------------------------
 * Button
 * -----------------------------------------------------------------------------
 */

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

/*
 * -----------------------------------------------------------------------------
 * TextInput
 * -----------------------------------------------------------------------------
 */

/*
 * =============================================================================
 * TopLevel
 * =============================================================================
 */

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
