
#pragma once

#include "Sense.h"
#include "SoundBase.h"

#include "Position3D.h"

#include "Serialization.h"

namespace Atmos
{
    class SoundHandler;
    class Sound : public SoundBase, public Sense
    {
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        friend SoundHandler;
        friend AssetRegistry<AudioAsset>;
    private:
        Volume baseVolume;

        void OnPositionChanged() override;
        void OnEnabledChanged() override;

        void PlayImpl() override;
        void StopImpl() override;

        void Init();
        void SetVolumeByCameraPos();
    protected:
        Sound(const AssetReference<AudioAsset> &asset, const Position3D &position, bool loop = false, bool enabled = true);
    public:
        Sound();
        Sound(const Sound &arg);
        Sound(Sound &&arg);
        Sound& operator=(const Sound &arg);
        Sound& operator=(Sound &&arg);

        bool operator==(const Sound &arg) const;
        bool operator!=(const Sound &arg) const;

        void SetBaseVolume(Volume set);
        Volume GetBaseVolume() const;
    };
}