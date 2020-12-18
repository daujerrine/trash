#include "ui.h"

/*
 * =============================================================================
 * Button
 * =============================================================================
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

UIWidget uiw_button = {
    .priv_data_size = 0,
    .init           = button_init,
    .draw           = button_draw,
    .update         = button_update
};

/*
 * =============================================================================
 * Label
 * =============================================================================
 */

void label_draw(UIState *s, UIWidgetInstance *u)
{
    
}

void label_update(UIState *s, UIWidgetInstance *u)
{
    
}

UIWidget uiw_label = {
    .priv_data_size = 0,
    .init           = label_init,
    .draw           = label_draw,
    .update         = label_update
};

/*
 * =============================================================================
 * Text Box
 * =============================================================================
 */

void textbox_draw(UIState *s, UIWidgetInstance *u)
{
    
}

void textbox_update(UIState *s, UIWidgetInstance *u)
{
    
}

UIWidget uiw_textbox = {
    .priv_data_size = 0,
    .init           = textbox_init,
    .draw           = textbox_draw,
    .update         = textbox_update
};


UIWidget ui_widget_list[] = {
    [UI_BUTTON]  = uiw_button,
    [UI_LABEL]   = uiw_label,
    [UI_TEXTBOX] = uiw_textbox
};
