
#include "Sound.h"

#include "AudioRegistry.h"
#include "Camera.h"
#include "Field.h"
#include "WorldManager.h"
#include "Math.h"

#include <Inscription\Scribe.h>
#include <Inscription\Inscripter.h>
#include <Inscription\String.h>
#include <Inscription\Const.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(Sound)
    {
        ::Inscription::BaseSerialize<Sense>(scribe, *this);
        ::Inscription::BaseSerialize<SoundBase>(scribe, *this);
        scribe(baseVolume);
    }

    void Sound::OnPositionChanged()
    {
        SetVolumeByCameraPos();
    }

    void Sound::OnEnabledChanged()
    {
        
    }

    void Sound::PlayImpl()
    {
        WorldManager::GetCurrentField()->sounds.Add(*this);
        StartAsset();
    }

    void Sound::StopImpl()
    {
        WorldManager::GetCurrentField()->sounds.Remove(*this);
        Pause();
    }

    void Sound::Init()
    {
        SetVolumeByCameraPos();
    }

    void Sound::SetVolumeByCameraPos()
    {
        auto distance = Position3D::FindDistance(Position3D(GetPosition()), Camera::GetViewOrigin3D());
        if (distance > baseVolume)
        {
            SetVolumeImpl(0);
            return;
        }

        SetVolumeImpl(GetPercentage(baseVolume - distance, 0.0f, baseVolume));
    }

    Sound::Sound(const AssetReference<AudioAsset> &asset, const Position3D &pos, bool loop, bool enabled) : SoundBase(asset, loop), Sense(pos, enabled)
    {}

    Sound::Sound() : Sense(true)
    {}

    Sound::Sound(const Sound &arg) : SoundBase(arg), Sense(arg), baseVolume(arg.baseVolume)
    {}

    Sound::Sound(Sound &&arg) : SoundBase(std::move(arg)), Sense(std::move(arg)), baseVolume(std::move(arg.baseVolume))
    {}

    Sound& Sound::operator=(const Sound &arg)
    {
        SoundBase::operator=(arg);
        Sense::operator=(arg);
        baseVolume = arg.baseVolume;
        return *this;
    }

    Sound& Sound::operator=(Sound &&arg)
    {
        SoundBase::operator=(std::move(arg));
        Sense::operator=(std::move(arg));
        baseVolume = std::move(arg.baseVolume);
        return *this;
    }

    bool Sound::operator==(const Sound &arg) const
    {
        return SoundBase::operator==(arg) && Sense::operator==(arg) && baseVolume == arg.baseVolume;
    }

    bool Sound::operator!=(const Sound &arg) const
    {
        return !(*this == arg);
    }

    void Sound::SetBaseVolume(Volume set)
    {
        SetVolumeImpl(set);
    }

    Sound::Volume Sound::GetBaseVolume() const
    {
        return baseVolume;
    }
}