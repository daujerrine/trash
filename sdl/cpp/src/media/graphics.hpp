#ifndef MEDIA_GRAPHICS_H
#define MEDIA_GRAPHICS_H

#include <vector>
#include <map>
#include <string>

#include <cstdint>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "common.hpp"

namespace media {

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
