#ifndef MEDIA_OBJECT_H
#define MEDIA_OBJECT_H

#include <SDL2/SDL.h>

#include "common.h"

namespace media {

/*
 * -----------------------------------------------------------------------------
 * Object
 * -----------------------------------------------------------------------------
 */


/**
 * Used in place of Surfaces and textures.
 * 
 * @note ALWAYS pass REFERENCES to Objects in functions, else they will be
 *       destructed by the function call copying over the values.
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

/// Typedef used for function arguments to pass an Object.
typedef Object & ObjectRef;
typedef ClipObject & ClipObjectRef;

};
#endif
