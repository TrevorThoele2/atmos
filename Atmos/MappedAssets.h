#pragma once

#include <Arca/ClosedTypedRelic.h>
#include "Name.h"

#include "StringUtility.h"
using namespace std::string_literals;

namespace Atmos::Asset
{
    template<class AssetT>
    class Mapped : public Arca::ClosedTypedRelic<Mapped<AssetT>>
    {
    public:
        std::unordered_map<Name, Arca::Index<AssetT>> map{};
    private:
        using BaseT = Arca::ClosedTypedRelic<Mapped<AssetT>>;
    protected:
        using Init = typename BaseT::Init;
    public:
        explicit Mapped(Init init);
    public:
        [[nodiscard]] Arca::Index<AssetT> Find(const Name& name) const;

        [[nodiscard]] bool Exists(const Name& name) const;

        [[nodiscard]] size_t Size() const;
    };

    template<class AssetT>
    Mapped<AssetT>::Mapped(Init init) : BaseT(init)
    {}

    template<class AssetT>
    Arca::Index<AssetT> Mapped<AssetT>::Find(const Name& name) const
    {
        const auto found = map.find(name);
        if (found == map.end())
            return {};

        return found->second;
    }

    template<class AssetT>
    bool Mapped<AssetT>::Exists(const Name& name) const
    {
        return Find(name) != nullptr;
    }

    template<class AssetT>
    size_t Mapped<AssetT>::Size() const
    {
        return map.size();
    }
}

namespace Arca
{
    template<class AssetT>
    struct Traits<::Atmos::Asset::Mapped<AssetT>>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Mapped"s + TypeFor<AssetT>().name + "s";
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class AssetT>
    class Scribe<Atmos::Asset::Mapped<AssetT>, BinaryArchive>
        : public ArcaNullScribe<Atmos::Asset::Mapped<AssetT>, BinaryArchive>
    {
    private:
        using BaseT = ArcaNullScribe<Atmos::Asset::Mapped<AssetT>, BinaryArchive>;
    public:
        using BaseT::ArchiveT;
        using BaseT::ObjectT;

        using BaseT::Scriven;
    };
}