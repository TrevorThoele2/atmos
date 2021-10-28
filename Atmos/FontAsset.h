#pragma once

#include "AssetWithResource.h"
#include "FontAssetResource.h"

#include "LoadAssetsUserContext.h"
#include "CreateFontAssetResource.h"
#include "LoadFontAssetResourceData.h"

#include "Size2D.h"

namespace Atmos::Asset
{
    class Font final : public AssetWithResource<Resource::Font>
    {
    public:
        Font(
            Arca::RelicInit init,
            const Atmos::Name& name,
            ResourcePtr&& resource);
        Font(Arca::RelicInit init, Arca::Serialization serialization);
        Font(Font&& arg) noexcept;

        Font& operator=(Font&& arg) noexcept;

        bool operator==(const Font& arg) const;
        bool operator!=(const Font& arg) const;
    public:
        void Setup(ResourcePtr&& set);
    public:
        [[nodiscard]] Spatial::Size2D Size(const String& string, bool bold, bool italics) const;
    private:
        Arca::RelicInit init;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::Font>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static TypeName TypeName() { return "Atmos::Asset::Font"; }
        static bool ShouldCreate(
            Reliquary& reliquary,
            const Atmos::Name& name,
            const Atmos::Asset::Font::ResourcePtr& data);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Asset::Font> final
    {
    public:
        using ObjectT = Atmos::Asset::Font;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            BaseScriven<Atmos::Asset::AssetWithResource<Atmos::Asset::Resource::Font>>(
                object, archive);
            if (archive.IsInput())
            {
                auto& assetUserContext = *archive.template UserContext<LoadAssetsUserContext>();

                auto extracted = assetUserContext.LoadFontData(object.Name());
                if (extracted)
                {
                    using LoadResource = Atmos::Asset::Resource::LoadData<Atmos::Asset::Resource::Font>;
                    const auto loaded = object.init.owner.Do(LoadResource
                        {
                            extracted->memory
                        });

                    using CreateResource = Atmos::Asset::Resource::Create<Atmos::Asset::Resource::Font>;
                    auto resource = object.init.owner.Do(CreateResource
                        {
                            loaded.buffer
                        });

                    object.Setup(std::move(resource));
                }
            }
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Asset::Font, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Asset::Font>;
    };
}