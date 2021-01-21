#ifndef MEDIA_H
#define MEDIA_H

#include <vector>
#include <string>

#include <cstdint>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define PRINT_LINE printf("At: %s, %d, %s\n", __PRETTY_FUNCTION__, __LINE__, __FILE__);
#define LZCHECK(_q) \
    if ((_q) < 0) { \
        printf("LZERO: "); \
        PRINT_LINE \
    }
#define NULLCHECK(_q) \
    if ((_q) == nullptr) { \
        printf("NULL: "); \
        PRINT_LINE \
    }
typedef SDL_Rect MediaRect;
typedef SDL_Point MediaPoint;
typedef SDL_Color MediaColor;

/// Widget Gravity
typedef enum MediaGravity {
    CENTER,
    TOPLEFT,
    TOP,
    TOPRIGHT,
    RIGHT,
    BOTTOMRIGHT,
    BOTTOM,
    BOTTOMLEFT,
    LEFT
} MediaGravity;

struct MediaFPSCounter {
    uint64_t start;
    uint64_t end;
    uint64_t elapsed;
    float value;
};

/**
 * Aligns an outer rectangle with an inner one.
 */
 
static inline MediaRect _rect_align(MediaRect out, MediaRect in, MediaGravity g, int hpad, int vpad)
{
    switch (g) {
    case TOPLEFT:
        return (MediaRect) { out.x + hpad, out.y + vpad, in.w, in.h };

    case TOP:
        return (MediaRect) { out.x + out.w / 2 - in.w / 2, out.y + vpad, in.w, in.h };

    case TOPRIGHT:
        return (MediaRect) { out.x + out.w - in.w - hpad, out.y + vpad, in.w, in.h};

    case RIGHT:
        return (MediaRect) { out.x + out.w - in.w - hpad, out.y / 2 - in.h / 2, in.w, in.h };

    case BOTTOMRIGHT:
        return (MediaRect) { out.x + out.w - in.w - hpad, out.y + out.w - in.w - vpad, in.w, in.h };

    case BOTTOM:
        return (MediaRect) { out.x + out.w / 2 - in.w / 2, out.y + out.h - in.h - hpad, in.w, in.h };

    case BOTTOMLEFT:
        return (MediaRect) { out.x + hpad, out.y + out.h - in.h - hpad, in.w, in.h };

    case LEFT:
        return (MediaRect) { out.x + hpad, out.y + out.h / 2 - in.h / 2, in.w, in.h };

    case CENTER:
        return (MediaRect) { out.x + out.w / 2 - in.w / 2, out.y + out.h / 2 - in.h / 2, in.w, in.h };

    default:
        return (MediaRect) {0, 0, 0, 0};
    }
}


/**
 * Used for timing operations.
 */
struct MediaTimer {
    int64_t time;
    uint32_t duration;

    MediaTimer(uint32_t duration);
    inline void update(uint32_t delta);
    inline bool done();
};

inline void MediaTimer::update(uint32_t delta)
{
    this->time -= delta;
}

inline bool MediaTimer::done()
{
    if (this->time <= 0) {
        this->time = this->duration;
        return true;
    } else {
        return false;
    }
}

/**
 * Used in place of Surfaces and textures.
 * 
 * Note: ALWAYS pass REFERENCES to MediaObjects in functions, else they will be
 * destructed by the function call copying over the values.
 */
struct MediaObject {
    SDL_Texture *texture;
    MediaRect clip_rect;

    /// Explicitly used to reassign textures. Automatically frees an existing
    /// allocated texture if it exists.
    void set(SDL_Texture *texture);

    /// Frees the texture,
    void free();

    inline void align(MediaRect k, MediaGravity g = CENTER, int hpad = 0, int vpad = 0);
    inline void scale(int sw, int sh);
    
    // The texture must be null by default.
    MediaObject(): texture(nullptr) {}

    ~MediaObject() {
        this->free();
        this->texture = nullptr;
    }
};

inline void MediaObject::align(MediaRect k, MediaGravity g, int hpad, int vpad) {
    clip_rect = _rect_align(clip_rect, k, g, hpad, vpad);
}

inline void MediaObject::scale(int sw, int sh) {
    clip_rect.w *= sw;
    clip_rect.h *= sh;
}



/// Typedef used for function arguments to pass a MediaObject.

typedef MediaObject& MediaObjectRef;

inline MediaRect scale_rect(MediaObjectRef k, int sw, int sh) {
    return { k.clip_rect.x, k.clip_rect.y, k.clip_rect.w * sw, k.clip_rect.h * sh };
}

inline MediaRect scale_rect(MediaRect &k, int sw, int sh) {
    return { k.x, k.y, k.w * sw, k.h * sh };
}

/*
 * =============================================================================
 * MediaState
 * =============================================================================
 */

/// Driver class for the Media
class MediaState {
    protected:
        static const size_t   ERROR_MESSAGE_SIZE          = 1024 * 8;
        static constexpr char err_msg[ERROR_MESSAGE_SIZE] = "";

    public:
        SDL_Window *w;           /// Default Window
        SDL_Renderer *r;         /// Default Renderer
        SDL_Event e;             /// Events
        TTF_Font *font;          /// Default Font
        MediaFPSCounter fps;     /// FPS tracker
        const char *sdl_err_msg; /// Error Message Pointer
        bool active;             /// Is frame loop active?
        int max_fps;             /// Maximum FPS of game
        int main_w;              /// Main window width
        int main_h;              /// Main window height
        uint32_t delta;          /// Delta Time

        MediaState(
            int w = 800,
            int h = 600,
            int max_fps = 60,
            const char *window_name = "Game",
            const char *font_path = "font.otb"
        );
        ~MediaState();
        inline void loop_start();
        inline void loop_end();
        inline float get_fps();
};

inline void MediaState::loop_start()
{
    this->fps.start = SDL_GetTicks();
}

inline void MediaState::loop_end()
{
    this->fps.end = SDL_GetTicks();
    this->fps.elapsed = this->fps.end - this->fps.start;

    if (this->fps.elapsed < this->max_fps)
        SDL_Delay(this->max_fps - this->fps.elapsed);

    this->fps.value = 1000 / (float) (SDL_GetTicks() - this->fps.start);
}

inline float MediaState::get_fps()
{
    return this->fps.value;
}

/*
 * =============================================================================
 * MediaGraphics
 * =============================================================================
 */

class MediaGraphics {
    protected:
        MediaState &m;

    public:
        /*
         * Note: SDL uses NULL pointers for denoting certain values
         * (particularly Rects). Hence we can't really use reference variables
         * as arguments in some of these functions.
         */

        // Basic

        MediaGraphics(MediaState &m): m(m) {};
        inline void paint(const MediaObject &m, const MediaRect dest);
        inline void paint(const MediaObject &m);
        inline int set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        inline int set_color(MediaColor c);
        inline void clear();
        inline void present();

        // Graphics Primitives

        inline int point(int x, int y);
        inline int points(const MediaPoint *points, int count);
        inline int points(const std::vector<MediaPoint> &points);

        inline int line(int x1, int x2, int y1, int y2);
        inline int line(const MediaRect dims);
        inline int lines(const MediaPoint *points, int count);
        inline int lines(const std::vector<MediaPoint> &points);

        inline int rect(const MediaRect &rect);
        inline int rects(const MediaRect *rects, int count);
        inline int rects(const std::vector<MediaRect> &rects);
        inline int frect(const MediaRect &rect);
        inline int frects(const MediaRect *rects, int count);
        inline int frects(const std::vector<MediaRect> &rects);

        // Text and Images

        void text(MediaObjectRef k, const char *str, MediaColor c);
        void text(MediaObjectRef k, const char *str);
        void text(MediaObjectRef k, std::string str, MediaColor c);
        void text(MediaObjectRef k, std::string str);
};

inline void MediaGraphics::paint(const MediaObject &k, const MediaRect dest)
{
    SDL_RenderCopy(m.r, k.texture, nullptr, &dest);
}

inline void MediaGraphics::paint(const MediaObject &k)
{
    // printf("At: %s, %d, %s\n", __PRETTY_FUNCTION__, __LINE__, __FILE__);
    // printf("** %lx\n", (unsigned long int) k.texture);
    MediaRect q = {0, 0, 10, 10};
    int w = SDL_RenderCopy(m.r, k.texture, nullptr, &k.clip_rect);

    char *err;
    if (w < 0)
        printf("Error: %d %s\n", w, SDL_GetError());
}


inline int MediaGraphics::set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return SDL_SetRenderDrawColor(m.r, r, g, b, a);
}

inline void MediaGraphics::clear()
{
    SDL_RenderClear(m.r);
    set_color(0, 0, 0, 255);
}

inline void MediaGraphics::present()
{
    SDL_RenderPresent(m.r);
}

inline int MediaGraphics::point(int x, int y)
{
    return SDL_RenderDrawPoint(m.r, x, y);
}

inline int MediaGraphics::points(const MediaPoint *points, int count)
{
    return SDL_RenderDrawPoints(m.r, points, count);
}

inline int MediaGraphics::points(const std::vector<MediaPoint> &points)
{
    return SDL_RenderDrawLines(m.r, points.data(), points.size());
}

inline int MediaGraphics::line(int x1, int x2, int y1, int y2)
{
    return SDL_RenderDrawLine(m.r, x1, x1, y1, y2);
}

inline int MediaGraphics::line(MediaRect k)
{
    return SDL_RenderDrawLine(m.r, k.x, k.y, k.x + k.w, k.y + k.h);
}

inline int MediaGraphics::lines(const MediaPoint *points, int count)
{
    return SDL_RenderDrawLines(m.r, points, count);
}

inline int MediaGraphics::lines(const std::vector<MediaPoint> &points)
{
    return SDL_RenderDrawLines(m.r, points.data(), points.size());
}

inline int MediaGraphics::rect(const MediaRect &rect)
{
    return SDL_RenderDrawRect(m.r, &rect);
}

inline int MediaGraphics::rects(const MediaRect *rects, int count)
{
    return SDL_RenderDrawRects(m.r, rects, count);
}

inline int MediaGraphics::rects(const std::vector<MediaRect> &rects)
{
    return SDL_RenderDrawRects(m.r, rects.data(), rects.size());
}

inline int MediaGraphics::frect(const MediaRect &rect)
{
    return SDL_RenderFillRect(m.r, &rect);
}

inline int MediaGraphics::frects(const MediaRect *rects, int count)
{
    return SDL_RenderFillRects(m.r, rects, count);
}

inline int MediaGraphics::frects(const std::vector<MediaRect> &rects)
{
    return SDL_RenderFillRects(m.r, rects.data(), rects.size());
}

#endif
