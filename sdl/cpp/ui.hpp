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
        std::string label;
        MediaState &m;
        MediaGraphics &g;
        int options;
        static constexpr char const *name = "generic widget";

    public:
        MediaRect dims;
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
        virtual inline char const *get_name() {
            return name;
        }
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
    long unsigned int widget_offset;
    int rows;
    int cols;
    int repeat_till; /// @todo handle
};

class UIGeometry {
    private:
        std::vector<std::unique_ptr<UIWidget>> &widgets;
        static constexpr const UIGridEntry default_grid = { 0, 1, 1, 0 };

    public:
        UIGeometry (std::vector<std::unique_ptr<UIWidget>> &widgets): widgets(widgets) {}
        std::vector<UIGridEntry> grid_list;
        MediaRect container_dim;
        MediaRect current_dim;
        int widget_index;
        int grid_index;
        int cell_index;

        inline void grid(int rows, int cols, int repeat_till = 1);
        inline UIGridEntry const *iter(int widget_index);
        inline void reset();
        inline void set_container_dim(MediaRect dim);
        inline void set_row_height(int h);
        inline void calculate_all();
};

inline void UIGeometry::grid(int rows, int cols, int repeat_till)
{
    grid_list.emplace_back((UIGridEntry) { widgets.size(), rows, cols, repeat_till });
}

inline void UIGeometry::reset()
{
    widget_index = 0;
    grid_index = 0;
}

inline void UIGeometry::set_container_dim(MediaRect dim)
{
    container_dim = dim;
}

inline void UIGeometry::set_row_height(int h)
{
    for (int i = widget_index - cell_index;
         i < widget_index + grid_list[widget_index].cols;
         i++) {
        widgets[i]->dims.h = h;
    }
}

inline UIGridEntry const *UIGeometry::iter(int widget_index) {
    if (grid_list.size() == 0            ||
        grid_index >= grid_list.size()   ||
        widget_index < grid_list[grid_index].widget_offset) {
        printf("Default Grid\n");
        return &default_grid;
    }

    printf("Custom Grid %d %d\n", widget_index, grid_index);
    return &grid_list[grid_index++];
}

inline void UIGeometry::calculate_all() {
    UIGridEntry const *curr_grid;
    int min_grid_height = 0;
    current_dim = container_dim;
    PRINT_LINE
    printf("widget size: %ld\n", widgets.size());
    for (int i = 0; i < widgets.size();) {
        curr_grid = iter(i);
        current_dim.w = container_dim.w / curr_grid->cols;
        printf("Entering loop: grid: %dr x %dc\n", curr_grid->rows, curr_grid->cols);
        for (int j = 0; j < curr_grid->rows; j++) {
            printf("q\n");
            current_dim.x = container_dim.x;
            for (int k = 0; k < curr_grid->cols; k++) {

                min_grid_height = min_grid_height > widgets[i]->dims.h ?
                                  min_grid_height :
                                  widgets[i]->dims.h;

                printf("k = %d j = %d %s\n", k, j, widgets[i]->get_name());

                widgets[i]->dims.x = UI_DEFAULT_PADDING + current_dim.x;
                widgets[i]->dims.y = current_dim.y + UI_DEFAULT_PADDING;
                widgets[i]->dims.w = current_dim.w - 2 * UI_DEFAULT_PADDING;
                widgets[i]->dims.h = min_grid_height + 1;

                printf("(%d, %d, %d, %d)\n",
                    widgets[i]->dims.x,
                    widgets[i]->dims.y,
                    widgets[i]->dims.w,
                    widgets[i]->dims.h);
                
                i++;
                current_dim.x += current_dim.w;

                if (i >= widgets.size())
                    goto end;
            }
            current_dim.y += min_grid_height + UI_DEFAULT_PADDING;
        }
    }

end:
    // REFRESH HERE
    return;
}


/*
 * =============================================================================
 * Derived Widgets
 * =============================================================================
 */

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
        
        UILabel(MediaState &m, MediaGraphics &g, std::string label, int options = 0):
            UIWidget(m, g, label, options)
        {
            g.text(tx_label, label);
            dims = tx_label.clip_rect;
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


/*
 * =============================================================================
 * Factory function?
 * =============================================================================
 */

/// @todo Replace with UIContainerWidget?

class UIState {
    private:    
        MediaState &m;
        MediaGraphics &g;
        UIDefaultPrimitives p;
        MediaRect dims = {100, 100, 400, 400};
        int options;
        std::vector<std::unique_ptr<UIWidget>> widgets;

    public:
        UIGeometry geo;
        UIState (MediaState &m, MediaGraphics &g): m(m), g(g), p(g), geo(widgets) {
            geo.set_container_dim(dims);
            refresh();
        };
        UIContainer &add_container();
        template <typename Widget, typename ...Args>
        Widget &add(std::string label, int options = 0, Args &&...args);
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
