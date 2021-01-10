#ifndef SDL_UI_H
#define SDL_UI_H

#include "media.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdint.h>

#define NAME_MAX_SIZE 32

/*
 * And this, kids, is why most windowing toolkits are in object oriented
 * languages.
 */

/// Options of a widget
typedef enum UIOptions {
    UI_OPTION_NOCLIP = 0x1,
    UI_OPTION_NOSTRETCH = 0x2,
} UIOptions;

/// Widget Gravity
typedef enum Gravity {
    TOPLEFT,
    TOP,
    TOPRIGHT,
    RIGHT,
    BOTTOMRIGHT,
    BOTTOM,
    BOTTOMLEFT,
    LEFT
} Gravity;


/// State of a widget
typedef enum UIWidgetState {
    UI_WIDGET_NORMAL,
    UI_WIDGET_ACTIVE,
    UI_WIDGET_DOWN,
    UI_WIDGET_CHANGED,
    UI_WIDGET_DISABLED,
    UI_WIDGET_INVISIBLE
} UIWidgetState;

typedef enum UIWidgetType {
    UI_BUTTON,
    UI_LABEL,
    UI_TEXTBOX
} UIWidgetType;

typedef struct UIWidgetClass UIWidgetClass;
typedef struct UIWidget UIWidget;

typedef struct UIGridEntry {
    uint8_t widget_offset;
    uint8_t rows;
    uint8_t cols;
    int8_t repeat_till;
} UIGridEntry;

typedef struct UIGeometryState {
    MediaRect container_dim; /// Current Dimensions of the container
    MediaRect current_dim;   /// Current dimensions of the widget placement "Cursor"
    UIGridEntry *grid_list;  /// The List of grids that are processed
    int grid_list_size;      /// Grid List Size 
    int grid_list_top;       /// Grid List Top
    int grid_list_index;
    int cursor_x;
    int cursor_y;
    int current_rows;
    int current_cols;
    int grid_col_offset;
    int widget_index;
} UIGeometryState;

typedef struct UIContainerWidget {
    UIWidget *widget_list;
    int num_widgets;
    int keyboard_cursor; /// Current active keyboard cursor location
    int mouse_cursor;    /// Current active mouse cursor location
} UIContainerWidget;

typedef struct UIState {
    UIGeometryState g;
    MediaState *w;
    // The following 3 elements are supposed to be a widget container.
    UIWidget *widget_list; /// Remove Later
    int widget_list_size;
    int num_widgets;       /// Remove Later
    MediaRect dims;        /// Remove Later
    int keyboard_cursor;   /// Current active keyboard cursor location
    int mouse_cursor;      /// Current active mouse cursor location
} UIState;

struct UIWidgetClass {
    char name[NAME_MAX_SIZE];
    int priv_data_size;
    // May eventually need to include a UIContainerWidget arg here
    void (*init)(UIState *s, UIWidget *u, const char *label); /// Initialises the widget
    void (*draw)(UIState *s, UIWidget *u); /// Draws the widget
    void (*update)(UIState *s, UIWidget *u); /// Updates the widget
    void (*refresh)(UIState *s, UIWidget *u); /// Called when container is resized, repositioned.
                                              /// This allows the widget positions to be recalculated
    void (*free)(UIWidget *u);
};

#define UI_LABEL_MAX_SIZE 256

struct UIWidget {
    char label[UI_LABEL_MAX_SIZE]; /// Label of the given widget
    MediaRect dims;      /// Dimensions of the widget
    UIWidgetType type;   /// Type of the widget. Used to render it.
    int options;         /// OR'ed values drom UIOptions
    UIWidgetState state; /// Current state of widget.
    void *priv_data;     /// Used in the case of widgets like sliders and textboxes.
    int rowspan;         /// Grid Row Span
    int colspan;         /// Grid Col Span
};


/// Initialises the GUI library
void ui_init(UIState *s, MediaState *w, MediaRect dims);

/// Add a widget
UIWidget *ui_add_widget(UIState *s, UIWidgetType type, const char *label, int options);

/// Refresh layout geometry
void ui_refresh_layout(UIState *s);

/// Supposed to be in the draw part of the game loop
void ui_draw_widgets(UIState *s);

/// Supposed to be in the update part of the game loop
void ui_update_widgets(UIState *s);

/// Tells the layout manager to create a grid layout of n rows and m columns
/// for the next nxm widgets.
int ui_grid(UIState *s, uint8_t rows, uint8_t cols);

/// Tells the layout manager to create a grid layout of n rows and m columns.
/// This layout will be repeated for each nxm widgets inserted.
int ui_repeatgrid(UIState *s, uint8_t rows, uint8_t cols, int8_t repeat_till);

/// Tells the layout manager that the next widget that will be inserted shall
/// cover m rows and n columns
int ui_gridspan(UIState *s, uint8_t rows, uint8_t cols);


static inline MediaRect ui_sticky_rect(int sw, int sh, int w, int h,
                                       Gravity g, int hpad, int vpad)
{
    switch (g) {
    case TOPLEFT:
        return (MediaRect) { hpad, vpad, w, h };

    case TOP:
        return (MediaRect) { sw / 2 - w / 2, vpad, w, h };

    case TOPRIGHT:
        return (MediaRect) { sw - w - hpad, vpad, w, h};

    case RIGHT:
        return (MediaRect) { sw - w - hpad, sh / 2 - h / 2, w, h };

    case BOTTOMRIGHT:
        return (MediaRect) { sw - w - hpad, sw - w - vpad, w, h };

    case BOTTOM:
        return (MediaRect) { sw / 2 - w / 2, sh - h - hpad, w, h };

    case BOTTOMLEFT:
        return (MediaRect) { hpad, sh - h - hpad, w, h };

    case LEFT:
        return (MediaRect) { hpad, sh / 2 - h / 2, w, h };

    default:
        return (MediaRect) {0, 0, 0, 0};
    }
}

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

extern const UIWidgetClass ui_widget_list[];
#endif
