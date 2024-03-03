#pragma once

#include "AssetSystem.h"

#include "ShaderAsset.h"

#include "Serialization.h"

namespace Atmos
{
    class ShaderAssetSystem : public AssetSystem<ShaderAsset>
    {
    public:
        ShaderAssetSystem(ObjectManager& manager);
    private:
        void InitializeImpl() override;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::ShaderAssetSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::ShaderAssetSystem, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };
}