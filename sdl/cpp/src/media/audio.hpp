#ifndef MEDIA_AUDIO_H
#define MEDIA_AUDIO_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "common.hpp"

namespace media {

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

};

#endif
