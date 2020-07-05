#pragma once

#include <Arca/Curator.h>

#include "Asset.h"
#include "MappedAssets.h"
#include "FindAsset.h"

#include "DebugValue.h"

namespace Atmos::Asset
{
    template<class T>
    struct CuratorTraits;

    template<class T>
    class Curator : public Arca::Curator
    {
    public:
        explicit Curator(Init init);

        void Work();

        Arca::Index<T> Handle(const Find<T>& command);
    private:
        using Traits = CuratorTraits<T>;
    private:
        Arca::Index<Mapped<T>> mappedAssets;
        Debug::Value debugSizeValue;

        void AddToMap(Arca::Index<T> index);
        void RemoveFromMap(const String& name);
    private:
        INSCRIPTION_ACCESS;
    };

    template<class T>
    Curator<T>::Curator(Init init) :
        Arca::Curator(init),
        mappedAssets(init.owner),
        debugSizeValue(
            [this](Debug::Statistics& statistics)
            {
                statistics.memory.*Traits::debugStatisticsSize = mappedAssets->Size();
            },
                MutablePointer())
    {
        init.owner.On<Arca::CreatedKnown<T>>(
            [this](const Arca::CreatedKnown<T>& signal)
            {
                AddToMap(signal.reference);
            });

        init.owner.On<Arca::DestroyingKnown<T>>(
            [this](const Arca::DestroyingKnown<T>& signal)
            {
                RemoveFromMap(signal.reference->Name());
            });

        init.owner.On<Arca::AssigningKnown<T>>(
            [this](const Arca::AssigningKnown<T>& signal)
            {
                RemoveFromMap(signal.reference->Name());
            });

        init.owner.On<Arca::AssignedKnown<T>>(
            [this](const Arca::AssignedKnown<T>& signal)
            {
                AddToMap(signal.reference);
            });
    }

    template<class T>
    void Curator<T>::Work()
    {
        debugSizeValue.Set();
    }

    template<class T>
    Arca::Index<T> Curator<T>::Handle(const Find<T>& command)
    {
        return mappedAssets->Find(command.name);
    }

    template<class T>
    void Curator<T>::AddToMap(Arca::Index<T> index)
    {
        const auto name = index->Name();
        MutablePointer().Of(mappedAssets)->map.emplace(name, index);
    }

    template<class T>
    void Curator<T>::RemoveFromMap(const String& name)
    {
        MutablePointer().Of(mappedAssets)->map.erase(name);
    }

    template<class T>
    struct CuratorTraitsBase
    {
        using DebugStatisticsSize = size_t Debug::Statistics::Memory::*;
    };
}

namespace Inscription
{
    template<class T>
    class Scribe<::Atmos::Asset::Curator<T>, BinaryArchive> :
        public ArcaNullScribe<::Atmos::Asset::Curator<T>, BinaryArchive>
    {
    private:
        using BaseT = ArcaNullScribe<::Atmos::Asset::Curator<T>, BinaryArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
        
        using BaseT::Scriven;
    };
}