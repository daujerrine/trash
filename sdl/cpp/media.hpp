#ifndef MEDIA_H
#define MEDIA_H

#include <vector>
#include <string>

#include <cstdint>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

typedef SDL_Rect MediaRect;
typedef SDL_Point MediaPoint;

struct MediaFPSCounter {
    uint64_t start;
    uint64_t end;
    uint64_t elapsed;
    float value;
};

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

struct MediaColor {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    MediaColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a):
        r(r), g(g), b(b), a(a) {}
};

/**
 * Used in place of Surfaces and textures
 */

typedef struct MediaObject {
    SDL_Texture *texture;
    MediaRect clip_rect;

    MediaObject();
    ~MediaObject();
} MediaObject;

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
        inline void paint(const MediaObject *m, const MediaRect *dest);
        inline int set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
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

        inline int rect(const MediaRect *rect);
        inline int rects(const MediaRect *rects, int count);
        inline int rects(const std::vector<MediaRect> &rects);
        inline int frect(const MediaRect *rect);
        inline int frects(const MediaRect *rects, int count);
        inline int frects(const std::vector<MediaRect> &rects);

        // Text and Images
        MediaObject text(const char *str);
        MediaObject text(std::string &str);
};

inline void MediaGraphics::paint(const MediaObject *m, const MediaRect *dest)
{
    if (dest)
        SDL_RenderCopy(this->m.r, m->texture, NULL, dest);
    else
        SDL_RenderCopy(this->m.r, m->texture, NULL, &m->clip_rect);
}


inline int MediaGraphics::set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return SDL_SetRenderDrawColor(this->m.r, r, g, b, a);
}

inline void MediaGraphics::clear()
{
    SDL_RenderClear(this->m.r);
}

inline void MediaGraphics::present()
{
    SDL_RenderPresent(this->m.r);
}

inline int MediaGraphics::point(int x, int y)
{
    return SDL_RenderDrawPoint(this->m.r, x, y);
}

inline int MediaGraphics::points(const MediaPoint *points, int count)
{
    return SDL_RenderDrawPoints(this->m.r, points, count);
}

inline int MediaGraphics::points(const std::vector<MediaPoint> &points)
{
    return SDL_RenderDrawLines(this->m.r, points.data(), points.size());
}

inline int MediaGraphics::line(int x1, int x2, int y1, int y2)
{
    return SDL_RenderDrawLine(this->m.r, x1, x1, y1, y2);
}

inline int MediaGraphics::line(MediaRect k)
{
    return SDL_RenderDrawLine(this->m.r, k.x, k.y, k.x + k.w, k.y + k.h);
}

inline int MediaGraphics::lines(const MediaPoint *points, int count)
{
    return SDL_RenderDrawLines(this->m.r, points, count);
}

inline int MediaGraphics::lines(const std::vector<MediaPoint> &points)
{
    return SDL_RenderDrawLines(this->m.r, points.data(), points.size());
}

inline int MediaGraphics::rect(const MediaRect *rect)
{
    return SDL_RenderDrawRect(this->m.r, rect);
}

inline int MediaGraphics::rects(const MediaRect *rects, int count)
{
    return SDL_RenderDrawRects(this->m.r, rects, count);
}

inline int MediaGraphics::rects(const std::vector<MediaRect> &rects)
{
    return SDL_RenderDrawRects(this->m.r, rects.data(), rects.size());
}

inline int MediaGraphics::frect(const MediaRect *rect)
{
    return SDL_RenderFillRect(this->m.r, rect);
}

inline int MediaGraphics::frects(const MediaRect *rects, int count)
{
    return SDL_RenderFillRects(this->m.r, rects, count);
}

inline int MediaGraphics::frects(const std::vector<MediaRect> &rects)
{
    return SDL_RenderFillRects(this->m.r, rects.data(), rects.size());
}

#endif
