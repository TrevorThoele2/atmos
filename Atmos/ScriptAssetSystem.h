#pragma once

#include "AssetSystem.h"

#include "ScriptAsset.h"

namespace Atmos::Asset
{
    class ScriptAssetSystem : public AssetSystem<ScriptAsset>
    {
    public:
        ScriptAssetSystem(ObjectManager& manager);
    protected:
        void InitializeImpl() override;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Asset::ScriptAssetSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::Asset::ScriptAssetSystem, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}