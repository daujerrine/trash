#include "audio.hpp"

namespace media {

/*
 * =============================================================================
 * SoundControl
 * =============================================================================
 */

void SoundControl::pause(int channel)
{
    Mix_Pause(channel);
}

void SoundControl::resume(int channel)
{
    Mix_Resume(channel);
}

void SoundControl::stop(int channel)
{
    Mix_HaltChannel(channel);
}

void SoundControl::expire(int channel, int ticks)
{
    Mix_ExpireChannel(channel, ticks);
}

void SoundControl::finished(void (*callback)(int channel))
{
    Mix_ChannelFinished(callback);
}

int SoundControl::paused(int channel)
{
    return Mix_Paused(channel);
}

int SoundControl::playing(int channel)
{
    return Mix_Playing(channel);
}

int SoundControl::fade_out(int duration, int channel)
{
    return Mix_FadeOutChannel(channel, duration);
}


/*
 * =============================================================================
 * Sound
 * =============================================================================
 */

int Sound::set_volume(int volume)
{
    return Mix_VolumeChunk(data, volume);
}

int Sound::play(int channel, int loops)
{
    loops -= 1;
    return Mix_PlayChannel(channel, data, loops);
}

int Sound::fade_in(int duration, int channel, int loops)
{
    loops -= 1;
    return Mix_FadeInChannel(channel, data, loops, duration);
}

/*
 * =============================================================================
 * Music
 * =============================================================================
 */

int Music::play(int loops)
{
    loops -= 1;
    return Mix_PlayMusic(data, loops);
}

int Music::fade_in(int duration, int loops)
{
    loops -= 1;
    return Mix_FadeInMusic(data, loops, duration);
}

/*
 * =============================================================================
 * MusicControl
 * =============================================================================
 */


int MusicControl::seek_to(double pos)
{
    return Mix_SetMusicPosition(pos);
}

int MusicControl::set_volume(int volume)
{
    return Mix_VolumeMusic(volume);
}
 
void MusicControl::rewind()
{
    Mix_RewindMusic();
}

void MusicControl::pause()
{
    Mix_PauseMusic();
}

void MusicControl::stop()
{
    Mix_HaltMusic();
}

void MusicControl::finished(void (*callback)())
{
    Mix_HookMusicFinished(callback);
}

int MusicControl::paused()
{
    return Mix_PausedMusic();
}

int MusicControl::playing()
{
    return Mix_PlayingMusic();
}

int MusicControl::fade_out(int duration)
{
    return Mix_FadeOutMusic(duration);
}


};
