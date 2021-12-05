#pragma once

#include <Arca/Curator.h>
#include <Arca/Assign.h>

#include "Asset.h"
#include "MappedAssets.h"
#include "FindAssetByName.h"

namespace Inscription
{
    template<class T>
    class AssetScribeCategory;
}

namespace Atmos::Asset
{
    template<class T>
    struct CuratorTraits;

    template<class T>
    class Curator : public Arca::Curator
    {
    public:
        explicit Curator(Init init);
        
        Arca::Index<T> Handle(const FindByName<T>& command);
    private:
        using Traits = CuratorTraits<T>;
    private:
        Arca::Index<Mapped<T>> mappedAssets;

        void ConstructMap();
        void AddToMap(Arca::Index<T> index);
        void RemoveFromMap(const String& name);
    private:
        INSCRIPTION_ACCESS;
        template<class U>
        friend class Inscription::AssetScribeCategory;
    };

    template<class T>
    Curator<T>::Curator(Init init) : Arca::Curator(init), mappedAssets(init.owner.Find<Mapped<T>>())
    {
        init.owner.On<Arca::CreatedKnown<T>>(
            [this](const Arca::CreatedKnown<T>& signal)
            {
                AddToMap(signal.index);
            });

        init.owner.On<Arca::DestroyingKnown<T>>(
            [this](const Arca::DestroyingKnown<T>& signal)
            {
                RemoveFromMap(signal.index->Name());
            });

        init.owner.On<Arca::AssigningKnown<T>>(
            [this](const Arca::AssigningKnown<T>& signal)
            {
                RemoveFromMap(signal.index->Name());
            });

        init.owner.On<Arca::AssignedKnown<T>>(
            [this](const Arca::AssignedKnown<T>& signal)
            {
                AddToMap(signal.index);
            });
    }
    
    template<class T>
    Arca::Index<T> Curator<T>::Handle(const FindByName<T>& command)
    {
        return mappedAssets->Find(command.name);
    }

    template<class T>
    void Curator<T>::ConstructMap()
    {
        auto mutableMappedAssets = MutablePointer().Of(mappedAssets);
        auto batch = Owner().Batch<T>();
        for (auto asset = batch.begin(); asset != batch.end(); ++asset)
        {
            const auto name = asset->Name();
            mutableMappedAssets->map.emplace(name, Owner().Find<T>(asset.ID()));
        }
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
}

namespace Inscription
{
    template<class T>
    class AssetScribeCategory final
    {
    public:
        using ObjectT = T;
    public:
        static constexpr auto requiresScribe = false;
    public:
        template<class Format>
        static void Scriven(ObjectT& object, Format& format);
        template<class Format>
        static void Scriven(const std::string& name, ObjectT& object, Format& format);
    public:
        template<class Format>
        static Type OutputType(const Format& format);
    };

    template<class T>
    template<class Format>
    void AssetScribeCategory<T>::Scriven(ObjectT& object, Format& format)
    {
        if (format.IsInput())
            object.ConstructMap();
    }

    template<class T>
    template<class Format>
    void AssetScribeCategory<T>::Scriven(const std::string& name, ObjectT& object, Format& format)
    {
        Scriven(object, format);
    }

    template<class T>
    template<class Format>
    Type AssetScribeCategory<T>::OutputType(const Format&)
    {
        return Arca::TypeFor<T>().name;
    }
}