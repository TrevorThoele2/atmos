#pragma once

#include "AssetSystem.h"

#include "AudioAsset.h"

#include "Serialization.h"

namespace Atmos
{
    class AudioSystem : public AssetSystem<AudioAsset>
    {
    public:
        AudioSystem(ObjectManager& manager);
        AudioSystem(const ::Inscription::Table<AudioSystem>& table);
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::AudioSystem)
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}