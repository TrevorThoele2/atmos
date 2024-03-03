#pragma once

#include <Arca/Curator.h>
#include <Arca/Reliquary.h>
#include <Arca/Created.h>
#include <Arca/Destroying.h>
#include <Arca/Actualization.h>

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
    protected:
        AssetCurator() = default;
    protected:
        void InitializeImplementation() override;
        void WorkImplementation() override;
    private:
        using Traits = AssetCuratorTraits<T>;
    private:
        MappedAssets<T>* mappedAssets = nullptr;
        Debug::Statistics* debugStatistics = nullptr;

        Arca::Batch<Arca::Created> createdBatch{};
        Arca::Batch<Arca::Destroying> destroyingBatch{};
    private:
        INSCRIPTION_ACCESS;
    };

    template<class T>
    void AssetCurator<T>::InitializeImplementation()
    {
        mappedAssets = Owner().Find<MappedAssets<T>>();
        debugStatistics = Owner().Find<Debug::Statistics>();

        createdBatch = Owner().Batch<Arca::Created>();
        destroyingBatch = Owner().Batch<Arca::Destroying>();
    }

    template<class T>
    void AssetCurator<T>::WorkImplementation()
    {
        for (auto& loop : createdBatch)
        {
            auto actualized = Arca::Actualize<T>(loop.handle);
            if (!actualized)
                continue;

            mappedAssets->map.emplace(actualized->Name(), actualized);
        }

        for (auto& loop : destroyingBatch)
        {
            auto actualized = Arca::Actualize<T>(loop.handle);
            if (!actualized)
                continue;

            mappedAssets->map.erase(actualized->Name());
        }

        (debugStatistics->memory.*Traits::debugStatisticsSize) = mappedAssets->Size();
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