#include "ui.hpp"

// --- Weird compiler obligations. Fixed in C++17.

constexpr const UIGridGeometry::GridEntry UIGridGeometry::default_grid;
constexpr const UIRelativeGeometry::GravityEntry UIRelativeGeometry::default_grav;

// --- End Weird compiler obligations.

using MediaUtil::point_in_rect;

/*
 * =============================================================================
 * UIWidgets
 * =============================================================================
 */


/*
 * -----------------------------------------------------------------------------
 * UILabel
 * -----------------------------------------------------------------------------
 */

inline MediaRect smaller(MediaRect in, MediaRect out)
{
    if (in.w > out.w)
        return {0, 0, out.w, out.h};
    else if (in.h > out.h)
        return {0, 0, out.w, out.h};
    else
        return {0, 0, in.w, in.h};
}

inline MediaRect smaller_dest(MediaRect in, MediaRect out)
{
    if (in.w > out.w)
        return out;
    else if (in.h > out.h)
        return out;
    else
        return in;
}

void UILabel::draw()
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

bool UILabel::event()
{
    return true;
}

bool UILabel::update()
{
    return true;
}

void UILabel::refresh()
{
    o_label.overflow_x(dims);
    o_label.align(dims, properties.content_align);
}

void UILabel::set_label(std::string label)
{
    this->label = label;
    g.text(o_label, label);
    refresh();
}

/*
 * -----------------------------------------------------------------------------
 * UIButton
 * -----------------------------------------------------------------------------
 */

void UIButton::draw()
{
    switch (state) {
    case UI_WIDGET_NORMAL:
        g.set_color(255, 255, 255, 255);
        g.rect(dims);
        g.paint(o_label);
        break;

    case UI_WIDGET_ACTIVE:
        g.set_color(80, 80, 80, 255);
        g.frect(dims);
        g.set_color(255, 255, 255, 255);
        g.rect(dims);
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

bool UIButton::event()
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

bool UIButton::update()
{
    //return false;
    return true;
}

void UIButton::refresh()
{
    o_label.overflow_x(dims);
    o_label.align(dims, RIGHT, properties.padding, properties.padding);
}

/*
 * =============================================================================
 * UITopLevel
 * =============================================================================
 */

bool UITopLevel::event()
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

    return UIContainer::event();
}

void UIFrame::draw()
{
    g.set_color(40, 40, 40, 255);
    g.frect(dims);
    g.set_color(255, 255, 255, 255);
    g.rect(dims);
    for (auto &i: widgets)
        i->draw();
}
