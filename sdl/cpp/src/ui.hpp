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

#define UI_OPTION_DEF_MAX 24
#define UI_OPTION_DEF_PARENT_MAX 32
#define UI_OPTION_DEF(x) (1 << (x))
#define UI_OPTION_DEF_PARENT(x) (UI_OPTION_DEF(x) << UI_OPTION_DEF_MAX)


enum UIOptions {
    UIOPT_WIDGET_NOCLIP    = UI_OPTION_DEF_PARENT(0),
    UIOPT_WIDGET_STRETCH   = UI_OPTION_DEF_PARENT(1),
    UIOPT_WIDGET_FIXED     = UI_OPTION_DEF_PARENT(2),
    /// Have properties been set?
    UIOPT_WIDGET_PROPS_SET = UI_OPTION_DEF(0),
};

enum UIWidgetState {
    UI_WIDGET_NORMAL,
    UI_WIDGET_ACTIVE,
    UI_WIDGET_DOWN,
    UI_WIDGET_CHANGED,
    UI_WIDGET_DISABLED
};

enum UIEventID {
    UI_BUTTON_CLICKED
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

class UIWidget;

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
        virtual inline int box(MediaRect k) = 0;
        virtual inline int fbox(MediaRect k) = 0;
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
        inline int box(MediaRect k);
        inline int fbox(MediaRect k);
        inline int line(UIWidget &u, int x1, int x2, int y1, int y2);
};

inline int UIDefaultPrimitives::box(MediaRect k)
{
    return g.rect(k);
}

inline int UIDefaultPrimitives::fbox(MediaRect k)
{
    g.set_color(128, 128, 128, 255);
    g.frect(k);
    g.set_color(255, 255, 255, 255);
    return g.rect(k);
}

inline int UIDefaultPrimitives::line(UIWidget &u, int x1, int y1, int x2, int y2)
{
    return g.line(x1, y1, x2, y2);
}

struct UIWidgetProperties {
    std::string tooltip;
    // UIPrimitives *style;
    UIGravity content_align = CENTER;
    MediaSize min_size      = { 0, 0 };
    MediaSize max_size      = { 100000, 100000 };
    MediaColor bg_color     = { 110, 110, 110  };
    MediaColor fg_color     = { 255, 255, 255, };
    int padding             = UI_DEFAULT_PADDING;
    int margin              = UI_DEFAULT_MARGIN;
    int scale               = 1;
};

/*
UIWidgetProperties ui_default_properties = (UIWidgetProperties) {
    .content_align = CENTER,
    .min_size      = { 0, 0 },
    .max_size      = { 100000, 100000 },
    .bg_color      = { 110, 110, 110  },
    .fg_color      = { 255, 255, 255, },
    .padding       = UI_DEFAULT_PADDING,
    .margin        = UI_DEFAULT_MARGIN,
    .scale         = 1
};
*/

/*
 * =============================================================================
 * UIState
 * =============================================================================
 */

/**
 * Context variables included in every widget
 * @todo Pass as argument instead?
 */
struct UIState {
    MediaState    &m;
    MediaGraphics &g;
    UIPrimitives  &p;
    UIState(MediaState &m, MediaGraphics &g, UIPrimitives &p): m(m), g(g), p(p) {}
};

/*
 * =============================================================================
 * UIWidget
 * =============================================================================
 */

/// Default Abstract widget class
class UIWidget {
    protected:
        std::string label; /// Display Name
        static constexpr char const *name = "generic widget"; /// Class Name
        MediaState &m;
        MediaGraphics &g;
        UIPrimitives &p;

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
        UIWidgetProperties properties;
        /**
         * "Ideal" dimensions of the given widget.
         * First set by the widget itself and then handled by the geometry
         * manager on invoking calculation.
         */
        MediaRect dims;
        
        UIWidget(MediaState &m, MediaGraphics &g, UIPrimitives &p, std::string label, int options):
            m(m), g(g), p(p), label(label), options(options) {}

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
typedef std::vector<std::unique_ptr<UIWidget>> UIWidgetList;

/*
 * =============================================================================
 * Widget Geometry Management
 * =============================================================================
 */

class UIGeometry {
    protected:
        UIWidgetList &widgets;
        UIWidgetProperties &properties;

    public:
        UIGeometry (
            UIWidgetList &widgets,
            UIWidgetProperties &properties
        ): widgets(widgets), properties(properties) {}
        MediaRect container_dim  = {0, 0, 0, 0};

        virtual MediaRect calculate_all(MediaRect dims) = 0;
        virtual MediaRect update_container_dim(MediaRect dims) = 0;
};

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
        UIGridGeometry(
            UIWidgetList &widgets,
            UIWidgetProperties &properties
        ): UIGeometry(widgets, properties) {}
        std::vector<GridEntry> grid_list;
        int grid_index;
        bool initial_refresh = true;
        MediaRect c = {0, 0, 0, 0};

        inline void add(int rows, int cols, int repeat_till = 1);
        inline void set_row_height(int widget_index, int row_start, int h);

        inline MediaRect calculate_all(MediaRect dims);
        inline MediaRect translate_all(MediaPoint pos);
        inline MediaRect update_container_dim(MediaRect dims);
};

inline void UIGridGeometry::add(int rows, int cols, int repeat_till)
{
    //printf("new grid: %ld %d %d %d\n", widgets.size(), rows, cols, repeat_till);
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
        //printf("Default Grid\n");
        return &default_grid;
    }

    //printf("Custom Grid %d %d\n", widget_index, grid_index);
    return &grid_list[grid_index++];
}

inline MediaRect UIGridGeometry::calculate_all(MediaRect new_dim)
{
    GridEntry const *curr_grid;
    int min_grid_height;
    int row_start;
    grid_index = 0;
    MediaRect current_dim = new_dim;
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

inline MediaRect UIGridGeometry::translate_all(MediaPoint dims)
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
inline MediaRect UIGridGeometry::update_container_dim(MediaRect new_dim)
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
        UIRelativeGeometry(
            std::vector<std::unique_ptr<UIWidget>> &widgets,
            UIWidgetProperties &properties
        ): UIGeometry(widgets, properties) {}
        std::vector<GravityEntry> grav_list;
        int grav_index;

        inline void add(UIGravity grav, int hpad, int vpad);
        inline MediaRect calculate_all(MediaRect new_dim);
        inline MediaRect update_container_dim(MediaRect new_dim);
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
        // printf("Default Grav\n");
        return &default_grav;
    }

    // printf("Custom Grav %d %d\n", widget_index, grav_index);
    return &grav_list[grav_index++];
}

inline MediaRect UIRelativeGeometry::calculate_all(MediaRect new_dim)
{
    GravityEntry const *current_grav;
    grav_index = 0;
    //printf("RELGEO\n");
    //PRINTRECT(container_dim);
    container_dim = new_dim;
    for (int i = 0; i < widgets.size(); i++) {
        current_grav = iter(i);
        widgets[i]->dims = MediaUtil::rect_align(
            container_dim, widgets[i]->dims, current_grav->gravity,
            current_grav->hpad, current_grav->vpad);
            //PRINTRECT(widgets[i]->dims);
    }

    return container_dim;
}

inline MediaRect UIRelativeGeometry::update_container_dim(MediaRect new_dim)
{
    return calculate_all(new_dim);
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
            // printf("LABELINIT::: "); PRINTRECT(o_label.dest_rect);
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
        static constexpr char const *name = "button";
        MediaClipObject o_label;
        bool clicked_flag = false;

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
            return clicked_flag;
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
        static constexpr char const *name = "container";
        // UIDefaultPrimitives &p;
        UIWidgetList widgets;
        UIDefaultPrimitives p;
        
    public:
        Geometry geo;

        UIContainer(
            MediaState &m,
            MediaGraphics &g,
            std::string label,
            int options = 0,
            MediaRect dims = {0, 0, 0, 0}
        ): UIWidget(m, g, label, options), geo(widgets, properties), p(g)
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
         * unique_ptr packages the allocated data pointer into a class which
         * will auto-delete the pointer when it goes out of scope.
         *
         * We then push it to the widget list vector via std::move, which gives
         * up ownership of the pointer to the vector. On destruction of the
         * vector the widget is also destructed.
         */

        template <typename Widget, typename ...Args>
        Widget &add(std::string label, int options = 0, Args &&...args)
        {
            Widget *k = new Widget(m, g, label, options, args...);
            std::unique_ptr<UIWidget> p(k);
            widgets.push_back(std::move(p));
            return *k;
        }
};

template <typename Geometry>
void UIContainer<Geometry>::resize(MediaRect dims)
{
    this->dims = geo.update_container_dim(dims);
    refresh();
}

template <typename Geometry>
void UIContainer<Geometry>::draw()
{
    for (auto &i: widgets) {
        if (i->shown())
            i->draw();
    }
}

template <typename Geometry>
bool UIContainer<Geometry>::event()
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
bool UIContainer<Geometry>::update()
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
void UIContainer<Geometry>::refresh()
{
    dims = geo.update_container_dim(dims);
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
        static constexpr char const *name = "toplevel";
        
    public:
        UITopLevel(
            MediaState &m,
            MediaGraphics &g,
            std::string label,
            int options = 0,
            MediaRect dims = {0, 0, 0, 0}
        ): UIContainer(m, g, label, options, dims) {}
        
        bool event();
};


/*
 * ============================================================================
 * UIFrame
 * =============================================================================
 */

class UIFrame : public UIContainer<UIGridGeometry> {
    protected:
        static constexpr char const *name = "frame";
        
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
