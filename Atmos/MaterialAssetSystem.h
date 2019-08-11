#pragma once

#include "AssetSystem.h"

#include "MaterialAsset.h"

namespace Atmos::Asset
{
    class MaterialAssetSystem : public AssetSystem<MaterialAsset>
    {
    public:
        MaterialAssetSystem(ObjectManager& manager);
    protected:
        void InitializeImpl() override;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Asset::MaterialAssetSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::Asset::MaterialAssetSystem, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}