#ifndef MEDIA_UI_WIDGET_BUTTON_H
#define MEDIA_UI_WIDGET_BUTTON_H

#include "ui/widget/common.hpp"

namespace media {

namespace ui {

class Button : public Widget {
    protected:
        static constexpr char const *name = "button";
        ClipObject o_label;
        bool clicked_flag = false;

    public:
        WidgetState state = UI_WIDGET_NORMAL;

        Button(State &m, Graphics &g, std::string label, int options = 0):
            Widget(m, g, label, options)
        {
            g.text(o_label, label);
            dims = o_label.dest_rect;
            dims.h += 2 * UI_DEFAULT_PADDING;
            PRINT_LINE
            PRINTRECT(dims);
        }

        ~Button() {
            // printf("Destructor called\n");
        }

        void draw();
        bool event();
        bool update();
        void refresh();

        inline bool is_down()
        {
            return clicked_flag;
        }

        inline bool is_changed()
        {
            return false;
        }
};

};

};

#endif
