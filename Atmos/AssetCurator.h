#pragma once

#include <Arca/Curator.h>

#include "Asset.h"
#include "MappedAssets.h"

#include "DebugStatistics.h"

namespace Atmos::Asset
{
    template<class T>
    struct AssetCuratorTraits;

    template<class T>
    class AssetCurator : public Arca::Curator
    {
    public:
        void Work();
    protected:
        explicit AssetCurator(Init init);
    private:
        using Traits = AssetCuratorTraits<T>;
    private:
        Arca::GlobalIndex<MappedAssets<T>> mappedAssets;
        Arca::GlobalIndex<Debug::Statistics> debugStatistics;
    private:
        INSCRIPTION_ACCESS;
    };

    template<class T>
    void AssetCurator<T>::Work()
    {
        (debugStatistics->memory.*Traits::debugStatisticsSize) = mappedAssets->Size();
    }

    template<class T>
    AssetCurator<T>::AssetCurator(Init init) :
        Curator(init),
        mappedAssets(init.owner), debugStatistics(init.owner)
    {
        init.owner.ExecuteOn<Arca::CreatedKnown<T>>(
            [this](const Arca::CreatedKnown<T>& signal)
            {
                mappedAssets->map.emplace(signal.index->Name(), signal.index);
            });

        init.owner.ExecuteOn<Arca::DestroyingKnown<T>>(
            [this](const Arca::DestroyingKnown<T>& signal)
            {
                mappedAssets->map.erase(signal.index->Name());
            });
    }

    template<class T>
    struct AssetCuratorTraitsBase
    {
        using DebugStatisticsSize = size_t Debug::Statistics::Memory::*;
    };
}

namespace Inscription
{
    template<class T>
    class Scribe<::Atmos::Asset::AssetCurator<T>, BinaryArchive> :
        public ArcaNullScribe<::Atmos::Asset::AssetCurator<T>, BinaryArchive>
    {
    private:
        using BaseT = ArcaNullScribe<::Atmos::Asset::AssetCurator<T>, BinaryArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
        
        using BaseT::Scriven;
    };
}