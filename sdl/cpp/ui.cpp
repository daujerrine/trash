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
    PRINT_LINE
    MediaDims k = o_label.dims();
    printf("## %d %d\n", k.w, dims.w);
    if ( o_label.dest_rect.w > dims.w ||
        ((o_label.src_rect_ptr != nullptr) && (k.w > dims.w))) {
        o_label.clip(dims.w, dims.h);
    }
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



