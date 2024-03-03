#pragma once

#include <Arca/Curator.h>

#include "Asset.h"
#include "MappedAssets.h"
#include "FindAsset.h"

#include "DebugValue.h"

namespace Atmos::Asset
{
    template<class T>
    struct AssetCuratorTraits;

    template<class T>
    class AssetCurator : public Arca::Curator
    {
    public:
        explicit AssetCurator(Init init);

        void Work();

        Arca::Index<T> Handle(const FindAsset<T>& command);
    private:
        using Traits = AssetCuratorTraits<T>;
    private:
        Arca::Index<MappedAssets<T>> mappedAssets;
        Debug::Value debugSizeValue;
    private:
        INSCRIPTION_ACCESS;
    };

    template<class T>
    AssetCurator<T>::AssetCurator(Init init) :
        Curator(init),
        mappedAssets(init.owner),
        debugSizeValue(
            [this](Debug::Statistics& statistics)
            {
                statistics.memory.*Traits::debugStatisticsSize = mappedAssets->Size();
            },
                MutablePointer())
    {
        init.owner.ExecuteOn<Arca::CreatedKnown<T>>(
            [this](const Arca::CreatedKnown<T>& signal)
            {
                MutablePointer().Of(mappedAssets)->map.emplace(signal.reference->Name(), signal.reference);
            });

        init.owner.ExecuteOn<Arca::DestroyingKnown<T>>(
            [this](const Arca::DestroyingKnown<T>& signal)
            {
                MutablePointer().Of(mappedAssets)->map.erase(signal.reference->Name());
            });
    }

    template<class T>
    void AssetCurator<T>::Work()
    {
        debugSizeValue.Set();
    }

    template<class T>
    Arca::Index<T> AssetCurator<T>::Handle(const FindAsset<T>& command)
    {
        return mappedAssets->Find(command.name);
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