#ifndef UI_H
#define UI_H

#include "media.hpp"

#include <memory>
#include <vector>
#include <string>

#define UI_DEFAULT_PADDING 10
#define UI_DEFAULT_MARGIN 10
#define UI_DEFAULT_MIN_WIDTH 30
#define UI_DEFAULT_MIN_HEIGHT 30

#define XCENTER_RECT(_fg, _bg) ((_bg).x + (_bg).w / 2 - (_fg).w / 2)
#define YCENTER_RECT(_fg, _bg) ((_bg).y + (_bg).h / 2 - (_fg).h / 2)

#define CENTER_RECT(_fg, _bg) ((MediaRect) {\
        .x = XCENTER_RECT((_fg), (_bg)),\
        .y = YCENTER_RECT((_fg), (_bg)),\
        .w = (_fg).w,\
        .h = (_fg).h})

#define POINT_IN_RECT(_x, _y, _rect) (\
    (_x) >= (_rect).x &&\
    (_y) >= (_rect).y &&\
    (_x) <= (_rect).x + (_rect).w &&\
    (_y) <= (_rect).y + (_rect).h)


// Options of a widget

enum UIOptions {
    UI_OPTION_NOCLIP = 0x1,
    UI_OPTION_NOSTRETCH = 0x2,
};

enum UIWidgetState {
    UI_WIDGET_NORMAL,
    UI_WIDGET_ACTIVE,
    UI_WIDGET_DOWN,
    UI_WIDGET_CHANGED,
    UI_WIDGET_DISABLED
};

/// Widget Gravity
typedef MediaGravity UIGravity;

/*
 * =============================================================================
 * UIWidget
 * =============================================================================
 */

class UIWidget {
    protected:
        static constexpr char const *name = "generic widget";
        const std::string label;
        MediaRect dims;
        MediaState &m;
        MediaGraphics &g;
        int options;

    public:
        int rowspan;
        int colspan;
        bool skip;
        
        UIWidget(MediaState &m, MediaGraphics &g, std::string label, int options):
            m(m), g(g), label(label), options(options) {};
        virtual void draw() = 0;
        virtual void update() = 0;
        virtual void refresh() = 0;
        virtual inline bool is_down() = 0;
        virtual inline bool is_changed() = 0;
};

/*
 * =============================================================================
 * UIPrimitives
 * =============================================================================
 */

/// Primitives used for drawing the GUI components.
class UIPrimitives {
    protected:
        MediaGraphics &g;
        int padding;
        int margin;
        int min_height;
        int min_width;

    public:
        UIPrimitives(MediaGraphics &g): g(g) {};
        virtual inline int box(UIWidget &u, MediaRect k) = 0;
        virtual inline int fbox(UIWidget &u, MediaRect k) = 0;
        virtual inline int line(UIWidget &u, int x1, int x2, int y1, int y2) = 0;
};

class UIDefaultPrimitives : public UIPrimitives {
    protected:
        int padding    = UI_DEFAULT_PADDING;
        int margin     = UI_DEFAULT_MARGIN;
        int min_height = UI_DEFAULT_MIN_HEIGHT;
        int min_width  = UI_DEFAULT_MIN_WIDTH;

    public:
        UIDefaultPrimitives(MediaGraphics &g): UIPrimitives(g) {}
        inline int box(UIWidget &u, MediaRect k);
        inline int fbox(UIWidget &u, MediaRect k);
        inline int line(UIWidget &u, int x1, int x2, int y1, int y2);
};

inline int UIDefaultPrimitives::box(UIWidget &u, MediaRect k)
{
    return g.rect(k);
}

inline int UIDefaultPrimitives::fbox(UIWidget &u, MediaRect k)
{
    return g.frect(k);
}

inline int UIDefaultPrimitives::line(UIWidget &u, int x1, int y1, int x2, int y2)
{
    return g.line(x1, y1, x2, y2);
}

/*
 * =============================================================================
 * Widget Geometry Management
 * =============================================================================
 */

struct UIGridEntry {
    int widget_offset;
    int rows;
    int cols;
    int repeat_till;
};

class UIGeometry {
    private:
        std::vector<std::unique_ptr<UIWidget>> &widgets;
        
    public:
        std::vector<UIGridEntry> grid_list;
        MediaRect container_dim;
        MediaRect current_dim;
        int widget_index;
        int grid_index;
        int cell_index;

        inline void grid(int widget_offset, int rows, int cols, int repeat_till = 1);
        inline void reset();
        inline void get_rect(int widget_index);
        inline void set_container_dim(MediaRect dim);
        inline void set_row_height(int h);
        inline void calculate_all();
};

inline void UIGeometry::grid(int widget_offset, int rows, int cols, int repeat_till)
{
    grid_list.emplace_back({ widget_offset, rows, cols, repeat_till });
}

inline void UIGeometry::reset()
{
    widget_index = 0;
    grid_index = 0;
}

inline MediaRect UIGeometry::get_rect(int widget_offset)
{
    return widgets[widget_offset].dims;
}

inline void UIGeometry::set_row_height(int h)
{
    for (int i = widget_index - cell_index;
         i < widget_index + grid_list[widget_index].cols;
         i++) {
        widgets[i].dims.h = h;
    }
}

inline UIGridEntry &UIGeometry::iter(int widget_index) {
    if (g->grid_list_top == 0                  ||
        g->grid_list_index >= g->grid_list_top ||
        widget_index < g->grid_list[g->grid_list_index].widget_offset) {
        printf("Default Grid\n");
        return &default_grid;
    }

    printf("Custom Grid %d %d\n", g->grid_list[g->grid_list_index].widget_offset, widget_index);
    return &g->grid_list[g->grid_list_index++];
}

inline void calculate_all() {
    for (int i = 0; i < s->num_widgets;) {
        curr_grid = iter_grid(g, i);
        g->current_dim.w = g->container_dim.w / curr_grid->cols;
        printf("Entering loop: grid: %dr x %dc\n", curr_grid->rows, curr_grid->cols);
        for (int j = 0; j < curr_grid->rows; j++) {
            printf("q\n");
            g->current_dim.x = g->container_dim.x;
            for (int k = 0; k < curr_grid->cols; k++) {

                min_grid_height = min_grid_height > s->widget_list[i].dims.h ? min_grid_height : s->widget_list[i].dims.h;

                printf("k = %d j = %d %s\n", k, j, s->widget_list[i].label);

                s->widget_list[i].dims.x = DEFAULT_PADDING + g->current_dim.x;
                s->widget_list[i].dims.y = g->current_dim.y + DEFAULT_PADDING;
                s->widget_list[i].dims.w = g->current_dim.w - 2 * DEFAULT_PADDING;
                s->widget_list[i].dims.h = min_grid_height + 1;

                printf("(%d, %d, %d, %d)\n",
                    s->widget_list[i].dims.x,
                    s->widget_list[i].dims.y,
                    s->widget_list[i].dims.w,
                    s->widget_list[i].dims.h);
                
                i++;
                g->current_dim.x += g->current_dim.w;

                if (i >= s->num_widgets)
                    goto end;
            }
            g->current_dim.y += min_grid_height + DEFAULT_PADDING;
        }
    }

end:
    ui_refesh_widgets(s);
    return;
}


class UIContainer : public UIWidget {
    private:
        UIGeometry gm;
        std::vector<UIWidget> widgets;
        
    public:
        template <typename Widget, typename ...Args>
        Widget &add(std::string label, int options, Args &&...args);
        void add(UIWidget *a);
        void draw();
        void update();
        void refresh();
};

class UILabel : public UIWidget {
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
            tx_label.scale(2,2);
        }

        void draw();
        void update();
        void refresh();

        inline bool is_down()
        {
            return false;
        }

        inline bool is_changed()
        {
            return false;
        }
};

/// @todo Replace with UIContainerWidget?

class UIState {
    private:    
        MediaState &m;
        MediaGraphics &g;
        UIDefaultPrimitives p;
        UIGeometry gm;
        MediaRect dims = {100, 100, 400, 400};
        int options;
        std::vector<std::unique_ptr<UIWidget>> widgets;

    public:
        UIState (MediaState &m, MediaGraphics &g): m(m), g(g), p(g) {};
        UIContainer &add_container();
        template <typename Widget, typename ...Args>
        Widget &add(std::string label, int options, Args &&...args);
        // UIWidget &add(UIWidget &&k);
        void snap(UIGravity grav, int hpad = UI_DEFAULT_PADDING, int vpad = UI_DEFAULT_PADDING);
        void draw();
        void update();
        void refresh();
};

/**
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
    Widget *k = new Widget(m, g, label, options, args...);
    std::unique_ptr<UIWidget> p(k);
    widgets.push_back(std::move(p));
    return *k;
}


#endif
