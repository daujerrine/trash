#ifndef MEDIA_UI_WIDGET_LABEL_H
#define MEDIA_UI_WIDGET_LABEL_H

#include "media/media.hpp"
#include "common.hpp"

namespace media {

namespace ui {

class Label : public Widget {
    protected:
        static constexpr char const *name = "label";
        ClipObject o_label;

    public:
        Label(State &m, Graphics &g, std::string label, int options = 0):
            Widget(m, g, label, options)
        {
            g.text(o_label, label);
            // printf("LABELINIT::: "); PRINTRECT(o_label.dest_rect);
            dims = o_label.dest_rect;
            dims.h += 2 * UI_DEFAULT_PADDING; /// @todo remove this
            PRINT_LINE
            PRINTRECT(dims);
        }

        ~Label() {
            // printf("Destructor called\n");
        }

        void draw();
        bool event();
        bool update();
        void refresh();

        inline bool is_down()
        {
            return false;
        }

        inline bool is_changed()
        {
            return false;
        }

        void set_label(std::string label);
};

};

};

#endif
