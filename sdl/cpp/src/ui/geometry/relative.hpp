#ifndef MEDIA_UI_GEOMETRY_RELATIVE_H
#define MEDIA_UI_GEOMETRY_RELATIVE_H

#include "media/media.hpp"
#include "ui/widget/widget.hpp"

namespace media {

namespace ui {

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
        widgets[i]->dims = util::rect_align(
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

};

};

#endif
