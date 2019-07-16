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
    private:
        void InitializeImpl() override;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::AudioAssetSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::AudioAssetSystem, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };
}