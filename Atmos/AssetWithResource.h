#pragma once

#include "Asset.h"

namespace Atmos::Asset
{
    template<class ResourceType>
    class AssetWithResource : public Asset
    {
    public:
        using ResourceT = ResourceType;
        using ResourcePtr = std::unique_ptr<ResourceT>;
    public:
        AssetWithResource(Arca::RelicInit init, const Atmos::Name& name, ResourcePtr&& resource);
        AssetWithResource(Arca::RelicInit init, Arca::Serialization serialization);
        AssetWithResource(const AssetWithResource& arg) = delete;
        AssetWithResource(AssetWithResource&& arg) noexcept = default;

        AssetWithResource& operator=(AssetWithResource&& arg) noexcept;

        bool operator==(const AssetWithResource& arg) const;
        bool operator!=(const AssetWithResource& arg) const;

        [[nodiscard]] ResourceT* Resource();
        [[nodiscard]] const ResourceT* Resource() const;
        template<class RealResourceT>
        [[nodiscard]] RealResourceT* ResourceAs();
        template<class RealResourceT>
        [[nodiscard]] const RealResourceT* ResourceAs() const;

        [[nodiscard]] bool ContainsResource() const;
    protected:
        void SetResource(ResourcePtr&& set);
    private:
        ResourcePtr resource;
    private:
        INSCRIPTION_ACCESS;
    };

    template<class AssetData>
    AssetWithResource<AssetData>::AssetWithResource(
        Arca::RelicInit init, const Atmos::Name& name, ResourcePtr&& resource)
        :
        Asset(init, name),
        resource(std::move(resource))
    {}

    template<class AssetData>
    AssetWithResource<AssetData>::AssetWithResource(
        Arca::RelicInit init, Arca::Serialization serialization)
        :
        Asset(init, serialization)
    {}

    template<class AssetData>
    AssetWithResource<AssetData>& AssetWithResource<AssetData>::operator=(AssetWithResource&& arg) noexcept
    {
        Asset::operator=(std::move(arg));
        resource = std::move(arg.resource);
        return *this;
    }

    template<class AssetData>
    bool AssetWithResource<AssetData>::operator==(const AssetWithResource& arg) const
    {
        return resource == arg.resource;
    }

    template<class AssetData>
    bool AssetWithResource<AssetData>::operator!=(const AssetWithResource& arg) const
    {
        return !(*this == arg);
    }

    template<class AssetData>
    auto AssetWithResource<AssetData>::Resource() -> ResourceT*
    {
        return resource.get();
    }

    template<class AssetData>
    auto AssetWithResource<AssetData>::Resource() const -> const ResourceT*
    {
        return resource.get();
    }

    template<class AssetData>
    template<class RealResourceT>
    RealResourceT* AssetWithResource<AssetData>::ResourceAs()
    {
        return static_cast<RealResourceT*>(resource.get());
    }

    template<class AssetData>
    template<class RealResourceT>
    const RealResourceT* AssetWithResource<AssetData>::ResourceAs() const
    {
        return static_cast<RealResourceT*>(resource.get());
    }

    template<class AssetData>
    bool AssetWithResource<AssetData>::ContainsResource() const
    {
        return static_cast<bool>(resource);
    }

    template<class AssetData>
    void AssetWithResource<AssetData>::SetResource(ResourcePtr&& set)
    {
        resource = std::move(set);
    }
}

namespace Inscription
{
    template<class AssetData>
    class Scribe<Atmos::Asset::AssetWithResource<AssetData>> final
    {
    public:
        using ObjectT = Atmos::Asset::AssetWithResource<AssetData>;
    public:
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            Inscription::BaseScriven<Atmos::Asset::Asset>(object, format);
        }
    };

    template<class AssetData, class Format>
    struct ScribeTraits<Atmos::Asset::AssetWithResource<AssetData>, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Asset::AssetWithResource<AssetData>>;
    };
}