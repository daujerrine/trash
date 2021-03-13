#ifndef MEDIA_TEXT_H
#define MEDIA_TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "common.hpp"
#include "media.hpp"
#include "object.hpp"

namespace media {

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
                printf("Font not loaded\n");
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

};

#endif
