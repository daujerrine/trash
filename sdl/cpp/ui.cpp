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
        return in;
}

void UILabel::draw()
{
    g.paint(tx_label, smaller(tx_label.clip_rect, dims), tx_label.clip_rect);
}

void UILabel::update()
{
}

void UILabel::refresh()
{
    PRINT_LINE
    tx_label.align(dims);
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
    g.rect(dims);
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



