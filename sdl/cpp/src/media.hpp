#ifndef MEDIA_H
#define MEDIA_H

#include <vector>
#include <map>
#include <string>

#include <cstdint>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "config.h"

#define PRINT_LINE printf("At: %s, %d, %s\n", __PRETTY_FUNCTION__, __LINE__, __FILE__);
#define PRINTRECT(_r) (printf("rect(%d, %d, %d, %d)\n", _r.x, _r.y, _r.w, _r.h))

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

namespace media {

typedef SDL_Rect Rect;
typedef SDL_Point Point;
typedef SDL_Color Color;
typedef SDL_Texture Texture;
typedef SDL_Surface Surface;

typedef Mix_Music MusicData;
typedef Mix_Chunk SoundData;


struct Exception {
    enum ExceptionType {
        SUBSYSTEM_SDL,
        SUBSYSTEM_TTF,
        SUBSYSTEM_IMG,
        SUBSYSTEM_MIXER,
        SUBSYSTEM_NET,
    };

    const char *msg;
    int err_code;
    
    Exception(char *msg, int err_code = -1): msg(msg), err_code(err_code) {}

    Exception(ExceptionType s, int err_code = -1): err_code(err_code)
    {
        switch (s) {
        case SUBSYSTEM_SDL:
            msg = SDL_GetError();
            break;

        case SUBSYSTEM_TTF:
            msg = TTF_GetError();
            break;

        case SUBSYSTEM_IMG:
            msg = IMG_GetError();
            break;

        case SUBSYSTEM_MIXER:
            msg = Mix_GetError();
            break;
        
        case SUBSYSTEM_NET:
        default:
            msg = "Unknown Error";
            break;
        }
    }
};

/// Widget Gravity
typedef enum Gravity {
    CENTER,
    TOPLEFT,
    TOP,
    TOPRIGHT,
    RIGHT,
    BOTTOMRIGHT,
    BOTTOM,
    BOTTOMLEFT,
    LEFT
} Gravity;


struct Size {
    int w;
    int h;
};

struct FPSCounter {
    uint64_t start;
    uint64_t end;
    uint64_t elapsed;
    float value;
};

/*
 * =============================================================================
 * Util
 * =============================================================================
 */

/**
 * Generic Utility functions go here.
 */
namespace Util {

/**
 * Aligns an outer rectangle with an inner one.
 */

static inline Rect rect_align(Rect out, Rect in, Gravity g, int hpad, int vpad)
{
    switch (g) {
    case TOPLEFT:
        return (Rect) { out.x + hpad, out.y + vpad, in.w, in.h };

    case TOP:
        return (Rect) { out.x + out.w / 2 - in.w / 2, out.y + vpad, in.w, in.h };

    case TOPRIGHT:
        return (Rect) { out.x + out.w - in.w - hpad, out.y + vpad, in.w, in.h};

    case RIGHT:
        return (Rect) { out.x + out.w - in.w - hpad, out.y + out.h / 2 - in.h / 2, in.w, in.h };

    case BOTTOMRIGHT:
        return (Rect) { out.x + out.w - in.w - hpad, out.y + out.h - in.h - vpad, in.w, in.h };

    case BOTTOM:
        return (Rect) { out.x + out.w / 2 - in.w / 2, out.y + out.h - in.h - hpad, in.w, in.h };

    case BOTTOMLEFT:
        return (Rect) { out.x + hpad, out.y + out.h - in.h - hpad, in.w, in.h };

    case LEFT:
        return (Rect) { out.x + hpad, out.y + out.h / 2 - in.h / 2, in.w, in.h };

    case CENTER:
        return (Rect) { out.x + out.w / 2 - in.w / 2, out.y + out.h / 2 - 1 - in.h / 2, in.w, in.h };

    default:
        return (Rect) {0, 0, 0, 0};
    }
}


static inline bool point_in_rect(int x, int y, Rect rect)
{
    return ((x) >= (rect).x && (y) >= (rect).y &&
            (x) <= (rect).x + (rect).w && (y) <= (rect).y + (rect).h);
}

}; // namespace Util end

/*
 * =============================================================================
 * Sound
 * =============================================================================
 */

class Audio {
    public:
        enum Loops {
            LOOP_FOREVER = 0,
            LOOP_ONCE = 1
        };
};

/**
 * Encapsulation structure for most sdl_mixer commands for audio samples.
 * Most of these functions are not specific to a sample instance, hence a
 * separate class has been created to make these functions accessible.
 */
struct SoundControl {
    static int fade_out(int duration, int channel = -1);
    static void pause(int channel = -1);
    static void resume(int channel = -1);
    static void stop(int channel = -1);
    static void expire(int ticks, int channel = -1);

    static void finished(void (*callback)(int channel));
    static int paused(int channel);
    static int playing(int channel);
};

/**
 * Encapsulation structure for most sdl_mixer commands for music.
 * Most of these functions are not specific to a music instance, hence a
 * separate class has been created to make these functions accessible.
 */
struct MusicControl {
    static int set_volume(int volume);
    static int fade_out(int duration);
    static int seek_to(double pos);
    static void rewind();
    static void pause();
    static void resume();
    static void stop();

    static void finished(void (*callback)());
    static int paused();
    static int playing();
};

class Sound : public Audio {
    private:
        SoundData *data;

    public:
        Sound(std::string filepath)
        {
            data = Mix_LoadWAV(filepath.c_str());
        }

        ~Sound()
        {
            Mix_FreeChunk(data);
        }

        bool fail() { return data == nullptr; }

        int set_volume(int volume);
        int play(int channel = -1, int loops = LOOP_ONCE);
        int fade_in(int duration, int channel = -1, int loops = LOOP_ONCE);

        using m = SoundControl;

        int fade_out(int duration, int channel = -1) { return m::fade_out(duration, channel); }
        void pause(int channel = -1)                 { m::pause(channel); }
        void resume(int channel = -1)                { m::resume(channel); }
        void stop(int channel = -1)                  { m::stop(channel); }
        void expire(int ticks, int channel = -1)     { m::expire(ticks, channel); }

        int paused(int channel = -1)                 { return m::paused(channel); }
        int playing(int channel = -1)                { return m::playing(channel); }
};

class Music : public Audio {
    private:
        MusicData *data = nullptr;

    public:
        Music(std::string filepath)
        {
            data = Mix_LoadMUS(filepath.c_str());
        }

        ~Music()
        {
            Mix_FreeMusic(data);
        }

        bool fail() { return data == nullptr; }

        using m = MusicControl;

        int set_volume(int volume) { return m::set_volume(volume); }
        int play(int loops = LOOP_ONCE);
        int fade_in(int duration, int loops = LOOP_ONCE);

        int fade_out(int duration) { return m::fade_out(duration); }
        int seek_to(double pos)    { return m::seek_to(pos); }
        void rewind()              { m::rewind(); }
        void pause()               { m::pause(); }
        void resume()              { m::resume(); }
        void stop()                { m::stop(); }
    
        int paused()               { return m::paused(); }
        int playing()              { return m::playing(); }
};


/*
 * =============================================================================
 * Timer
 * =============================================================================
 */

/**
 * Used for timing operations.
 */
struct Timer {
    int64_t time;
    uint32_t duration;

    Timer(uint32_t duration)
    {
        this->duration = duration;
        this->time = duration;
    }

    inline void update(uint32_t delta);
    inline bool done();
    inline uint32_t delay();
};

inline void Timer::update(uint32_t delta)
{
    time -= delta;
}

inline bool Timer::done()
{
    if (time <= 0) {
        time = duration;
        return true;
    } else {
        return false;
    }
}

inline uint32_t Timer::delay()
{
    if (time > 0) {
        return 0;
    } else {
        return duration - time; 
    }
}

/*
 * =============================================================================
 * Object
 * =============================================================================
 */

/**
 * Used in place of Surfaces and textures.
 * 
 * @note ALWAYS pass REFERENCES to Objects in functions, else they will be
 * destructed by the function call copying over the values.
 */

struct Object {
    Texture *texture;
    Rect dest_rect;

    /// Explicitly used to reassign textures. Automatically frees an existing
    /// allocated texture if it exists.
    virtual void set(SDL_Texture *texture);

    /// Frees the texture,
    void free();

    inline void align(Rect k, Gravity g = CENTER, int hpad = 0, int vpad = 0);
    inline void scale(int sw, int sh);
    inline Size tx_dims();
    inline void set_size(int w, int h);
    inline void set_rect(Rect k);
    
    // The texture must be null by default.
    Object(): texture(nullptr) {}

    virtual ~Object() {
        this->free();
        this->texture = nullptr;
    }
};

inline void Object::align(Rect k, Gravity g, int hpad, int vpad)
{
    // printf("Align\n");
    // PRINTRECT(dest_rect);
    // PRINTRECT(k);
    // PRINTRECT(Util::rect_align(k, dest_rect, g, hpad, vpad));
    dest_rect = Util::rect_align(k, dest_rect, g, hpad, vpad);
}

inline void Object::scale(int sw, int sh)
{
    dest_rect.w *= sw;
    dest_rect.h *= sh;
}

inline Size Object::tx_dims()
{
    Size k;
    SDL_QueryTexture(texture, nullptr, nullptr, &k.w, &k.h);
    return k;
}

inline void Object::set_size(int w, int h)
{
    // printf("Setting rectangle: %d %d\n", w, h);
    dest_rect.w = w;
    dest_rect.h = h;
}

inline void Object::set_rect(Rect k)
{
    dest_rect = k;
}

/*
 * -----------------------------------------------------------------------------
 * ClipObject
 * -----------------------------------------------------------------------------
 */


/**
 * Used for objects that may be clipped alongside being scaled.
 */

struct ClipObject : Object {

    Rect src_rect;
    Rect *src_rect_ptr = nullptr;

    int w;
    int h;

    /// Overridden to store actual width and height as well.
    void set(SDL_Texture *texture);

    /// Clips the src rect and the width and height of the dest rect.
    inline void clip(int w, int h);
    inline void clip(int w);
    inline void clip(Rect k);

    /// Clips only the src rect
    inline void clip_src(int w, int h);
    inline void clip_src(int w);
    inline void clip_src(Rect k);

    /// Clears clipping of src rect and dest rect.
    inline void clip_clear();
    inline void clip_clear_src();

    /// Used to control element overflow in things like GUI elements.
    inline void overflow_x(Rect bounds);

    ~ClipObject() {}
};

inline void ClipObject::clip_src(int w, int h)
{
    src_rect_ptr = &src_rect;
    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.w = w;
    src_rect.h = h;
}

inline void ClipObject::clip_src(int w)
{
    src_rect_ptr = &src_rect;
    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.w = w;
}


inline void ClipObject::clip_src(Rect k)
{
    src_rect_ptr = &src_rect;
    src_rect = k;
}

inline void ClipObject::clip(int w, int h)
{
    clip_src(w, h);
    dest_rect.w = w;
    dest_rect.h = h;
}

inline void ClipObject::clip(int w)
{
    clip_src(w);
    dest_rect.w = w;
}

inline void ClipObject::overflow_x(Rect bounds)
{
    if (  dest_rect.w > bounds.w ||
        ((src_rect_ptr != nullptr) && (src_rect.w < w))) {
        clip(bounds.w, h);
    }
}

inline void ClipObject::clip(Rect k)
{
    clip_src(k);
    dest_rect.w = k.w;
    dest_rect.h = k.h;
}

inline void ClipObject::clip_clear_src()
{
    src_rect_ptr = nullptr;
}

inline void ClipObject::clip_clear()
{
    src_rect_ptr = nullptr;
    Size k = tx_dims();
    dest_rect.w = k.w;
    dest_rect.h = k.h;
}

/// Typedef used for function arguments to pass a Object.
typedef Object & ObjectRef;
typedef ClipObject & ClipObjectRef;

/*
 * =============================================================================
 * State
 * =============================================================================
 */

/// Driver class.
class State {
    protected:
        static const size_t   ERROR_MESSAGE_SIZE          = 1024 * 8;
        static constexpr char err_msg[ERROR_MESSAGE_SIZE] = "";
        const char *sdl_err_msg; /// Error Message Pointer
        bool fail_flag = false;  /// Has initialisation failed?

    /// @todo handle error throws
    public:
        SDL_Window *w;       /// Default Window
        SDL_Renderer *r;     /// Default Renderer
        SDL_Event e;         /// Events
        TTF_Font *font;      /// Default Font
        FPSCounter fps; /// FPS tracker
        bool active;         /// Is frame loop active?
        int max_fps;         /// Maximum FPS of game
        int main_w;          /// Main window width
        int main_h;          /// Main window height
        uint32_t delta;      /// Delta Time

#if GAME_DEBUG_BUILD
        std::map<std::string, std::string> debug_keys;
#endif

        State(
            int w = 800,
            int h = 600,
            int max_fps = 60,
            const char *window_name = "Engine",
            const char *font_path = "assets/font.otb"
        );
        ~State();

        const char *get_err();
        bool fail();
        void print_err();
        void display_err();

        inline void loop_start();
        inline void loop_end();
        inline float get_fps();
};

inline void State::loop_start()
{
    this->fps.start = SDL_GetTicks();
}

inline void State::loop_end()
{
    this->fps.end = SDL_GetTicks();
    this->fps.elapsed = this->fps.end - this->fps.start;

    if (this->fps.elapsed < this->max_fps)
        SDL_Delay(this->max_fps - this->fps.elapsed);


    this->delta = (SDL_GetTicks() - this->fps.start);

    this->fps.value = 1000 / (float) this->delta;
}

inline float State::get_fps()
{
    return this->fps.value;
}

/*
 * =============================================================================
 * Text
 * =============================================================================
 */

/**
 * Caching monospace TTF/Bitmap font renderer.
 */

class Text {
    public:
        enum class FontDataType {
            FONT_DATA_STANDARD,
            FONT_DATA_IMAGE
        };

    protected:
        State &m;
        // We cache any glyphs we use.
        TTF_Font *font;
        Texture *glyph_tx;
        Rect std_glyph_offsets[128]; // Usually we don't really access glyphs
                                          // above 128
        std::map<uint32_t, Texture *> ext_glyphs; // Any extra glyphs we need

        inline Texture *get_glyph(uint32_t glyph);

    public:
        void init(FontDataType ft, char *font_path);

        Text(State &m, FontDataType ft, std::string font_path): m(m)
        {
            font = TTF_OpenFont(font_path.c_str(), 16);
            if (!font) {
                printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>> Font null\n");
            }
        }

        ~Text()
        {
            TTF_CloseFont(font);
        }

        void set_glyph_spacing(int spacing) {}
        void set_line_spacing(int spacing) {}

        void text(ObjectRef k, const char *str, Color c);
        void text(ObjectRef k, const char *str);
        void text(ObjectRef k, std::string str, Color c);
        void text(ObjectRef k, std::string str);

        void wrap_text(ObjectRef k, const char *str, Color c, Rect wrap_rect);
        void wrap_text(ObjectRef k, const char *str, Rect wrap_rect);
        void wrap_text(ObjectRef k, std::string str, Color c, Rect wrap_rect);
        void wrap_text(ObjectRef k, std::string str, Rect wrap_rect);
};

/*
 * =============================================================================
 * Graphics
 * =============================================================================
 */

class Graphics {
    protected:
        State &m;

    public:
        /*
         * Note: SDL uses NULL pointers for denoting certain values
         * (particularly Rects). Hence we can't really use reference variables
         * as arguments in some of these functions.
         */

        // Basic

        Graphics(State &m): m(m) {};

        inline void paint(const ObjectRef k, const Rect &src, const Rect &dest);
        inline void paint(const ObjectRef k, const Rect &src);
        inline void paint(const ObjectRef k);
        inline void paint(const ClipObjectRef k);

        inline void paint_clip(const ObjectRef k, const Rect &src);

        inline int set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        inline int set_color(Color c);

        inline int set_paint_target(const ObjectRef k); /// Set render target
        inline void clear();   /// Called at start of draw loop
        inline void present(); /// Called at end of draw loop

        // Graphics Primitives

        inline int point(int x, int y);
        inline int points(const Point *points, int count);
        inline int points(const std::vector<Point> &points);

        inline int line(int x1, int x2, int y1, int y2);
        inline int line(const Rect dims);
        inline int lines(const Point *points, int count);
        inline int lines(const std::vector<Point> &points);

        inline int rect(const Rect &rect);
        inline int rects(const Rect *rects, int count);
        inline int rects(const std::vector<Rect> &rects);
        inline int frect(const Rect &rect);
        inline int frects(const Rect *rects, int count);
        inline int frects(const std::vector<Rect> &rects);

        // Text and Images
        /// @note Object Makers will always set the dest rectangle to object dims

        void text(ObjectRef k, const char *str, Color c);
        void text(ObjectRef k, const char *str);
        void text(ObjectRef k, std::string str, Color c);
        void text(ObjectRef k, std::string str);

        void image(ObjectRef k, std::string filepath);
};

inline void Graphics::paint(const ObjectRef k, const Rect &src, const Rect &dest)
{
    SDL_RenderCopy(m.r, k.texture, &src, &dest);
}

inline void Graphics::paint(const ObjectRef k, const Rect &src)
{
    SDL_RenderCopy(m.r, k.texture, &src, &k.dest_rect);
}

inline void Graphics::paint(const ObjectRef k)
{
    // PRINT_LINE
    // printf("** %lx\n", (unsigned long int) k.texture);
    SDL_RenderCopy(m.r, k.texture, nullptr, &k.dest_rect);
}

inline void Graphics::paint(const ClipObjectRef k)
{
    // PRINT_LINE
    // printf("** %lx\n", (unsigned long int) k.texture);
    SDL_RenderCopy(m.r, k.texture, k.src_rect_ptr, &k.dest_rect);
}

/// Clip the object to the bounding rectangle's dimensions.
inline void Graphics::paint_clip(const ObjectRef k, const Rect &src)
{
    Rect self_clip = { 0, 0, src.w, src.h }; 
    SDL_RenderCopy(m.r, k.texture, &self_clip, &k.dest_rect);
}

inline int Graphics::set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return SDL_SetRenderDrawColor(m.r, r, g, b, a);
}

inline int Graphics::set_color(Color c)
{
    return SDL_SetRenderDrawColor(m.r, c.r, c.g, c.b, c.a);
}

inline int Graphics::set_paint_target(const ObjectRef k)
{
    return SDL_SetRenderTarget(m.r, k.texture);
}

inline void Graphics::clear()
{
    set_color(0, 0, 0, 255);
    SDL_RenderClear(m.r);
}

inline void Graphics::present()
{
    SDL_RenderPresent(m.r);
}

inline int Graphics::point(int x, int y)
{
    return SDL_RenderDrawPoint(m.r, x, y);
}

inline int Graphics::points(const Point *points, int count)
{
    return SDL_RenderDrawPoints(m.r, points, count);
}

inline int Graphics::points(const std::vector<Point> &points)
{
    return SDL_RenderDrawLines(m.r, points.data(), points.size());
}

inline int Graphics::line(int x1, int x2, int y1, int y2)
{
    return SDL_RenderDrawLine(m.r, x1, x1, y1, y2);
}

inline int Graphics::line(Rect k)
{
    return SDL_RenderDrawLine(m.r, k.x, k.y, k.x + k.w, k.y + k.h);
}

inline int Graphics::lines(const Point *points, int count)
{
    return SDL_RenderDrawLines(m.r, points, count);
}

inline int Graphics::lines(const std::vector<Point> &points)
{
    return SDL_RenderDrawLines(m.r, points.data(), points.size());
}

inline int Graphics::rect(const Rect &rect)
{
    return SDL_RenderDrawRect(m.r, &rect);
}

inline int Graphics::rects(const Rect *rects, int count)
{
    return SDL_RenderDrawRects(m.r, rects, count);
}

inline int Graphics::rects(const std::vector<Rect> &rects)
{
    return SDL_RenderDrawRects(m.r, rects.data(), rects.size());
}

inline int Graphics::frect(const Rect &rect)
{
    return SDL_RenderFillRect(m.r, &rect);
}

inline int Graphics::frects(const Rect *rects, int count)
{
    return SDL_RenderFillRects(m.r, rects, count);
}

inline int Graphics::frects(const std::vector<Rect> &rects)
{
    return SDL_RenderFillRects(m.r, rects.data(), rects.size());
}

};

#endif
