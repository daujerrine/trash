#ifndef SDL_UI_H
#define SDL_UI_H

/*
 * And this, kids, is why most windowing toolkits are in object oriented
 * languages.
 */

// Options of a widget
typedef enum UIOptions {
    UI_OPTION_NOCLIP = 0x1,
    UI_OPTION_NOSTRETCH = 0x2,
} UIOptions;

// State of a widget
typedef enum UIWidgetState {
    UI_WIDGET_NORMAL,
    UI_WIDGET_ACTIVE,
    UI_WIDGET_DOWN,
    UI_WIDGET_CHANGED,
    UI_WIDGET_DISABLED
} UIWidgetState;

typedef struct UIWidget UIWidget;
typedef struct UIWidgetInstance UIWidgetInstance;

typedef struct UIContainerWidget {
    UIWidgetInstance *widget_list;
    int num_widgets;
    int keyboard_cursor; /// Current active keyboard cursor location
    int mouse_cursor;    /// Current active mouse cursor location
} UIContainerWidget;

typedef struct UIState {
    SDLWindowState *state;
    UIWidgetInstance *widget_list;
    int num_widgets;
    int keyboard_cursor; /// Current active keyboard cursor location
    int mouse_cursor;    /// Current active mouse cursor location
} UIState;

struct UIWidget {
    int priv_data_size;
    void (*init)(UIState *s, UIWidgetInstance *u); /// Initialises the widget
    void (*draw)(UIState *s, UIWidgetInstance *u); /// Draws the widget
    void (*update)(UIState *s, UIWidgetInstance *u); /// Updates the widget
};

struct UIWidgetInstance {
    char label[256];     /// Label of the given widget
    MediaRect dims;      /// Dimensions of the widget
    UIWidget *type;      /// Type of the widget. Used to render it.
    int options;         /// OR'ed values drom UIOptions
    UIWidgetState state; /// Current state of widget.
    void *priv_data;     /// Used in the case of widgets like sliders and textboxes.
};

void ui_init(UIState *s, SDLWindowState *w);
void ui_update_widgets(UIState *s);


static inline void ui_isdown(UIWidgetInstance *s)
{
    return s->state == UI_WIDGET_DOWN;
}

static inline void ui_ischanged(UIWidgetInstance *s)
{
    return s->state == UI_WIDGET_CHANGED;
}

typedef enum UIWidgetType {
    UI_BUTTON,
    UI_LABEL,
    UI_TEXTBOX
} UIWidgetType;

extern UIWidget ui_widget_list[];
#endif
