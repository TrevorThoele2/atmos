
#include "CurrentMusic.h"
#include "AudioRegistry.h"
#include "WorldManager.h"

namespace Atmos
{
    CurrentMusic::CurrentMusic()
    {
        WorldManager::Instance().eventFinalizeField.Subscribe(&CurrentMusic::OnFinalizeField, *this);
    }

    void CurrentMusic::OnFinalizeField()
    {
        playing.Set();
    }

    CurrentMusic& CurrentMusic::Instance()
    {
        static CurrentMusic instance;
        return instance;
    }

    void CurrentMusic::ChangePlaying(const FileName &name)
    {
        auto &playing = Instance().playing;
        if (playing)
            playing->Stop();

        playing = AssetRegistry<AudioAsset>::CreateMusic(name);
        if (!playing)
            return;

        playing->Play();
    }

    void CurrentMusic::Pause()
    {
        auto &playing = Instance().playing;
        if (playing.IsValid())
            playing->Pause();
    }

    void CurrentMusic::Resume()
    {
        auto &playing = Instance().playing;
        if (playing.IsValid())
            playing->Resume();
    }
}