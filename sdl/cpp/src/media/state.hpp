#ifndef MEDIA_STATE_H
#define MEDIA_STATE_H

#include "common.hpp"
#include "audio.hpp"
#include "text.hpp"
#include "object.hpp"
#include "timer.hpp"

namespace media {

struct FPSCounter {
    uint64_t start;
    uint64_t end;
    uint64_t elapsed;
    float value;
};

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

};

#include "graphics.hpp"

#endif
