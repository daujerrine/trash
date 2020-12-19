#ifndef SDL_UI_H
#define SDL_UI_H

#include "media.h"

/*
 * And this, kids, is why most windowing toolkits are in object oriented
 * languages.
 */

/// Options of a widget
typedef enum UIOptions {
    UI_OPTION_NOCLIP = 0x1,
    UI_OPTION_NOSTRETCH = 0x2,
} UIOptions;

/// State of a widget
typedef enum UIWidgetState {
    UI_WIDGET_NORMAL,
    UI_WIDGET_ACTIVE,
    UI_WIDGET_DOWN,
    UI_WIDGET_CHANGED,
    UI_WIDGET_DISABLED,
    UI_WIDGET_INVISIBLE
} UIWidgetState;

typedef struct UIWidgetClass UIWidgetClass;
typedef struct UIWidget UIWidget;

typedef struct UIContainerWidget {
    UIWidget *widget_list;
    int num_widgets;
    int keyboard_cursor; /// Current active keyboard cursor location
    int mouse_cursor;    /// Current active mouse cursor location
} UIContainerWidget;

typedef struct UIState {
    SDLWindowState *w;
    // The following 3 elements are supposed to be a widget container.
    UIWidget *widget_list; /// Remove Later
    int num_widgets;       /// Remove Later
    MediaRect dims;        /// Remove Later
    int keyboard_cursor; /// Current active keyboard cursor location
    int mouse_cursor;    /// Current active mouse cursor location
} UIState;

struct UIWidgetClass {
    int priv_data_size;
    // May eventually need to include a UIContainerWidget arg here
    void (*init)(UIState *s, UIWidget *u, const char *label); /// Initialises the widget
    void (*draw)(UIState *s, UIWidget *u); /// Draws the widget
    void (*update)(UIState *s, UIWidget *u); /// Updates the widget
};

#define UI_LABEL_MAX_SIZE 256

struct UIWidget {
    char label[UI_LABEL_MAX_SIZE]; /// Label of the given widget
    MediaRect dims;      /// Dimensions of the widget
    UIWidgetClass *type; /// Type of the widget. Used to render it.
    int options;         /// OR'ed values drom UIOptions
    UIWidgetState state; /// Current state of widget.
    void *priv_data;     /// Used in the case of widgets like sliders and textboxes.
};

void ui_init(UIState *s, SDLWindowState *w);
void ui_update_widgets(UIState *s);


static inline int ui_isdown(UIWidget *s)
{
    return s->state == UI_WIDGET_DOWN;
}

static inline int ui_ischanged(UIWidget *s)
{
    return s->state == UI_WIDGET_CHANGED;
}

static inline void ui_disable(UIWidget *s)
{
    s->state = UI_WIDGET_DISABLED;
}

typedef enum UIWidgetType {
    UI_BUTTON,
    UI_LABEL,
    UI_TEXTBOX
} UIWidgetType;

extern UIWidgetClass ui_widget_list[];
#endif
