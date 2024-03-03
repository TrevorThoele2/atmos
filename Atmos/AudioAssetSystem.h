#pragma once

#include "AssetSystem.h"

#include "AudioAsset.h"

#include "Serialization.h"

namespace Atmos::Asset
{
    class AudioAssetSystem : public AssetSystem<AudioAsset>
    {
    public:
        AudioAssetSystem(ObjectManager& manager);
    protected:
        void InitializeImpl() override;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Asset::AudioAssetSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::Asset::AudioAssetSystem, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}