#pragma once

#include "AssetSystem.h"

#include "ScriptAsset.h"

namespace Atmos
{
    class ScriptAssetSystem : public AssetSystem<ScriptAsset>
    {
    public:
        ScriptAssetSystem(ObjectManager& manager);
    private:
        void InitializeImpl() override;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::ScriptAssetSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::ScriptAssetSystem, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };
}