#ifndef MEDIA_H
#define MEDIA_H

#include <vector>
#include <map>
#include <string>

#include <cstdint>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

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

typedef SDL_Rect MediaRect;
typedef SDL_Point MediaPoint;
typedef SDL_Color MediaColor;
typedef SDL_Texture MediaTexture;

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


struct MediaDims {
    int w;
    int h;
};

struct MediaFPSCounter {
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
namespace MediaUtil {

/**
 * Aligns an outer rectangle with an inner one.
 */

static inline MediaRect rect_align(MediaRect out, MediaRect in, MediaGravity g, int hpad, int vpad)
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
        return (MediaRect) { out.x + out.w - in.w - hpad, out.y + out.h - in.h - vpad, in.w, in.h };

    case BOTTOM:
        return (MediaRect) { out.x + out.w / 2 - in.w / 2, out.y + out.h - in.h - hpad, in.w, in.h };

    case BOTTOMLEFT:
        return (MediaRect) { out.x + hpad, out.y + out.h - in.h - hpad, in.w, in.h };

    case LEFT:
        return (MediaRect) { out.x + hpad, out.y + out.h / 2 - in.h / 2, in.w, in.h };

    case CENTER:
        return (MediaRect) { out.x + out.w / 2 - in.w / 2, out.y + out.h / 2 - 1 - in.h / 2, in.w, in.h };

    default:
        return (MediaRect) {0, 0, 0, 0};
    }
}


static inline bool point_in_rect(int x, int y, MediaRect rect)
{
    return ((x) >= (rect).x && (y) >= (rect).y &&
            (x) <= (rect).x + (rect).w && (y) <= (rect).y + (rect).h);
}

}; // namespace Util end

/*
 * =============================================================================
 * MediaSound
 * =============================================================================
 */

/*
class MediaSound {
    
}*/


/*
 * =============================================================================
 * MediaTimer
 * =============================================================================
 */

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

/*
 * =============================================================================
 * MediaObject
 * =============================================================================
 */

/**
 * Used in place of Surfaces and textures.
 * 
 * Note: ALWAYS pass REFERENCES to MediaObjects in functions, else they will be
 * destructed by the function call copying over the values.
 */

struct MediaObject {
    MediaTexture *texture;
    MediaRect dest_rect;

    /// Explicitly used to reassign textures. Automatically frees an existing
    /// allocated texture if it exists.
    virtual void set(SDL_Texture *texture);

    /// Frees the texture,
    void free();

    inline void align(MediaRect k, MediaGravity g = CENTER, int hpad = 0, int vpad = 0);
    inline void scale(int sw, int sh);
    inline MediaDims tx_dims();
    inline void set_size(int w, int h);
    inline void set_rect(MediaRect k);
    
    // The texture must be null by default.
    MediaObject(): texture(nullptr) {}

    virtual ~MediaObject() {
        this->free();
        this->texture = nullptr;
    }
};

inline void MediaObject::align(MediaRect k, MediaGravity g, int hpad, int vpad)
{
    // PRINTRECT(dest_rect);
    // PRINTRECT(k);
    // PRINTRECT(Util::rect_align(k, dest_rect, g, hpad, vpad));
    dest_rect = MediaUtil::rect_align(k, dest_rect, g, hpad, vpad);
}

inline void MediaObject::scale(int sw, int sh)
{
    dest_rect.w *= sw;
    dest_rect.h *= sh;
}

inline MediaDims MediaObject::tx_dims()
{
    MediaDims k;
    SDL_QueryTexture(texture, nullptr, nullptr, &k.w, &k.h);
    return k;
}

inline void MediaObject::set_size(int w, int h)
{
    printf("Setting rectangle: %d %d\n", w, h);
    dest_rect.w = w;
    dest_rect.h = h;
}

inline void MediaObject::set_rect(MediaRect k)
{
    dest_rect = k;
}

/*
 * -----------------------------------------------------------------------------
 * MediaClipObject
 * -----------------------------------------------------------------------------
 */


/**
 * Used for objects that may be clipped alongside being scaled.
 */

struct MediaClipObject : MediaObject {

    MediaRect src_rect;
    MediaRect *src_rect_ptr = nullptr;

    int w;
    int h;

    /// Overridden to store actual width and height as well.
    void set(SDL_Texture *texture);

    /// Clips the src rect and the width and height of the dest rect.
    inline void clip(int w, int h);
    inline void clip(int w);
    inline void clip(MediaRect k);

    /// Clips only the src rect
    inline void clip_src(int w, int h);
    inline void clip_src(int w);
    inline void clip_src(MediaRect k);

    /// Clears clipping of src rect and dest rect.
    inline void clip_clear();
    inline void clip_clear_src();

    /// Used to control element overflow in things like GUI elements.
    inline void overflow_x(MediaRect bounds);

    ~MediaClipObject() {}
};

inline void MediaClipObject::clip_src(int w, int h)
{
    src_rect_ptr = &src_rect;
    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.w = w;
    src_rect.h = h;
}

inline void MediaClipObject::clip_src(int w)
{
    src_rect_ptr = &src_rect;
    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.w = w;
}


inline void MediaClipObject::clip_src(MediaRect k)
{
    src_rect_ptr = &src_rect;
    src_rect = k;
}

inline void MediaClipObject::clip(int w, int h)
{
    clip_src(w, h);
    dest_rect.w = w;
    dest_rect.h = h;
}

inline void MediaClipObject::clip(int w)
{
    clip_src(w);
    dest_rect.w = w;
}

inline void MediaClipObject::overflow_x(MediaRect bounds)
{
    if (  dest_rect.w > bounds.w ||
        ((src_rect_ptr != nullptr) && (src_rect.w < w))) {
        clip(bounds.w, h);
    }
}

inline void MediaClipObject::clip(MediaRect k)
{
    clip_src(k);
    dest_rect.w = k.w;
    dest_rect.h = k.h;
}

inline void MediaClipObject::clip_clear_src()
{
    src_rect_ptr = nullptr;
}

inline void MediaClipObject::clip_clear()
{
    src_rect_ptr = nullptr;
    MediaDims k = tx_dims();
    dest_rect.w = k.w;
    dest_rect.h = k.h;
}

/// Typedef used for function arguments to pass a MediaObject.
typedef MediaObject & MediaObjectRef;
typedef MediaClipObject & MediaClipObjectRef;

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
        const char *sdl_err_msg; /// Error Message Pointer
        bool fail_flag = false;  /// Has initialisation failed?

    /// @todo handle error throws
    public:
        SDL_Window *w;       /// Default Window
        SDL_Renderer *r;     /// Default Renderer
        SDL_Event e;         /// Events
        TTF_Font *font;      /// Default Font
        MediaFPSCounter fps; /// FPS tracker
        bool active;         /// Is frame loop active?
        int max_fps;         /// Maximum FPS of game
        int main_w;          /// Main window width
        int main_h;          /// Main window height
        uint32_t delta;      /// Delta Time

#if GAME_DEBUG_BUILD
        std::map<std::string, std::string> debug_keys;
#endif

        MediaState(
            int w = 800,
            int h = 600,
            int max_fps = 60,
            const char *window_name = "Game",
            const char *font_path = "assets/font.otb"
        );
        ~MediaState();

        const char *get_err();
        bool fail();
        void print_err();
        void display_err();

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


    this->delta = (SDL_GetTicks() - this->fps.start);

    this->fps.value = 1000 / (float) this->delta;
}

inline float MediaState::get_fps()
{
    return this->fps.value;
}

/*
 * =============================================================================
 * MediaText
 * =============================================================================
 */

/**
 * Caching monospace TTF/Bitmap font renderer.
 */

class MediaText {
    public:
        enum FontDataType {
            FONT_DATA_STANDARD,
            FONT_DATA_IMAGE
        };

    protected:
        MediaState &m;
        // We cache any glyphs we use.
        TTF_Font *font;
        MediaTexture *glyph_tx;
        MediaRect std_glyph_offsets[128]; // Usually we don't really access glyphs
                                          // above 128
        std::map<uint32_t, MediaTexture *> ext_glyphs; // Any extra glyphs we need

        inline MediaTexture *get_glyph(uint32_t glyph);

    public:
        void calc(FontDataType ft, char *font_path)
        {
            
            for (int i = 0; i < 128; i++) {
                
            }
        }

        MediaText(MediaState &m, FontDataType ft, std::string font_path): m(m) {}
        ~MediaText() {}

        void set_glyph_spacing(int spacing) {}
        void set_line_spacing(int spacing) {}

        void text(MediaObjectRef k, const char *str, MediaColor c);
        void text(MediaObjectRef k, const char *str);
        void text(MediaObjectRef k, std::string str, MediaColor c);
        void text(MediaObjectRef k, std::string str);

        void wrap_text(MediaObjectRef k, const char *str, MediaColor c, MediaRect wrap_rect);
        void wrap_text(MediaObjectRef k, const char *str, MediaRect wrap_rect);
        void wrap_text(MediaObjectRef k, std::string str, MediaColor c, MediaRect wrap_rect);
        void wrap_text(MediaObjectRef k, std::string str, MediaRect wrap_rect);
};

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

        inline void paint(const MediaObjectRef k, const MediaRect &src, const MediaRect &dest);
        inline void paint(const MediaObjectRef k, const MediaRect &src);
        inline void paint(const MediaObjectRef k);
        inline void paint(const MediaClipObjectRef k);

        inline void paint_clip(const MediaObjectRef k, const MediaRect &src);

        inline int set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        inline int set_color(MediaColor c);

        inline int set_paint_target(const MediaObjectRef k); /// Set render target
        inline void clear();   /// Called at start of draw loop
        inline void present(); /// Called at end of draw loop

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
        /// @note Object Makers will always set the dest rectangle to object dims

        void text(MediaObjectRef k, const char *str, MediaColor c);
        void text(MediaObjectRef k, const char *str);
        void text(MediaObjectRef k, std::string str, MediaColor c);
        void text(MediaObjectRef k, std::string str);
};

inline void MediaGraphics::paint(const MediaObjectRef k, const MediaRect &src, const MediaRect &dest)
{
    SDL_RenderCopy(m.r, k.texture, &src, &dest);
}

inline void MediaGraphics::paint(const MediaObjectRef k, const MediaRect &src)
{
    SDL_RenderCopy(m.r, k.texture, &src, &k.dest_rect);
}

inline void MediaGraphics::paint(const MediaObjectRef k)
{
    // PRINT_LINE
    // printf("** %lx\n", (unsigned long int) k.texture);
    SDL_RenderCopy(m.r, k.texture, nullptr, &k.dest_rect);
}

inline void MediaGraphics::paint(const MediaClipObjectRef k)
{
    // PRINT_LINE
    // printf("** %lx\n", (unsigned long int) k.texture);
    SDL_RenderCopy(m.r, k.texture, k.src_rect_ptr, &k.dest_rect);
}

/// Clip the object to the bounding rectangle's dimensions.
inline void MediaGraphics::paint_clip(const MediaObjectRef k, const MediaRect &src)
{
    MediaRect self_clip = { 0, 0, src.w, src.h }; 
    SDL_RenderCopy(m.r, k.texture, &self_clip, &k.dest_rect);
}

inline int MediaGraphics::set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return SDL_SetRenderDrawColor(m.r, r, g, b, a);
}

inline int MediaGraphics::set_color(MediaColor c)
{
    return SDL_SetRenderDrawColor(m.r, c.r, c.g, c.b, c.a);
}

inline int MediaGraphics::set_paint_target(const MediaObjectRef k)
{
    return SDL_SetRenderTarget(m.r, k.texture);
}

inline void MediaGraphics::clear()
{
    set_color(0, 0, 0, 255);
    SDL_RenderClear(m.r);
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
