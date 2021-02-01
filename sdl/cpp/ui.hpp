#ifndef UI_H
#define UI_H

#include "media.hpp"

#include <memory>
#include <vector>
#include <string>

#define UI_DEFAULT_PADDING 3
#define UI_DEFAULT_MARGIN 5
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

/**
 * Generic Event handlers
 */

namespace Behaviour {
/*
static inline bool clicked(MediaState &k, MediaRect b, UIWidgetState &b)
{
    return false;
}
*/
};

/*
 * =============================================================================
 * UIWidget
 * =============================================================================
 */

class UIWidget {
    protected:
        std::string label;
        static constexpr char const *name = "generic widget";
        MediaState &m;
        MediaGraphics &g;
        int options;

    public:
        MediaRect dims;
        int rowspan;
        int colspan;
        bool show = true;
        
        UIWidget(MediaState &m, MediaGraphics &g, std::string label, int options):
            m(m), g(g), label(label), options(options) {};
        virtual ~UIWidget() {};
        virtual void draw() = 0;
        virtual void update() = 0;
        virtual void refresh() = 0;
        virtual inline bool is_down() = 0;
        virtual inline bool is_changed() = 0;
        virtual inline char const *get_name() {
            return name;
        }
};

/// Widget list

typedef std::vector<std::unique_ptr<UIWidget>> UIWidgetList;

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
    return g.rect(k);
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

class UIGeometry {
    protected:
        UIWidgetList &widgets;

    public:
        UIGeometry (std::vector<std::unique_ptr<UIWidget>> &widgets): widgets(widgets) {}
        MediaRect container_dim;
        inline void set_container_dim(MediaRect dim);
        virtual inline void calculate_all() = 0;
};

inline void UIGeometry::set_container_dim(MediaRect dim)
{
    container_dim = dim;
    calculate_all();
}


/*
 * -----------------------------------------------------------------------------
 * GridGeometry. Default.
 * -----------------------------------------------------------------------------
 */

class UIGridGeometry : public UIGeometry {
    public:
        struct UIGridEntry {
            size_t widget_offset;
            int rows;
            int cols;
            int repeat_till; /// @todo handle this
        };
        
    private:
        inline UIGridEntry const *iter(int widget_index);
        inline void set_row_height(int h);

    public:
        static constexpr const UIGridEntry default_grid = { 0, 1, 1, 0 };
        UIGridGeometry(std::vector<std::unique_ptr<UIWidget>> &widgets): UIGeometry(widgets) {}
        std::vector<UIGridEntry> grid_list;
        MediaRect current_dim;
        int grid_index;

        inline void add(int rows, int cols, int repeat_till = 1);
        inline void set_row_height(int widget_index, int row_start, int h);
        inline void calculate_all();
};

inline void UIGridGeometry::add(int rows, int cols, int repeat_till)
{
    printf("new grid: %ld %d %d %d\n", widgets.size(), rows, cols, repeat_till);
    grid_list.emplace_back((UIGridEntry) { widgets.size(), rows, cols, repeat_till });
}

inline void UIGridGeometry::set_row_height(int widget_index, int row_start, int h)
{
    for (int i = row_start; i <= widget_index; i++) {
        widgets[i]->dims.h = h;
    }
}

inline UIGridGeometry::UIGridEntry const *UIGridGeometry::iter(int widget_index)
{
    if (grid_list.size() == 0            ||
        grid_index >= grid_list.size()   ||
        widget_index < grid_list[grid_index].widget_offset) {
        printf("Default Grid\n");
        return &default_grid;
    }

    printf("Custom Grid %d %d\n", widget_index, grid_index);
    return &grid_list[grid_index++];
}

inline void UIGridGeometry::calculate_all()
{
    UIGridEntry const *curr_grid;
    int min_grid_height;
    int row_start;
    grid_index = 0;
    current_dim = container_dim;

    for (int i = 0; i < widgets.size();) {
        curr_grid = iter(i);
        current_dim.w = container_dim.w / curr_grid->cols;
        row_start = i;
        for (int j = 0; j < curr_grid->rows; j++) {
            current_dim.x = container_dim.x;
            min_grid_height = 0;
            for (int k = 0; k < curr_grid->cols; k++) {

                if (min_grid_height < widgets[i]->dims.h) {
                    set_row_height(i, row_start, widgets[i]->dims.h);
                    min_grid_height = widgets[i]->dims.h;
                }

                widgets[i]->dims.x = UI_DEFAULT_MARGIN + current_dim.x;
                widgets[i]->dims.y = current_dim.y + UI_DEFAULT_MARGIN;
                widgets[i]->dims.w = current_dim.w - 2 * UI_DEFAULT_MARGIN;
                widgets[i]->dims.h = min_grid_height;
                
                i++;
                current_dim.x += current_dim.w;

                if (i >= widgets.size())
                    goto end;
            }
            current_dim.y += min_grid_height + UI_DEFAULT_MARGIN;
        }
    }

end:
    return;
}

/*
 * -----------------------------------------------------------------------------
 * RelativeGeometry. Positions widgets relative to the window.
 * -----------------------------------------------------------------------------
 */

class UIRelativeGeometry : public UIGeometry {
    public:
        struct GravityEntry {
            size_t widget_offset;
            UIGravity gravity;
            int hpad;
            int vpad;
        };

    private:
        inline GravityEntry const *iter(int widget_index);

    public:
        static constexpr const GravityEntry default_grav = {0, CENTER, 0, 0};
        UIRelativeGeometry(std::vector<std::unique_ptr<UIWidget>> &widgets): UIGeometry(widgets) {}
        std::vector<GravityEntry> grav_list;
        int grav_index;

        inline void add(UIGravity grav, int hpad, int vpad);
        inline void calculate_all();
};

inline void UIRelativeGeometry::add(UIGravity grav, int hpad, int vpad)
{
    grav_list.emplace_back((GravityEntry) { widgets.size(), grav, hpad, vpad });
}

inline UIRelativeGeometry::GravityEntry const *UIRelativeGeometry::iter(int widget_index)
{
    if (grav_list.size() == 0            ||
        grav_index >= grav_list.size()   ||
        widget_index < grav_list[grav_index].widget_offset) {
        printf("Default Grav\n");
        return &default_grav;
    }

    printf("Custom Grav %d %d\n", widget_index, grav_index);
    return &grav_list[grav_index++];
}

inline void UIRelativeGeometry::calculate_all() {
    GravityEntry const *current_grav;
    grav_index = 0;
    for (int i = 0; i < widgets.size(); i++) {
        current_grav = iter(i);
        Util::rect_align(container_dim, widgets[i]->dims, current_grav->gravity, current_grav->hpad, current_grav->vpad);
    }
}


/*
 * =============================================================================
 * Derived Widgets
 * =============================================================================
 */

class UILabel : public UIWidget {
    protected:
        static constexpr char const *name = "label";
        MediaClipObject o_label;

    public:
        UILabel(MediaState &m, MediaGraphics &g, std::string label, int options = 0):
            UIWidget(m, g, label, options)
        {
            g.text(o_label, label);
            dims = o_label.dest_rect;
            dims.h += 2 * UI_DEFAULT_PADDING;
        }

        ~UILabel() {
            // printf("Destructor called\n");
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

class UIButton : public UIWidget {
    protected:
        static constexpr char const *name = "label";
        MediaClipObject o_label;

    public:
        UIWidgetState state = UI_WIDGET_NORMAL;

        UIButton(MediaState &m, MediaGraphics &g, std::string label, int options = 0):
            UIWidget(m, g, label, options)
        {
            g.text(o_label, label);
            dims = o_label.dest_rect;
            dims.h += 2 * UI_DEFAULT_PADDING;
        }

        ~UIButton() {
            // printf("Destructor called\n");
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
 * The toplevel widget which holds all the other widgets.
 * =============================================================================
 */

class UITopLevel : public UIWidget {
    protected:
        static constexpr char const *name = "toplevel widget";
        // MediaState &m; /// @todo replace these in sub elements with function arg passing instead?
        // MediaGraphics &g; /// @todo replace these in sub elements with function arg passing instead?
        UIDefaultPrimitives p;
        int options;
        UIWidgetList widgets;
        
    public:
        UIRelativeGeometry geo;
        UITopLevel(MediaState &m, MediaGraphics &g, std::string label, int options):
            UIWidget(m, g, label, options),
            p(g),
            geo(widgets)
        {
            dims = {0, 0, 800, 600};
            geo.set_container_dim(dims);
            refresh();
        };
        
        template <typename Widget, typename ...Args>
        Widget &add(std::string label, int options = 0, Args &&...args);

        void resize(MediaRect dims);
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

/**
 * unique_ptr packages the allocated data pointer into a class which will
 * auto-delete the pointer when it goes out of scope.
 *
 * We then push it to the widget list vector via std::move, which gives up
 * ownership of the pointer to the vector. On destruction of the vector the
 * widget is also destructed.
 */

template <typename Widget, typename ...Args>
Widget &UITopLevel::add(std::string label, int options, Args &&...args)
{
    Widget *k = new Widget(m, g, label, options, args...);
    std::unique_ptr<UIWidget> p(k);
    widgets.push_back(std::move(p));
    return *k;
}

template <typename Geometry = UIGridGeometry>
class UIContainer : public UITopLevel {
    public:
        Geometry geo_a;
        UIContainer(MediaState &m, MediaGraphics &g, std::string label, int options):
            UITopLevel(m, g, label, options), geo_a(widgets)
        {
            dims = {0, 0, 800, 600};
            geo_a.set_container_dim(dims);
            refresh();
        };
        void draw();
        void update();
        void refresh();
        void resize(MediaRect dims);
};


template <typename Geometry>
void UIContainer<Geometry>::resize(MediaRect dims)
{
    this->dims = dims;
    refresh();
}

template <typename Geometry>
void UIContainer<Geometry>::draw()
{
    for (auto &i: widgets) {
        i->draw();
    }
}

template <typename Geometry>
void UIContainer<Geometry>::update()
{
    for (auto &i: widgets) {
        i->update();
    }
}

template <typename Geometry>
void UIContainer<Geometry>::refresh()
{
    geo_a.calculate_all();
    for (auto &i: widgets) {
        i->refresh();
    }
}

#endif
