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
        AudioAssetSystem(const ::Inscription::Table<AudioAssetSystem>& table);
    private:
        void InitializeImpl() override;
    };
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::AudioAssetSystem)
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}