#ifndef UI_H
#define UI_H

#include "media.hpp"

#include <string>
#include <vector>

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
typedef enum UIGravity {
    CENTER,
    TOPLEFT,
    TOP,
    TOPRIGHT,
    RIGHT,
    BOTTOMRIGHT,
    BOTTOM,
    BOTTOMLEFT,
    LEFT
} UIGravity;


/// Primitives used for drawing the GUI components.
class UIPrimitives {
    private:
        MediaGraphics &g;
        int padding;
        int margin;
        int min_height;
        int min_width;

    public:
        virtual UIPrimitives(MediaGraphics &g): g(g) {};
        virtual inline int box(UIWidget &u, MediaRect k) = 0;
        virtual inline int fbox(UIWidget &u, MediaRect k) = 0;
        virtual inline int line(UIWidget &u, int x1, int x2, int y1, int y2) = 0;
};

/*
 * =============================================================================
 * UIDefaultPrimitives
 * =============================================================================
 */

class UIDefaultPrimitives : public UIPrimitives {
    public:
        int padding    = UI_DEFAULT_PADDING;
        int margin     = UI_DEFAULT_MARGIN;
        int min_height = UI_DEFAULT_MIN_HEIGHT;
        int min_width  = UI_DEFAULT_MIN_WIDTH;
};

inline int UIDefaultPrimitives::box(UIWidget &u, MediaRect k)
{
    g.rect(k);
}

inline int UIDefaultPrimitives::fbox(UIWidget &u, MediaRect k)
{
    g.frect(k)
}

inline int UIDefaultPrimitives::line(UIWidget &u, int x1, int y1, int x2, int y2)
{
    g.line(x1, y1, x2, y2);
}

struct UIGridEntry {
    int widget_offset;
    int rows;
    int cols;
    int repeat_till;
};

class UIGeometry {
    public:
        std::vector<UIGridEntry> grid_list;
        MediaRect containter_dim;
        MediaRect current_dim;
        int widget_index;

        UIGeometry();
        ~UIGeometry();
        void grid();
        void repeat_grid();
        void reset();
        void get_rect(int widget_index);
};

class UIWidget {
    protected:
        static const type = "generic";
        const std::string label;
        MediaRect dims;
        MediaState &m;
        MediaGraphics &g;
        int options;

    public:
        int rowspan;
        int colspan;
        bool skip;
        
        virtual UIWidget(MediaState &m, MediaGraphics &g, std::string label);
        virtual int draw() = 0;
        virtual int update() = 0;
        virtual int refresh() = 0;
        virtual inline bool is_down() = 0;
        virtual inline bool is changed() = 0;
};

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

class UIState {
    private:    
        MediaState &m;
        MediaGraphics &g;
        UIDefaultPrimitives p;
        UIGeometry gm;
        MediaRect dims;
        int options;
        UIContainer &current_container;
        std::vector<UIWidget> widgets;

    public:
        UIState (MediaState &m, MediaGraphics &g):m(m), g(g) {};
        UIContainer &add_container();
        template <typename Widget, typename ...Args>
        UIWidget &add<Widget>(std::string label, int options, Args &&...args);
        UIWidget &add(UIWidget &&a);
        int snap(UIGravity grav, int hpad, int vpad);
        int draw();
        int update();
        int refresh();
};

#endif
