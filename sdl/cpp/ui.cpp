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
