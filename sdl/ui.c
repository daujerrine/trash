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

void ui_init(UIState *s, SDLWindowState *w)
{
    s->w = w;

    /// TODO bound checking required for static init?
    /// TODO implement vectors instead
    s->widget_list = malloc(DEFAULT_WIDGETLIST_SIZE * sizeof(*s->widget_list));
    s->num_widgets = 0;
}

UIWidget ui_widget_init(UIState *s, UIWidgetType type, const char *label, int options)
{
    UIWidget k;
    k->type = &ui_widget_list[type];
    k->options = options;
    k->state = UI_WIDGET_NORMAL;
    s->priv_data = malloc(ui_widget_list[type].priv_data_size); /// TODO NULL CHECK
    strncpy(k->label, label, UI_LABEL_MAX_SIZE);
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
        // TODO
    }
}

/*
 * -----
 * Label
 * -----
 */

typedef struct UILabel {
    MediaObject label;
} UILabel;

void label_init(UIState *s, UIWidget *u)
{
    M
}

void label_draw(UIState *s, UIWidget *u)
{
    UILabel *k = u->priv_data;
    m_paint(s->w->renderer, k->label, NULL, u->dims);
}

void label_update(UIState *s, UIWidget *u)
{
    // Nothing to do
}

UIWidgetClass uiw_label = {
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
    SDL_Texture button_bg;
    SDL_Texture button_label;
} UIButton;

void button_init(UIState *s, UIWidgetInstance *u)
{
    UIButton *k = u->priv_data;
    
}

void button_draw(UIState *s, UIWidgetInstance *u)
{
    
}

void button_update(UIState *s, UIWidgetInstance *u)
{
    
}

UIWidgetClass uiw_button = {
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

void textbox_draw(UIState *s, UIWidgetInstance *u)
{
    
}

void textbox_update(UIState *s, UIWidgetInstance *u)
{
    
}

UIWidgetClass uiw_textbox = {
    .priv_data_size = 0,
    .init           = textbox_init,
    .draw           = textbox_draw,
    .update         = textbox_update
};

/******************************************************************************/

UIWidgetClass ui_widget_list[] = {
    [UI_BUTTON]  = uiw_button,
    [UI_LABEL]   = uiw_label,
    [UI_TEXTBOX] = uiw_textbox
};
