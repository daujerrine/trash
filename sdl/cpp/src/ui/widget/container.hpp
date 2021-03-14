#ifndef MEDIA_UI_WIDGET_CONTAINER_H
#define MEDIA_UI_WIDGET_CONTAINER_H

#include "media/media.hpp"
#include "ui/geometry/geometry.hpp"
#include "common.hpp"

namespace media {

namespace ui {

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

template <typename GeometryT>
void Container<GeometryT>::resize(Rect dims)
{
    this->dims = geo.update_container_dim(dims);
    refresh();
}

template <typename GeometryT>
void Container<GeometryT>::draw()
{
    for (auto &i: widgets) {
        if (i->shown())
            i->draw();
    }
}

template <typename GeometryT>
bool Container<GeometryT>::event()
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

template <typename GeometryT>
bool Container<GeometryT>::update()
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

template <typename GeometryT>
void Container<GeometryT>::refresh()
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
