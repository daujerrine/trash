#ifndef MEDIA_H
#define MEDIA_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

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
    SDL_Rect clip_rect;
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
} MediaState;

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

static inline void m_paint(MediaState *s, MediaObject *m, SDL_Rect *dest)
{
    if (dest)
        SDL_RenderCopy(s->renderer, m->texture, NULL, dest);
    else
        SDL_RenderCopy(s->renderer, m->texture, NULL, &m->clip_rect);
}

void m_close(MediaState *s);

#endif
