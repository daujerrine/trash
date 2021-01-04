#include "ui.h"

/// Size of widgetlist
#define DEFAULT_WIDGETLIST_SIZE 16

/// Pixel gap between the bounding box and content of widget. Eqiv. to CSS' padding rule.
#define DEFAULT_PADDING 4

/// Pixel gap outside the bounding box of widget. Eqiv. to CSS' margin rule.
#define DEFAULT_MARGIN 4

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
    m_rect(s->w, &s->g.container_dim);
    for (int i = 0; i < s->num_widgets; i++) {
        ui_widget_list[s->widget_list[i].type].draw(s, &s->widget_list[i]); 
    }
}


/*
 * Called before anything else. If new elements have been added, the widget
 * rectangles are updated. USe an if statemnt to detect changes or something
 * similar.
MediaRect container_dim;
MediaRect current_dim;
int cursor_x;
int cursor_y;
int current_rows;
int current_cols;
 */
 
void ui_refresh_layout(UIState *s)
{
    UIGeometryState *g = &s->g;

    g->current_dim = g->container_dim;
    g->current_rows = 1;
    g->current_cols = 1;
    g->cursor_x = 0;
    g->cursor_y = 0;

    for (int i = 0; i < s->num_widgets; i++) {
        s->widget_list[i].dims.x = g->current_dim.x + DEFAULT_PADDING;
        s->widget_list[i].dims.y = g->current_dim.y + DEFAULT_PADDING;
        s->widget_list[i].dims.w = g->current_dim.w - 2 * DEFAULT_PADDING;
        s->widget_list[i].dims.h = 20 - 2 * DEFAULT_PADDING;

        g->current_dim.x += 0;
        g->current_dim.y += 20;
    }
}

/*
 * =============================================================================
 * Widget Defintions
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
}

void label_refresh(UIState *s, UIWidget *u)
{

}

void label_draw(UIState *s, UIWidget *u)
{
    UILabel *k = u->priv_data;
    k->label.clip_rect.x = u->dims.x + u->dims.w / 2 - k->label.clip_rect.w / 2;
    k->label.clip_rect.y = u->dims.y;
    m_paint(s->w, &k->label, &k->label.clip_rect);
}

void label_update(UIState *s, UIWidget *u)
{
    // Nothing to do
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
    .free           = label_free
};

/*
 * ------
 * Button
 * ------
 */

typedef struct UIButton {
    MediaObject button_bg;
    MediaObject button_label;
} UIButton;

void button_init(UIState *s, UIWidget *u, const char *label)
{
    // UIButton *k = u->priv_data;
    
}

void button_draw(UIState *s, UIWidget *u)
{
    
}

void button_update(UIState *s, UIWidget *u)
{
    
}

const UIWidgetClass uiw_button = {
    .name = "button",
    .priv_data_size = 0,
    .init           = button_init,
    .draw           = button_draw,
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
   // [UI_BUTTON]  = uiw_button,
    [UI_LABEL]   = uiw_label,
   // [UI_TEXTBOX] = uiw_textbox
};
