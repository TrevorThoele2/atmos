#pragma once

#include "Music.h"
#include "Optional.h"

namespace Atmos
{
    class CurrentMusic
    {
    private:
        Optional<Music> playing;

        CurrentMusic();
        CurrentMusic(const CurrentMusic &arg) = delete;
        CurrentMusic& operator=(const CurrentMusic &arg) = delete;

        void OnFinalizeField();
    public:
        static CurrentMusic& Instance();
        static void StopPlaying();
        static void ChangePlaying(const FileName &name);
        static void Pause();
        static void Resume();
    };
}