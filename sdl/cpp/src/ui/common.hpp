#ifndef MEDIA_UI_COMMON_H
#define MEDIA_UI_COMMON_H

#include <memory>
#include <vector>
#include <string>

#define UI_DEFAULT_PADDING 3
#define UI_DEFAULT_MARGIN 5
#define UI_DEFAULT_MIN_WIDTH 30
#define UI_DEFAULT_MIN_HEIGHT 30

#define UI_OPTION_DEF_MAX 24
#define UI_OPTION_DEF_PARENT_MAX 32
#define UI_OPTION_DEF(x) (1 << (x))
#define UI_OPTION_DEF_PARENT(x) (UI_OPTION_DEF(x) << UI_OPTION_DEF_MAX)


namespace media {

namespace ui {

enum Options {
    OPT_WIDGET_NOCLIP    = UI_OPTION_DEF_PARENT(0),
    OPT_WIDGET_STRETCH   = UI_OPTION_DEF_PARENT(1),
    OPT_WIDGET_FIXED     = UI_OPTION_DEF_PARENT(2),
    /// Have properties been set?
    OPT_WIDGET_PROPS_SET = UI_OPTION_DEF(0),
};

enum WidgetState {
    UI_WIDGET_NORMAL,
    UI_WIDGET_ACTIVE,
    UI_WIDGET_DOWN,
    UI_WIDGET_CHANGED,
    UI_WIDGET_DISABLED
};

enum EventID {
    UI_BUTTON_CLICKED
};

/**
 * Generic Event handlers
 */

namespace behaviour {
/*
static inline bool clicked(State &k, Rect b, WidgetState &b)
{
    return false;
}
*/
};



};

};

#endif
