
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

    void CurrentMusic::StopPlaying()
    {
        auto &playing = Instance().playing;
        if (playing)
        {
            playing->Stop();
            playing.Reset();
        }
    }

    void CurrentMusic::ChangePlaying(const FileName &name)
    {
        StopPlaying();

        auto &playing = Instance().playing;
        playing = AssetRegistry<AudioAsset>::CreateMusic(name);
        if (!playing)
        {
            Logger::Log("Cannot find audio asset for music.",
                Logger::Type::ERROR_LOW,
                Logger::NameValueVector{ NameValuePair("File Name", name.GetValue()) });
            return;
        }

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