#pragma once

#include "ArcaRelicIncludes.h"
#include "Name.h"

#include "StringUtility.h"
using namespace std::string_literals;

namespace Atmos::Asset
{
    template<class AssetT>
    class Mapped
    {
    public:
        std::unordered_map<Name, Arca::Index<AssetT>> map{};
    public:
        Mapped() = default;
    public:
        [[nodiscard]] Arca::Index<AssetT> Find(const Name& name) const;

        [[nodiscard]] bool Exists(const Name& name) const;

        [[nodiscard]] size_t Size() const;
    };

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
        static constexpr ObjectType objectType = ObjectType::Relic;
        static const inline TypeName typeName = "Atmos::Asset::Mapped<"s + TypeFor<AssetT>().name + ">";
        static constexpr Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class AssetT, class Format>
    struct ScribeTraits<Atmos::Asset::Mapped<AssetT>, Format> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Asset::Mapped<AssetT>>;
    };
}