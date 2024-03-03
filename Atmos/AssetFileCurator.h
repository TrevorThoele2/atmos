#pragma once

#include <Arca/Curator.h>

#include "ExtractAsset.h"

namespace Atmos::File
{
    class ManagerProvider;
}

namespace Atmos::Asset
{
    class AssetFileCurator final : public Arca::Curator
    {
    public:
        using Curator::Curator;
    public:
        Buffer Handle(const ExtractAsset& command);
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Asset::AssetFileCurator, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Asset::AssetFileCurator, BinaryArchive>
    {};
}