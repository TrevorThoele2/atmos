#pragma once

#include "AssetSystem.h"

#include "AudioAsset.h"

#include "Serialization.h"

namespace Atmos
{
    class AudioAssetSystem : public AssetSystem<AudioAsset>
    {
    public:
        AudioAssetSystem(ObjectManager& manager);
        INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DECLARE(AudioAssetSystem);
    private:
        void InitializeImpl() override;
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::AudioAssetSystem)
    {
    public:
        INSCRIPTION_BINARY_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_BINARY_DECLARE_CLASS_NAME_RESOLVER;
    };
}