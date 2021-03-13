#ifndef MEDIA_UI_GEOMETRY_GRID_H
#define MEDIA_UI_GEOMETRY_GRID_H

#include "ui/widget/widget.hpp"

namespace media {

namespace ui {

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

};

};

#endif
