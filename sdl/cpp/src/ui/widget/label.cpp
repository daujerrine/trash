#include "label.hpp"

namespace media {

namespace ui {

void Label::draw()
{
    /*printf("***\n");
    PRINTRECT(o_label.src_rect);
    PRINTRECT(o_label.dest_rect);
    PRINTRECT(dims);
    PRINTRECT(smaller(o_label.dest_rect, dims));
    PRINTRECT(smaller_dest(o_label.dest_rect, dims));
    printf("***\n");*/
    g.paint(o_label);
}

bool Label::event()
{
    return true;
}

bool Label::update()
{
    return true;
}

void Label::refresh()
{
    o_label.overflow_x(dims);
    o_label.align(dims, properties.content_align);
}

void Label::set_label(std::string label)
{
    this->label = label;
    g.text(o_label, label);
    refresh();
}

}

}
