#ifndef MEDIA_H
#define MEDIA_H

#include <stdint.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

typedef SDL_Rect MediaRect;
typedef SDL_Point MediaPoint;

typedef struct MediaFPSCounter {
    uint64_t start;
    uint64_t end;
    uint64_t elapsed;
    float value;
} MediaFPSCounter;

/**
 * Used in place of Surfaces and textures
 */
typedef struct MediaObject {
    SDL_Texture *texture;
    MediaRect clip_rect;
} MediaObject;

typedef struct MediaState {
    SDL_Window *window;     /// Default Window
    SDL_Renderer *renderer; /// Default Renderer
    SDL_Event e;            /// Events
    TTF_Font *font;         /// Default Font
    MediaFPSCounter fps;    /// FPS tracker
    const char *err_msg;    /// Error Message Pointer
    int active;             /// Is frame loop active?
    int max_fps;            /// Maximum FPS of game
    int main_w;             /// Main window width
    int main_h;             /// Main window height
    uint32_t delta;         /// Delta Time
} MediaState;


typedef struct MediaTimer {
    int64_t time;
    uint32_t duration;
} MediaTimer;

static inline void m_timer_init(MediaTimer *t, uint32_t duration)
{
    t->duration = duration;
    t->time = duration;
}

static inline void m_timer_update(MediaState *s, MediaTimer *t)
{
    t->time -= s->delta;
}

static inline int m_timer_done(MediaTimer *t)
{
    if (t->time <= 0) {
        t->time = t->duration;
        return 1;
    } else {
        return 0;
    }
}

int m_init(MediaState *s, char *window_name, char *font_path);

static inline void m_loop_start(MediaState *s)
{
    s->fps.start = SDL_GetTicks();
}

static inline void m_loop_end(MediaState *s)
{
    s->fps.end = SDL_GetTicks();
    s->fps.elapsed = s->fps.end - s->fps.start;

    if (s->fps.elapsed < s->max_fps)
        SDL_Delay(s->max_fps - s->fps.elapsed);

    s->fps.value = 1000 / (float) (SDL_GetTicks() - s->fps.start);
}

static inline float m_get_fps(MediaState *s)
{
    return s->fps.value;
}

MediaObject m_text(MediaState *s, const char *str);

static inline void m_paint(MediaState *s, MediaObject *m, MediaRect *dest)
{
    if (dest)
        SDL_RenderCopy(s->renderer, m->texture, NULL, dest);
    else
        SDL_RenderCopy(s->renderer, m->texture, NULL, &m->clip_rect);
}

void m_objfree(MediaObject *m);

void m_close(MediaState *s);

/*
 * Primitives
 */

static inline int m_set_color(MediaState *s, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return SDL_SetRenderDrawColor(s->renderer, r, g, b, a);
}

static inline int m_line(MediaState *s, int x1, int y1, int x2, int y2)
{
    return SDL_RenderDrawLine(s->renderer, x1, y1, x2, y2);
}

static inline int m_lines(MediaState *s, const MediaPoint *points, int count)
{
    return SDL_RenderDrawLines(s->renderer, points, count);
}

static inline int m_point(MediaState *s, int x, int y)
{
    return SDL_RenderDrawPoint(s->renderer, x, y);
}

static inline int m_points(MediaState *s, const MediaPoint *points, int count)
{
    return SDL_RenderDrawPoints(s->renderer, points, count);
}

static inline int m_rect(MediaState *s, const MediaRect *rect)
{
    return SDL_RenderDrawRect(s->renderer, rect);
}

static inline int m_rects(MediaState *s, const MediaRect *rects, int count)
{
    return SDL_RenderDrawRects(s->renderer, rects, count);
}

static inline int m_frect(MediaState *s, const MediaRect *rect)
{
    return SDL_RenderFillRect(s->renderer, rect);
}

static inline int m_frects(MediaState *s, const MediaRect *rects, int count)
{
    return SDL_RenderFillRects(s->renderer, rects, count);
}

#endif
