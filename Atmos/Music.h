#pragma once

#include "Sound.h"
#include "Serialization.h"

namespace Atmos
{
    class Music : public SoundBase
    {
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        friend AssetRegistry<AudioAsset>;
    private:
        void PlayImpl() override;
        void StopImpl() override;
    protected:
        Music();
        Music(const AssetReference<AudioAsset> &asset);
    public:
        Music(const Music &arg);
        Music(Music &&arg);
        Music& operator=(const Music &arg);
        Music& operator=(Music &&arg);
    };
}