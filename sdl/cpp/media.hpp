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

    MediaTimer();
    inline void update();
    inline bool done();
};

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

/// Driver class for the Media
class MediaState {
    protected:
        static const size_t ERROR_MESSAGE_SIZE                = 1024 * 8;
        static char         err_msg[ERROR_MESSAGE_SIZE] = "";

    private:
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

    public:
        MediaState(int w, int h, int max_fps, const char *font_path);
        ~MediaState();
        inline void loop_start();
        inline void loop_end();
        inline void get_fps();
};

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

        MediaGraphics(MediaState &m);
        inline void paint(const MediaObject *m, const MediaRect *dest);
        inline int set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

        // Graphics Primitives

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

#endif
