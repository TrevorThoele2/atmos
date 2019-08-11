#pragma once

#include "AssetSystem.h"

#include "ShaderAsset.h"

#include "Serialization.h"

namespace Atmos::Asset
{
    class ShaderAssetSystem : public AssetSystem<ShaderAsset>
    {
    public:
        ShaderAssetSystem(ObjectManager& manager);
    protected:
        void InitializeImpl() override;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Asset::ShaderAssetSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::Asset::ShaderAssetSystem, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}