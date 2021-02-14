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

/// Default Abstract widget class
class UIWidget {
    protected:
        std::string label; /// In-System Name
        static constexpr char const *name = "generic widget";
        MediaState &m;
        MediaGraphics &g;
        int options;  /// Several Drawing options

    public:
        /// "Ideal" dimensions of the given widget.
        /// First set by the widget itself and then handled by the geometry
        /// manager on invoking calculation.
        MediaRect dims;

        /// Should the widget be drawn to the screen
        bool show = true;
        
        UIWidget(MediaState &m, MediaGraphics &g, std::string label, int options):
            m(m), g(g), label(label), options(options) {};

        virtual ~UIWidget() {};

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
        inline void set_container_dim(MediaRect dims);
        virtual inline void calculate_all() = 0;
};

inline void UIGeometry::set_container_dim(MediaRect dims)
{
    container_dim = dims;
    calculate_all();
}

/*
 * -----------------------------------------------------------------------------
 * GridGeometry. Default.
 * -----------------------------------------------------------------------------
 */

class UIGridGeometry : public UIGeometry {
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
        UIGridGeometry(std::vector<std::unique_ptr<UIWidget>> &widgets): UIGeometry(widgets) {}
        std::vector<GridEntry> grid_list;
        int grid_index;
        bool initial_refresh = true;
        inline void add(int rows, int cols, int repeat_till = 1);
        inline void set_row_height(int widget_index, int row_start, int h);
        inline void set_container_dim(MediaRect dims);
        inline void calculate_all();
        inline void translate_all(MediaPoint pos);
};

inline void UIGridGeometry::add(int rows, int cols, int repeat_till)
{
    printf("new grid: %ld %d %d %d\n", widgets.size(), rows, cols, repeat_till);
    grid_list.emplace_back((GridEntry) { widgets.size(), rows, cols, repeat_till });
}

inline void UIGridGeometry::set_row_height(int widget_index, int row_start, int h)
{
    for (int i = row_start; i <= widget_index; i++) {
        widgets[i]->dims.h = h;
    }
}

inline UIGridGeometry::GridEntry const *UIGridGeometry::iter(int widget_index)
{
    if (grid_list.size() == 0          ||
        grid_index >= grid_list.size() ||
        widget_index < grid_list[grid_index].widget_offset) {
        printf("Default Grid\n");
        return &default_grid;
    }

    printf("Custom Grid %d %d\n", widget_index, grid_index);
    return &grid_list[grid_index++];
}

inline void UIGridGeometry::calculate_all()
{
    GridEntry const *curr_grid;
    int min_grid_height;
    int row_start;
    grid_index = 0;
    MediaRect current_dim = container_dim;

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
                current_dim.x += current_dim.w;

                i++;
                if (i >= widgets.size())
                    goto end;
            }
            current_dim.y += min_grid_height + UI_DEFAULT_MARGIN;
        }
    }

end:
    return;
}

inline void UIGridGeometry::translate_all(MediaPoint dims)
{
    int diffx = dims.x - container_dim.x;
    int diffy = dims.y - container_dim.y;

    for (int i = 0; i < widgets.size(); i++) {
        widgets[i]->dims.x += diffx;
        widgets[i]->dims.y += diffy;
    }
}

inline void UIGridGeometry::set_container_dim(MediaRect dims)
{
    if (initial_refresh == true && container_dim.w == dims.w && container_dim.h == dims.h) {
        printf(">>>>>>>> translate\n");
        translate_all({dims.x, dims.y});
        container_dim = dims;
    } else {
        printf(">>>>>>>> resize\n");
        container_dim = dims;
        calculate_all();
        initial_refresh = false;
    }
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
    printf("RELGEO\n");
    PRINTRECT(container_dim);
    for (int i = 0; i < widgets.size(); i++) {
        current_grav = iter(i);
        widgets[i]->dims = Util::rect_align(
            container_dim, widgets[i]->dims, current_grav->gravity,
            current_grav->hpad, current_grav->vpad);
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
            dims.h += 2 * UI_DEFAULT_PADDING; /// @todo remove this
            PRINT_LINE
            PRINTRECT(dims);
        }

        ~UILabel() {
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
            PRINT_LINE
            PRINTRECT(dims);
        }

        ~UIButton() {
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
};


/*
 * =============================================================================
 * UIContainer
 * =============================================================================
 */

template <typename Geometry>
class UIContainer : public UIWidget {
    protected:
        static constexpr char const *name = "container widget";
        // UIDefaultPrimitives &p;
        UIWidgetList widgets;
        
    public:
        Geometry geo;
        UIContainer(
            MediaState &m,
            MediaGraphics &g,
            std::string label,
            int options = 0,
            MediaRect dims = {0, 0, 0, 0}
        ): UIWidget(m, g, label, options), geo(widgets)
        {
            this->dims = dims;
            //printf("UAUAUAUAUAU\n");
            //PRINTRECT(this->dims);
            geo.set_container_dim(this->dims);
            refresh();
            //printf("EAEAEAEAEAE\n");
            //PRINTRECT(this->dims);
        }

        virtual void draw();
        virtual bool event();
        virtual bool update();
        virtual void refresh();
        virtual void resize(MediaRect dims);

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
     * unique_ptr packages the allocated data pointer into a class which will
     * auto-delete the pointer when it goes out of scope.
     *
     * We then push it to the widget list vector via std::move, which gives up
     * ownership of the pointer to the vector. On destruction of the vector the
     * widget is also destructed.
     */

    template <typename Widget, typename ...Args>
    Widget &add(std::string label, int options = 0, Args &&...args)
    {
        Widget *k = new Widget(m, g, label, options, args...);
        std::unique_ptr<UIWidget> p(k);
        //printf("INIT DUNCCCCCCC\n");
        //PRINTRECT(p->dims);
        widgets.push_back(std::move(p));
        return *k;
    }
};

template <typename Geometry>
void UIContainer<Geometry>::resize(MediaRect dims)
{
    this->dims = dims;
    geo.set_container_dim(dims);
    refresh();
}

template <typename Geometry>
void UIContainer<Geometry>::draw()
{
    for (auto &i: widgets)
        i->draw();
}

template <typename Geometry>
bool UIContainer<Geometry>::event()
{
    bool no_refresh;
    for (auto &i: widgets)
        no_refresh = i->event();
    if (!no_refresh) {
        refresh();
        return false;
    }
    return true;
}

template <typename Geometry>
bool UIContainer<Geometry>::update()
{
    bool no_refresh;
    for (auto &i: widgets)
        no_refresh = i->update();
    if (!no_refresh) {
        refresh();
        return false;
    }
    return true;
}

template <typename Geometry>
void UIContainer<Geometry>::refresh()
{
    geo.set_container_dim(dims);
    for (auto &i: widgets)
        i->refresh();
}

/*
 * =============================================================================
 * UITopLevel
 * =============================================================================
 */

class UITopLevel : public UIContainer<UIRelativeGeometry> {
    protected:
        static constexpr char const *name = "toplevel widget";
        
    public:
        UITopLevel(
            MediaState &m,
            MediaGraphics &g,
            std::string label,
            int options = 0,
            MediaRect dims = {0, 0, 0, 0}
        ): UIContainer(m, g, label, options, dims) {
            //printf("INIT))()()()(\n");
            //PRINTRECT(dims);
        }
        
        bool event();
};


/*
 * ============================================================================
 * UIFrame
 * =============================================================================
 */

class UIFrame : public UIContainer<UIGridGeometry> {
    protected:
        static constexpr char const *name = "frame container widget";
        
    public:
        UIFrame(
            MediaState &m,
            MediaGraphics &g,
            std::string label,
            int options = 0,
            MediaRect dims = {0, 0, 0, 0}
        ): UIContainer(m, g, label, options, dims) {}

        void draw();
};
#endif
