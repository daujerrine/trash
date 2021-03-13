#ifndef MEDIA_UI_GEOMETRY_COMMON_H
#define MEDIA_UI_GEOMETRY_COMMON_H

#include "media/media.hpp"
#include "ui/widget/widget.hpp"
#include "ui/common.hpp"


namespace media {

namespace ui {

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


};

};

#endif
