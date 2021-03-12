#ifndef MEDIA_UI_PRIMITIVES_H
#define MEDIA_UI_PRIMITIVES_H

namespace media {

namespace ui {

class Widget;


/// Primitives used for drawing the GUI components.
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

};

};

#endif
