#include "ui.hpp"

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

class UILabel : UIWidget {
    protected:
        static constexpr char const *name = "label";
        MediaObject tx_label;

    public:
        int rowspan;
        int colspan;
        bool skip;
        
        UILabel(MediaState &m, MediaGraphics &g, std::string label, int options):
            UIWidget(m, g, label, options)
        {
            g.text(tx_label, label);
        }

        int draw();
        int update();
        int refresh();

        inline bool is_down()
        {
            return false;
        }

        inline bool is_changed()
        {
            return false;
        }
};

int UILabel::draw()
{
    g.paint(tx_label);
    return 1;
}

int UILabel::update()
{
    return 1;
}

int UILabel::refresh() {
    tx_label.align(dims);
    return 1;
}

/*
 * =============================================================================
 * UIState
 * =============================================================================
 */

/*
 * unique_ptr packages the allocated data pointer into a class which will
 * auto-delete the pointer when it goes out of scope.
 *
 * We then push it to the widget list vector via std::move, which gives up
 * ownership of the pointer to the vector. On destruction of the vector the
 * widget is also destructed.
 */

template <typename Widget, typename ...Args>
Widget &UIState::add(std::string label, int options, Args &&...args)
{
    Widget *k = new Widget(args...);
    std::unique_ptr<UIWidget> p(k);
    widgets.push_back(std::move(p));
    return *k;
}

/*
void UIState::add(UIWidget *k)
{
    std::unique_ptr<UIWidget> p(k);
    widgets.push_back(std::move(p));
}
*/

int UIState::snap(UIGravity grav, int hpad, int vpad)
{
    return 1;
}

int UIState::draw()
{
    for (auto &i: widgets) {
        i->draw();
    }
    return 1;
}

int UIState::update()
{
    for (auto &i: widgets) {
        i->update();
    }
    return 1;
}

int UIState::refresh()
{
    for (auto &i: widgets) {
        i->refresh();
    }
    return 1;
}



