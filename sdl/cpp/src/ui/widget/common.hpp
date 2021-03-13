#ifndef MEDIA_UI_WIDGET_COMMON_H
#define MEDIA_UI_WIDGET_COMMON_H

#include <memory>
#include <vector>
#include <string>

#include "media/media.hpp"
#include "ui/common.hpp"
#include "ui/primitives.hpp"

namespace media {

namespace ui {

struct WidgetProperties {
    std::string tooltip;
    // Primitives *style;
    Gravity content_align = CENTER;
    Size min_size         = { 0, 0 };
    Size max_size         = { 100000, 100000 };
    Color bg_color        = { 110, 110, 110  };
    Color fg_color        = { 255, 255, 255, };
    int padding           = UI_DEFAULT_PADDING;
    int margin            = UI_DEFAULT_MARGIN;
    int scale             = 1;
};

/// Default Abstract widget class
class Widget {
    protected:
        std::string label; /// Display Name
        static constexpr char const *name = "generic widget"; /// Class Name
        media::State &m;
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

};

};

#endif
