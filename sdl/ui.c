// TODO Implement vectors

#include "ui.h"

/// Size of widgetlist
#define DEFAULT_WIDGETLIST_SIZE 16

/// Size of gridlist
#define DEFAULT_GRIDLIST_SIZE 8

/// Pixel gap between the bounding box and content of widget. Eqiv. to CSS' padding rule.
#define DEFAULT_PADDING 4

/// Pixel gap outside the bounding box of widget. Eqiv. to CSS' margin rule.
#define DEFAULT_MARGIN 4

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


/*
 * =============================================================================
 * Basic Functions
 * =============================================================================
 */

void ui_init(UIState *s, MediaState *w, MediaRect dims)
{
    s->w = w;
    s->dims = dims;
    s->g.container_dim = dims;

    /// TODO bound checking required for static init?
    /// TODO implement vectors instead
    s->widget_list = malloc(DEFAULT_WIDGETLIST_SIZE * sizeof(*s->widget_list));
    s->widget_list_size = DEFAULT_WIDGETLIST_SIZE;

    s->g.grid_list = malloc(DEFAULT_GRIDLIST_SIZE * sizeof(*s->g.grid_list));
    s->g.grid_list_size = DEFAULT_GRIDLIST_SIZE;
    s->g.grid_list_offset = 0;

    s->num_widgets = 0;
}

UIWidget ui_widget_init(UIState *s, UIWidgetType type, const char *label, int options)
{
    UIWidget k;
    k.type = type;
    k.options = options;
    k.state = UI_WIDGET_NORMAL;
    k.priv_data = malloc(ui_widget_list[type].priv_data_size); /// TODO NULL CHECK
    strncpy(k.label, label, UI_LABEL_MAX_SIZE);
    ui_widget_list[type].init(s, &k, label);
    return k;
}

UIWidget *ui_add_widget(UIState *s, UIWidgetType type, const char *label, int options)
{
    /// TODO accomodate container widgets
    UIWidget k = ui_widget_init(s, type, label, options);
    s->widget_list[s->num_widgets] = k;
    return &s->widget_list[s->num_widgets++];
}

void ui_update_widgets(UIState *s)
{
    for (int i = 0; i < s->num_widgets; i++) {
        ui_widget_list[s->widget_list[i].type].update(s, &s->widget_list[i]);
    }
}

void ui_draw_widgets(UIState *s)
{
    m_set_color(s->w, 32, 32, 32, 255);
    m_frect(s->w, &s->g.container_dim);
    m_set_color(s->w, 255, 255, 255, 255);
    m_rect(s->w, &s->g.container_dim);
    for (int i = 0; i < s->num_widgets; i++) {
        ui_widget_list[s->widget_list[i].type].draw(s, &s->widget_list[i]); 
    }
}

void ui_refesh_widgets(UIState *s)
{
    for (int i = 0; i < s->num_widgets; i++) {
        ui_widget_list[s->widget_list[i].type].refresh(s, &s->widget_list[i]);
        printf("* (%d, %d, %d, %d)\n",
                s->widget_list[i].dims.x,
                s->widget_list[i].dims.y,
                s->widget_list[i].dims.w,
                s->widget_list[i].dims.h);
    }
}


/*
 * Called before anything else. If new elements have been added, the widget
 * rectangles are updated. Use an if statement to detect changes or something
 * similar.
 */
 
void ui_refresh_layout(UIState *s)
{
    UIGeometryState *g = &s->g;
    UIGridEntry *curr_grid;

    g->current_dim = g->container_dim;
    g->current_rows = 1;
    g->current_cols = 1;
    g->cursor_x = 0;
    g->cursor_y = 0;

    int min_grid_height = 0;
    int grid_list_index = 0;

    if (g->grid_list_offset == 0)
        curr_grid = NULL;
    else {
        curr_grid = &g->grid_list[grid_list_index];
        if (curr_grid->widget_offset == 0) {
            g->current_rows = curr_grid->rows;
            g->current_cols = curr_grid->cols;
            grid_list_index++;
        }
    }

    // TODO Fix the Grid Layout

    for (int i = 0; i < s->num_widgets;) {
        printf("p\n");
        g->current_dim.w = g->container_dim.w / g->current_rows;
        for (int j = 0; j < g->current_cols; j++) {
            printf("q\n");
            for (int k = 0; k < g->current_rows; k++) {

                min_grid_height = min_grid_height > s->widget_list[i].dims.h ? min_grid_height : s->widget_list[i].dims.h;

                printf("k = %d j = %d\n", k, j);

                s->widget_list[i].dims.x = DEFAULT_PADDING + g->current_dim.x;
                s->widget_list[i].dims.y = g->current_dim.y + DEFAULT_PADDING;
                s->widget_list[i].dims.w = g->current_dim.w - 2 * DEFAULT_PADDING;
                s->widget_list[i].dims.h = min_grid_height + 1;

                printf("(%d, %d, %d, %d)\n",
                s->widget_list[i].dims.x,
                s->widget_list[i].dims.y,
                s->widget_list[i].dims.w,
                s->widget_list[i].dims.h);
                
                i++;

                if (i >= s->num_widgets)
                    goto end;
                g->current_dim.x += g->current_dim.w;
            }
            g->current_dim.y += min_grid_height + DEFAULT_PADDING;
            g->current_dim.x = g->container_dim.x;
        }

        if (grid_list_index >= g->grid_list_offset) {
            curr_grid = NULL;
            g->current_rows = 1;
            g->current_cols = 1;
        } else if (i == curr_grid->widget_offset) {
            printf("Switching: %d :: %d %d\n", i, curr_grid->rows, curr_grid->cols);
            curr_grid = &g->grid_list[grid_list_index++];
            g->current_rows = curr_grid->rows;
            g->current_cols = curr_grid->cols;
        } 
    }

end:
    ui_refesh_widgets(s);
    return;
}

int ui_grid(UIState *s, uint8_t rows, uint8_t cols)
{
    UIGeometryState *g = &s->g;

    g->grid_list[g->grid_list_offset++] = (UIGridEntry) {
        .widget_offset = s->num_widgets,
        .rows          = rows,
        .cols          = cols,
        .repeat_till   = 0
    };

    return 0;
}

int ui_repeatgrid(UIState *s, uint8_t rows, uint8_t cols, int8_t repeat_till)
{
    UIGeometryState *g = &s->g;

    g->grid_list[g->grid_list_offset++] = (UIGridEntry) {
        .widget_offset = s->num_widgets,
        .rows          = rows,
        .cols          = cols,
        .repeat_till   = repeat_till // TODO -1 for Forever
    };

    return 0;
}

int ui_gridspan(UIState *s, uint8_t rows, uint8_t cols)
{
    
}


/*
 * =============================================================================
 * Widget Definitions
 * =============================================================================
 */

/*
 * -----
 * Label
 * -----
 */

typedef struct UILabel {
    MediaObject label;
} UILabel;

void label_init(UIState *s, UIWidget *u, const char *label)
{
    UILabel *k = u->priv_data;
    k->label = m_text(s->w, label);
    u->dims = k->label.clip_rect;
    u->dims.w += DEFAULT_PADDING;
    u->dims.h += DEFAULT_PADDING;
}

void label_refresh(UIState *s, UIWidget *u)
{
    UILabel *k = u->priv_data;
    k->label.clip_rect = CENTER_RECT(k->label.clip_rect, u->dims);
}

void label_draw(UIState *s, UIWidget *u)
{
    UILabel *k = u->priv_data;
    m_paint(s->w, &k->label, &k->label.clip_rect);
}

void label_update(UIState *s, UIWidget *u)
{
    
}

void label_free(UIWidget *u)
{
    UILabel *k = u->priv_data;
    m_objfree(&k->label);
}

const UIWidgetClass uiw_label = {
    .name = "label",
    .priv_data_size = 0,
    .init           = label_init,
    .draw           = label_draw,
    .update         = label_update,
    .refresh        = label_refresh,
    .free           = label_free
};

/*
 * ------
 * Button
 * ------
 */

typedef struct UIButton {
    MediaObject button_label;
} UIButton;

void button_init(UIState *s, UIWidget *u, const char *label)
{
    UIButton *k = u->priv_data;
    k->button_label = m_text(s->w, label);
    u->dims = k->button_label.clip_rect;
    u->state = UI_WIDGET_NORMAL;
    u->dims.w += DEFAULT_PADDING;
    u->dims.h += DEFAULT_PADDING;
}

void button_refresh(UIState *s, UIWidget *u)
{
    UIButton *k = u->priv_data;
    k->button_label.clip_rect = CENTER_RECT(k->button_label.clip_rect, u->dims);
}

void button_draw(UIState *s, UIWidget *u)
{
    UIButton *k = u->priv_data;

    switch (u->state) {
    case UI_WIDGET_NORMAL:
        m_paint(s->w, &k->button_label, &k->button_label.clip_rect);
        m_set_color(s->w, 127, 127, 127, 255);
        m_rect(s->w, &u->dims);
        m_set_color(s->w, 255, 255, 255, 255);
        break;

    case UI_WIDGET_ACTIVE:
        m_paint(s->w, &k->button_label, &k->button_label.clip_rect);
        m_rect(s->w, &u->dims);
        break;

    case UI_WIDGET_DOWN:
        m_paint(s->w, &k->button_label, &k->button_label.clip_rect);
        m_rect(s->w, &u->dims);
        m_frect(s->w, &(MediaRect) {u->dims.x, u->dims.y, u->dims.w/10, u->dims.h});
        break;
    } 
}

void button_update(UIState *s, UIWidget *u)
{
    SDL_Event *e = &s->w->e;

    switch (e->type) {
    case SDL_MOUSEMOTION:
        if (POINT_IN_RECT(e->motion.x, e->motion.y, u->dims) && u->state != UI_WIDGET_DOWN) {
            u->state = UI_WIDGET_ACTIVE;
        } else if (u->state != UI_WIDGET_DOWN) {
            u->state = UI_WIDGET_NORMAL;
        }
        break;

    case SDL_MOUSEBUTTONDOWN:
        if (POINT_IN_RECT(e->button.x, e->button.y, u->dims)) {
            u->state = UI_WIDGET_DOWN;
        }
        break;

    case SDL_MOUSEBUTTONUP:
        if (POINT_IN_RECT(e->button.x, e->button.y, u->dims) && u->state == UI_WIDGET_DOWN) {
            u->state = UI_WIDGET_ACTIVE;
            // CALLBACK HERE
            printf("Clicked\n");
        } else if (POINT_IN_RECT(e->button.x, e->button.y, u->dims)) {
            u->state = UI_WIDGET_ACTIVE;
        } else {
            u->state = UI_WIDGET_NORMAL;
        }
        break;
    }
}

const UIWidgetClass uiw_button = {
    .name = "button",
    .priv_data_size = 0,
    .init           = button_init,
    .draw           = button_draw,
    .refresh        = button_refresh,
    .update         = button_update
};

/*
 * --------
 * Text Box
 * --------
 */

void textbox_init(UIState *s, UIWidget *u, const char *label)
{
    
}

void textbox_draw(UIState *s, UIWidget *u)
{
    
}

void textbox_update(UIState *s, UIWidget *u)
{
    
}

const UIWidgetClass uiw_textbox = {
    .name = "textbox",
    .priv_data_size = 0,
    .init           = textbox_init,
    .draw           = textbox_draw,
    .update         = textbox_update
};

/******************************************************************************/

const UIWidgetClass ui_widget_list[] = {
    [UI_BUTTON]  = uiw_button,
    [UI_LABEL]   = uiw_label,
   // [UI_TEXTBOX] = uiw_textbox
};
