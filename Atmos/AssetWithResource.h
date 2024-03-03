#pragma once

#include "Asset.h"

namespace Atmos::Asset
{
    template<class ResourceType, class Derived>
    class AssetWithResource : public Asset<Derived>
    {
    private:
        using BaseT = Asset<Derived>;
    public:
        using ResourceT = ResourceType;
        using ResourcePtr = std::unique_ptr<ResourceT>;
    protected:
        using Init = typename BaseT::Init;
    public:
        AssetWithResource(Init init, const Atmos::Name& name, ResourcePtr&& resource);
        AssetWithResource(Init init, Arca::Serialization serialization);
        AssetWithResource(const AssetWithResource& arg) = delete;
        AssetWithResource(AssetWithResource&& arg) noexcept = default;

        AssetWithResource& operator=(AssetWithResource&& arg) noexcept;

        [[nodiscard]] ResourceT* Resource();
        [[nodiscard]] const ResourceT* Resource() const;
        template<class RealFileDataT>
        [[nodiscard]] RealFileDataT* ResourceAs();
        template<class RealFileDataT>
        [[nodiscard]] const RealFileDataT* ResourceAs() const;

        [[nodiscard]] bool ContainsResource() const;
    private:
        ResourcePtr resource;
    private:
        INSCRIPTION_ACCESS;
    };

    template<class AssetData, class Derived>
    AssetWithResource<AssetData, Derived>::AssetWithResource(Init init, const Atmos::Name& name, ResourcePtr&& resource) :
        Asset(init, name),
        resource(std::move(resource))
    {}

    template<class AssetData, class Derived>
    AssetWithResource<AssetData, Derived>::AssetWithResource(Init init, Arca::Serialization serialization) :
        Asset(init, serialization)
    {}

    template<class AssetData, class Derived>
    AssetWithResource<AssetData, Derived>& AssetWithResource<AssetData, Derived>::operator=(AssetWithResource&& arg) noexcept
    {
        Asset<Derived>::operator=(std::move(arg));
        resource = std::move(arg.resource);
        return *this;
    }

    template<class AssetData, class Derived>
    auto AssetWithResource<AssetData, Derived>::Resource() -> ResourceT*
    {
        return resource.get();
    }

    template<class AssetData, class Derived>
    auto AssetWithResource<AssetData, Derived>::Resource() const -> const ResourceT*
    {
        return resource.get();
    }

    template<class AssetData, class Derived>
    template<class RealFileDataT>
    RealFileDataT* AssetWithResource<AssetData, Derived>::ResourceAs()
    {
        return static_cast<RealFileDataT*>(resource.get());
    }

    template<class AssetData, class Derived>
    template<class RealFileDataT>
    const RealFileDataT* AssetWithResource<AssetData, Derived>::ResourceAs() const
    {
        return static_cast<RealFileDataT*>(resource.get());
    }

    template<class AssetData, class Derived>
    bool AssetWithResource<AssetData, Derived>::ContainsResource() const
    {
        return static_cast<bool>(resource);
    }
}

namespace Inscription
{
    template<class AssetData, class Derived>
    class Scribe<::Atmos::Asset::AssetWithResource<AssetData, Derived>, BinaryArchive> final :
        public ArcaCompositeScribe<::Atmos::Asset::AssetWithResource<AssetData, Derived>, BinaryArchive>
    {
    private:
        using BaseT = ArcaCompositeScribe<::Atmos::Asset::AssetWithResource<AssetData, Derived>, BinaryArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;

        using BaseT::Scriven;
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override
        {
            Inscription::BaseScriven<Atmos::Asset::Asset<Derived>>(object, archive);
        }
    };
}