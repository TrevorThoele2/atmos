#pragma once

#include <Arca/ClosedTypedRelicAutomation.h>
#include "FileAsset.h"

#include <type_traits>
using namespace std::string_literals;

#include "Name.h"

namespace Atmos::Asset
{
    template<class AssetT, class Enable = void>
    class MappedAssets;

    template<class AssetT>
    class MappedAssets<AssetT, std::enable_if_t<std::is_base_of_v<FileAsset, AssetT>>>
        : public Arca::ClosedTypedRelicAutomation<MappedAssets<AssetT>>
    {
    public:
        std::unordered_map<File::Name, AssetT*> map{};

        [[nodiscard]] AssetT* Find(const File::Name& name) const
        {
            const auto found = map.find(name);
            if (found == map.end())
                return nullptr;

            return found->second;
        }

        [[nodiscard]] bool Exists(const File::Name& name) const
        {
            return Find(name) != nullptr;
        }

        [[nodiscard]] size_t Size() const
        {
            return map.size();
        }
    };

    template<class AssetT>
    class MappedAssets<AssetT, std::enable_if_t<!std::is_base_of_v<FileAsset, AssetT>>>
        : public Arca::ClosedTypedRelicAutomation<MappedAssets<AssetT>>
    {
    public:
        std::unordered_map<Name, AssetT*> map{};

        [[nodiscard]] AssetT* Find(const Name& name) const
        {
            auto found = map.find(name);
            if (found == map.end())
                return nullptr;

            return found->second;
        }

        [[nodiscard]] bool Exists(const Name& name) const
        {
            return Find(name) != nullptr;
        }

        [[nodiscard]] size_t Size() const
        {
            return map.size();
        }
    };
}

namespace Arca
{
    template<class AssetT>
    struct Traits<::Atmos::Asset::MappedAssets<AssetT>>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Mapped"s + TypeFor<AssetT>().name + "s";
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class AssetT>
    class Scribe<Atmos::Asset::MappedAssets<AssetT>, BinaryArchive>
        : public ArcaNullScribe<Atmos::Asset::MappedAssets<AssetT>, BinaryArchive>
    {
    private:
        using BaseT = ArcaNullScribe<Atmos::Asset::MappedAssets<AssetT>, BinaryArchive>;
    public:
        using BaseT::ArchiveT;
        using BaseT::ObjectT;

        using BaseT::Scriven;
    };
}