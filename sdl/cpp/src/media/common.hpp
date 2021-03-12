#ifndef MEDIA_COMMON_H
#define MEDIA_COMMON_H

#include <vector>
#include <map>
#include <string>

#include <cstdint>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

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

struct Size {
    int w;
    int h;
};

typedef Mix_Music MusicData;
typedef Mix_Chunk SoundData;

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

};
#endif
