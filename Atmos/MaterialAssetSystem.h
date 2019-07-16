#pragma once

#include "AssetSystem.h"

#include "MaterialAsset.h"

namespace Atmos
{
    class MaterialAssetSystem : public AssetSystem<MaterialAsset>
    {
    public:
        MaterialAssetSystem(ObjectManager& manager);
    private:
        void InitializeImpl() override;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::MaterialAssetSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::MaterialAssetSystem, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };
}