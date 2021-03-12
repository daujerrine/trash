#ifndef _H
#define _H

#include "media.hpp"

#include <memory>
#include <vector>
#include <string>

#define UI_DEFAULT_PADDING 3
#define UI_DEFAULT_MARGIN 5
#define UI_DEFAULT_MIN_WIDTH 30
#define UI_DEFAULT_MIN_HEIGHT 30

#define UI_OPTION_DEF_MAX 24
#define UI_OPTION_DEF_PARENT_MAX 32
#define UI_OPTION_DEF(x) (1 << (x))
#define UI_OPTION_DEF_PARENT(x) (UI_OPTION_DEF(x) << UI_OPTION_DEF_MAX)


namespace media {

namespace ui {

enum Options {
    OPT_WIDGET_NOCLIP    = UI_OPTION_DEF_PARENT(0),
    OPT_WIDGET_STRETCH   = UI_OPTION_DEF_PARENT(1),
    OPT_WIDGET_FIXED     = UI_OPTION_DEF_PARENT(2),
    /// Have properties been set?
    OPT_WIDGET_PROPS_SET = UI_OPTION_DEF(0),
};

enum WidgetState {
    UI_WIDGET_NORMAL,
    UI_WIDGET_ACTIVE,
    UI_WIDGET_DOWN,
    UI_WIDGET_CHANGED,
    UI_WIDGET_DISABLED
};

enum EventID {
    UI_BUTTON_CLICKED
};

/// Widget Gravity
typedef Gravity Gravity;

/**
 * Generic Event handlers
 */

namespace Behaviour {
/*
static inline bool clicked(State &k, Rect b, WidgetState &b)
{
    return false;
}
*/
};

class Widget;

/*
 * =============================================================================
 * Primitives
 * =============================================================================
 */

/// Primitives used for drawing the G components.
class Primitives {
    protected:
        Graphics &g;
        int padding;
        int margin;
        int min_height;
        int min_width;

    public:
        Primitives(Graphics &g): g(g) {};
        virtual inline int box(Rect k) = 0;
        virtual inline int fbox(Rect k) = 0;
        virtual inline int line(Widget &u, int x1, int x2, int y1, int y2) = 0;
};

class DefaultPrimitives : public Primitives {
    protected:
        int padding    = UI_DEFAULT_PADDING;
        int margin     = UI_DEFAULT_MARGIN;
        int min_height = UI_DEFAULT_MIN_HEIGHT;
        int min_width  = UI_DEFAULT_MIN_WIDTH;

    public:
        DefaultPrimitives(Graphics &g): Primitives(g) {}
        inline int box(Rect k);
        inline int fbox(Rect k);
        inline int line(Widget &u, int x1, int x2, int y1, int y2);
};

inline int DefaultPrimitives::box(Rect k)
{
    return g.rect(k);
}

inline int DefaultPrimitives::fbox(Rect k)
{
    g.set_color(128, 128, 128, 255);
    g.frect(k);
    g.set_color(255, 255, 255, 255);
    return g.rect(k);
}

inline int DefaultPrimitives::line(Widget &u, int x1, int y1, int x2, int y2)
{
    return g.line(x1, y1, x2, y2);
}

struct WidgetProperties {
    std::string tooltip;
    // Primitives *style;
    Gravity content_align = CENTER;
    Size min_size      = { 0, 0 };
    Size max_size      = { 100000, 100000 };
    Color bg_color     = { 110, 110, 110  };
    Color fg_color     = { 255, 255, 255, };
    int padding             = UI_DEFAULT_PADDING;
    int margin              = UI_DEFAULT_MARGIN;
    int scale               = 1;
};

/*
WidgetProperties ui_default_properties = (WidgetProperties) {
    .content_align = CENTER,
    .min_size      = { 0, 0 },
    .max_size      = { 100000, 100000 },
    .bg_color      = { 110, 110, 110  },
    .fg_color      = { 255, 255, 255, },
    .padding       = _DEFAULT_PADDING,
    .margin        = _DEFAULT_MARGIN,
    .scale         = 1
};
*/

/*
 * =============================================================================
 * State
 * =============================================================================
 */

/**
 * Context variables included in every widget
 * @todo Pass as argument instead?
 */
/*
struct State {
    State    &m;
    Graphics &g;
    Primitives  &p;
    State(State &m, Graphics &g, Primitives &p): m(m), g(g), p(p) {}
};
*/

/*
 * =============================================================================
 * Widget
 * =============================================================================
 */

/// Default Abstract widget class
class Widget {
    protected:
        std::string label; /// Display Name
        static constexpr char const *name = "generic widget"; /// Class Name
        State &m;
        Graphics &g;
        DefaultPrimitives p;

        /**
         * Several Drawing options.
         * The first two bytes from the LSB are reserved for the widget's
         * options itself. The last 2 bytes are reserved for the parent widget.
         *
         * Parent - Parent - Self - Self
         *
         * The parent component can hold flags that are specific to itself, i.e.
         * declare flags local to itself and its geometry manager.
         */
        uint32_t options;

        /// Storage flag for whether a refresh request has been made or not.
        bool no_refresh_flag = true;

        /// Should the widget be drawn to the screen?
        bool show_flag = true;

        /// Sets the refresh flag
        inline void request_refresh()
        {
            no_refresh_flag = false;
        }

    public:
        WidgetProperties properties;
        /**
         * "Ideal" dimensions of the given widget.
         * First set by the widget itself and then handled by the geometry
         * manager on invoking calculation.
         */
        Rect dims;
        
        Widget(State &m, Graphics &g, std::string label, int options):
            m(m), g(g), p(g), label(label), options(options) {}

        virtual ~Widget() {};

        /// Recalculates widget geometry.
        virtual void refresh() = 0;

        /// Draws the widget to the screen.
        virtual void draw() = 0;

        /// Processes events for the given widget,
        /// @return false if refresh() needs to be invoked by the parent widget. True otherwise.
        virtual bool event() = 0;

        /// Updates the widget.
        /// @return false if refresh() needs to be invoked by the parent widget. True otherwise.
        virtual bool update() = 0;

        /**
         * Gets the request and resets it afterwards.
         */
        inline bool get_refresh_request()
        {
            bool ret = no_refresh_flag;
            no_refresh_flag = false;
            return ret;
        }

        inline bool shown()
        {
            return show_flag;
        }
        
        inline void show()
        {
            show_flag = true;
        }

        inline void hide()
        {
            show_flag = false;
        }

        virtual inline bool is_down() = 0;
        virtual inline bool is_changed() = 0;

        virtual void set_label(std::string label)
        {
            this->label = label;
        }
        
        virtual inline char const *get_name()
        {
            return name;
        }
};

/// Widget list alias
typedef std::vector<std::unique_ptr<Widget>> WidgetList;

/*
 * =============================================================================
 * Widget Geometry Management
 * =============================================================================
 */

class Geometry {
    protected:
        WidgetList &widgets;
        WidgetProperties &properties;

    public:
        Geometry (
            WidgetList &widgets,
            WidgetProperties &properties
        ): widgets(widgets), properties(properties) {}
        Rect container_dim  = {0, 0, 0, 0};

        virtual Rect calculate_all(Rect dims) = 0;
        virtual Rect update_container_dim(Rect dims) = 0;
};

/*
 * -----------------------------------------------------------------------------
 * GridGeometry. Default.
 * -----------------------------------------------------------------------------
 */

class GridGeometry : public Geometry {
    public:
        struct GridEntry {
            size_t widget_offset;
            int rows;
            int cols;
            int repeat_till; /// @todo handle this
            int options;
        };
        
    private:
        inline GridEntry const *iter(int widget_index);
        inline void set_row_height(int h);

    public:
        static constexpr const GridEntry default_grid = { 0, 1, 1, 0 };
        GridGeometry(
            WidgetList &widgets,
            WidgetProperties &properties
        ): Geometry(widgets, properties) {}
        std::vector<GridEntry> grid_list;
        int grid_index;
        bool initial_refresh = true;
        Rect c = {0, 0, 0, 0};

        inline void add(int rows, int cols, int repeat_till = 1);
        inline void set_row_height(int widget_index, int row_start, int h);

        inline Rect calculate_all(Rect dims);
        inline Rect translate_all(Point pos);
        inline Rect update_container_dim(Rect dims);
};

inline void GridGeometry::add(int rows, int cols, int repeat_till)
{
    //printf("new grid: %ld %d %d %d\n", widgets.size(), rows, cols, repeat_till);
    grid_list.emplace_back((GridEntry) { widgets.size(), rows, cols, repeat_till });
}

inline void GridGeometry::set_row_height(int widget_index, int row_start, int h)
{
    for (int i = row_start; i <= widget_index; i++) {
        widgets[i]->dims.h = h;
    }
}

inline GridGeometry::GridEntry const *GridGeometry::iter(int widget_index)
{
    if (grid_list.size() == 0          ||
        grid_index >= grid_list.size() ||
        widget_index < grid_list[grid_index].widget_offset) {
        //printf("Default Grid\n");
        return &default_grid;
    }

    //printf("Custom Grid %d %d\n", widget_index, grid_index);
    return &grid_list[grid_index++];
}

inline Rect GridGeometry::calculate_all(Rect new_dim)
{
    GridEntry const *curr_grid;
    int min_grid_height;
    int row_start;
    grid_index = 0;
    Rect current_dim = new_dim;
    current_dim.y += properties.padding;

    for (int i = 0; i < widgets.size();) {
        // PRINT_LINE
        curr_grid = iter(i);
        current_dim.w = new_dim.w / curr_grid->cols;
        row_start = i;

        for (int j = 0; j < curr_grid->rows; j++) {
            int padding = widgets[i]->properties.padding;
            int margin  = widgets[i]->properties.margin;
            current_dim.x = new_dim.x;
            min_grid_height = 0;

            for (int k = 0; k < curr_grid->cols; k++) {
                if (min_grid_height < widgets[i]->dims.h) {
                    set_row_height(i, row_start, widgets[i]->dims.h);
                    min_grid_height = widgets[i]->dims.h;
                }

                widgets[i]->dims.x = current_dim.x;
                widgets[i]->dims.y = current_dim.y;

                if (k == 0) {
                    widgets[i]->dims.x += margin;
                    // printf(":: %d\n", margin);
                } else {
                    widgets[i]->dims.x += margin - margin / 2;
                    // printf(":: %d\n", margin - margin / 2);
                }

                if ((k == (curr_grid->cols - 1)) && (k == 0)) {
                    widgets[i]->dims.w = current_dim.w - margin * 2;
                    // printf("Only element\n");
                } else if ((k == (curr_grid->cols - 1))) {
                    widgets[i]->dims.w = new_dim.w - (widgets[i]->dims.x - new_dim.x) - margin;
                    // PRINTRECT(current_dim);
                    // printf("j = %d k = %d last newdim.w = %d, w = %d x = %d\n",
                    //        j,  k, new_dim.w, widgets[i]->dims.w, widgets[i]->dims.x);
                } else if ((k == 0)) {
                    widgets[i]->dims.w = current_dim.w - margin / 2 - margin;
                    //     printf("j = %d k = %d first newdim.w = %d, w = %d x = %d\n",
                    //        j,  k, new_dim.w, widgets[i]->dims.w, widgets[i]->dims.x);
                } else {
                    widgets[i]->dims.w = current_dim.w - margin;
                    // printf("j = %d k = %d Middle newdim.w = %d, w = %d x = %d\n",
                    //        j,  k, new_dim.w, widgets[i]->dims.w, widgets[i]->dims.x);
                }

                widgets[i]->dims.h = min_grid_height;

                current_dim.x += current_dim.w;

                i++;
                if (i >= widgets.size())
                    goto end;
            }
            current_dim.y += min_grid_height + margin;
        }
    }

end:
    current_dim.y += min_grid_height + UI_DEFAULT_MARGIN * 3;
    // printf("Recommended size: <>%d %d %d %d\n", new_dim.y, current_dim.y, new_dim.w, current_dim.y - new_dim.y);
    // printf("Container size: %d %d\n", new_dim.w, new_dim.h);

    container_dim = {new_dim.x, new_dim.y, new_dim.w, current_dim.y - new_dim.y};
    return container_dim;
}

inline Rect GridGeometry::translate_all(Point dims)
{
    int diffx = dims.x - container_dim.x;
    int diffy = dims.y - container_dim.y;

    for (int i = 0; i < widgets.size(); i++) {
        widgets[i]->dims.x += diffx;
        widgets[i]->dims.y += diffy;
    }

    container_dim.x = dims.x;
    container_dim.y = dims.y;

    return container_dim;
}


/**
 * @note Here we check whether the container has been simply translated from
 *       one point to another or not. If so, we simply translate all the
 *       container's coordinates.
 */
inline Rect GridGeometry::update_container_dim(Rect new_dim)
{
    if (container_dim.w == new_dim.w && container_dim.h == new_dim.h) {
        //printf(">>>>>>>> translate\n");
        return translate_all({new_dim.x, new_dim.y});
    } else {
        //printf(">>>>>>>> resize\n");
        return calculate_all(new_dim);
    }
}

/*
 * -----------------------------------------------------------------------------
 * RelativeGeometry. Positions widgets relative to the window.
 * -----------------------------------------------------------------------------
 */

class RelativeGeometry : public Geometry {
    public:
        struct GravityEntry {
            size_t widget_offset;
            Gravity gravity;
            int hpad;
            int vpad;
        };

    private:
        inline GravityEntry const *iter(int widget_index);

    public:
        static constexpr const GravityEntry default_grav = {0, CENTER, 0, 0};
        RelativeGeometry(
            std::vector<std::unique_ptr<Widget>> &widgets,
            WidgetProperties &properties
        ): Geometry(widgets, properties) {}
        std::vector<GravityEntry> grav_list;
        int grav_index;

        inline void add(Gravity grav, int hpad, int vpad);
        inline Rect calculate_all(Rect new_dim);
        inline Rect update_container_dim(Rect new_dim);
};

inline void RelativeGeometry::add(Gravity grav, int hpad, int vpad)
{
    grav_list.emplace_back((GravityEntry) { widgets.size(), grav, hpad, vpad });
}

inline RelativeGeometry::GravityEntry const *RelativeGeometry::iter(int widget_index)
{
    if (grav_list.size() == 0            ||
        grav_index >= grav_list.size()   ||
        widget_index < grav_list[grav_index].widget_offset) {
        // printf("Default Grav\n");
        return &default_grav;
    }

    // printf("Custom Grav %d %d\n", widget_index, grav_index);
    return &grav_list[grav_index++];
}

inline Rect RelativeGeometry::calculate_all(Rect new_dim)
{
    GravityEntry const *current_grav;
    grav_index = 0;
    //printf("RELGEO\n");
    //PRINTRECT(container_dim);
    container_dim = new_dim;
    for (int i = 0; i < widgets.size(); i++) {
        current_grav = iter(i);
        widgets[i]->dims = Util::rect_align(
            container_dim, widgets[i]->dims, current_grav->gravity,
            current_grav->hpad, current_grav->vpad);
            //PRINTRECT(widgets[i]->dims);
    }

    return container_dim;
}

inline Rect RelativeGeometry::update_container_dim(Rect new_dim)
{
    return calculate_all(new_dim);
}


/*
 * =============================================================================
 * Derived Widgets
 * =============================================================================
 */

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


/*
 * =============================================================================
 * Container
 * =============================================================================
 */

template <typename GeometryT>
class Container : public Widget {
    protected:
        static constexpr char const *name = "container";
        // DefaultPrimitives &p;
        WidgetList widgets;
        DefaultPrimitives p;
        
    public:
        GeometryT geo;

        Container(
            State &m,
            Graphics &g,
            std::string label,
            int options = 0,
            Rect dims = {0, 0, 0, 0}
        ): Widget(m, g, label, options), geo(widgets, properties), p(g)
        {
            PRINT_LINE
            printf("Initialiser called.\n");
            this->dims = dims;
            // this->dims = geo.update_container_dim(dims);
            // refresh();
            //printf("EAEAEAEAEAE\n");
            //PRINTRECT(this->dims);
        }

        virtual void draw();
        virtual bool event();
        virtual bool update();
        virtual void refresh();
        virtual void resize(Rect dims);

        virtual inline bool is_down()
        {
            return false;
        }

        virtual inline bool is_changed()
        {
            return false;
        }

        /**
         * Adds a widget to the container.
         * 
         * unique_ptr packages the allocated data pointer into a class which
         * will auto-delete the pointer when it goes out of scope.
         *
         * We then push it to the widget list vector via std::move, which gives
         * up ownership of the pointer to the vector. On destruction of the
         * vector the widget is also destructed.
         */

        template <typename WidgetT, typename ...Args>
        WidgetT &add(std::string label, int options = 0, Args &&...args)
        {
            WidgetT *k = new WidgetT(m, g, label, options, args...);
            std::unique_ptr<WidgetT> p(k);
            widgets.push_back(std::move(p));
            return *k;
        }
};

template <typename Geometry>
void Container<Geometry>::resize(Rect dims)
{
    this->dims = geo.update_container_dim(dims);
    refresh();
}

template <typename Geometry>
void Container<Geometry>::draw()
{
    for (auto &i: widgets) {
        if (i->shown())
            i->draw();
    }
}

template <typename Geometry>
bool Container<Geometry>::event()
{
    bool no_refresh = true;
    for (auto &i: widgets) {
        // Order important for optimisation
        if (i->shown())
            no_refresh = i->event() && no_refresh;
    }

    if (!no_refresh) {
        refresh();
        return false;
    }
    return true;
}

template <typename Geometry>
bool Container<Geometry>::update()
{
    bool no_refresh = true;

    for (auto &i: widgets) {
        // Order important for optimisation
        if (i->shown())
            no_refresh = i->update() && no_refresh;
    }

    if (!no_refresh) {
        refresh();
        return false;
    }

    return true;
}

template <typename Geometry>
void Container<Geometry>::refresh()
{
    dims = geo.update_container_dim(dims);
    for (auto &i: widgets)
        i->refresh();
}

/*
 * =============================================================================
 * TopLevel
 * =============================================================================
 */

class TopLevel : public Container<RelativeGeometry> {
    protected:
        static constexpr char const *name = "toplevel";
        
    public:
        TopLevel(
            State &m,
            Graphics &g,
            std::string label,
            int options = 0,
            Rect dims = {0, 0, 0, 0}
        ): Container(m, g, label, options, dims) {}
        
        bool event();
};


/*
 * ============================================================================
 * Frame
 * =============================================================================
 */

class Frame : public Container<GridGeometry> {
    protected:
        static constexpr char const *name = "frame";
        
    public:
        Frame(
            State &m,
            Graphics &g,
            std::string label,
            int options = 0,
            Rect dims = {0, 0, 0, 0}
        ): Container(m, g, label, options, dims) {}

        void draw();
};

};

};

#endif
