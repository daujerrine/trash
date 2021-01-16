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

class UIPrimitives {
    private:
        MediaGraphics &g;
        MediaColor &c;

    public:
        const int padding;
        const int margin;
        const int min_height;
        const int min_width;
        virtual inline void set_color(MediaColor c);
        virtual inline int box(UIWidget &u, MediaRect k) = 0;
        virtual inline int fbox(UIWidget &u, MediaRect k) = 0;
        virtual inline int line(UIWidget &u, MediaRect k) = 0;
        virtual inline int slider_handle(UIWidget &u, MediaRect k) = 0;
        virtual inline int slider_rail(UIWidget &u, MediaRect k) = 0;
        virtual inline int scrollbar_handle(UIWidget &u, MediaRect k) = 0;
        virtual inline int scrollbar_rail(UIWidget &u, MediaRect k) = 0;
};

class UIDefaultPrimitives : public UIPrimitives;

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
        UIState ()
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
