#include "ui.hpp"

// --- Weird compiler obligations. Fixed in C++17.

constexpr const UIGridEntry UIGeometry::default_grid;

// --- End Weird compiler obligations.

/*
 * =============================================================================
 * UIContainer
 * =============================================================================
 */
/*
class UIContainer : public UIWidget {
    private:
        std::vector<UIWidget> widgets;
        
    public:
        template <typename Widget, typename ...Args>
        int add<Widget>(std::string label, int options, Args &&...args);
        int add(UIWidget &&a);
        int draw();
        int update();
        int refresh();
};

template <typename Widget, typename ...Args>
int UIContainer::add<Widget>(std::string label, int options)
{
    Widget w(label, options, args...);
    add()
}*/


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

void UILabel::update()
{
}

void UILabel::refresh()
{
    o_label.overflow_x(dims);
    o_label.align(dims);
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
        g.frect(dims);
        SDL_SetTextureColorMod(o_label.texture, 0, 0, 0);
        o_label.dest_rect.y += 1;
        g.paint(o_label);
        SDL_SetTextureColorMod(o_label.texture, 255, 255, 255);
        o_label.dest_rect.y -= 1;
        break;
    }
}

void UIButton::update()
{
    switch (m.e.type) {
    case SDL_MOUSEMOTION:
        if (POINT_IN_RECT(m.e.motion.x, m.e.motion.y, dims) && state != UI_WIDGET_DOWN) {
            state = UI_WIDGET_ACTIVE;
        } else if (state != UI_WIDGET_DOWN) {
            state = UI_WIDGET_NORMAL;
        }
        break;

    case SDL_MOUSEBUTTONDOWN:
        if (POINT_IN_RECT(m.e.button.x, m.e.button.y, dims)) {
            state = UI_WIDGET_DOWN;
        }
        break;

    case SDL_MOUSEBUTTONUP:
        if (POINT_IN_RECT(m.e.button.x, m.e.button.y, dims) && state == UI_WIDGET_DOWN) {
            state = UI_WIDGET_ACTIVE;
            printf("Clicked\n");
        } else if (POINT_IN_RECT(m.e.button.x, m.e.button.y, dims)) {
            state = UI_WIDGET_ACTIVE;
        } else {
            state = UI_WIDGET_NORMAL;
        }
        break;
    }
}

void UIButton::refresh()
{
    o_label.overflow_x(dims);
    o_label.align(dims);
}

/*
 * =============================================================================
 * UIState
 * =============================================================================
 */


/*
void UIState::add(UIWidget *k)
{
    std::unique_ptr<UIWidget> p(k);
    widgets.push_back(std::move(p));
}
*/

void UIState::snap(UIGravity grav, int hpad, int vpad)
{
}

void UIState::draw()
{
    g.set_color(32, 32, 32, 255);
    g.frect(geo.container_dim);
    g.set_color(255, 255, 255, 255);
    g.rect(geo.container_dim);
    for (auto &i: widgets) {
        i->draw();
    }
}

void UIState::update()
{
    for (auto &i: widgets) {
        i->update();
    }
}

void UIState::refresh()
{
    geo.calculate_all();
    for (auto &i: widgets) {
        i->refresh();
    }
}



