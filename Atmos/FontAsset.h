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
        static constexpr ObjectType objectType = ObjectType::Relic;
        static const inline TypeName typeName = "Atmos::Asset::Font";
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
        template<class Format>
        void Scriven(ObjectT& object, Format& format)
        {
            BaseScriven<Atmos::Asset::AssetWithResource<Atmos::Asset::Resource::Font>>(
                object, format);
            if (format.IsInput())
            {
                auto& assetUserContext = *format.template UserContext<LoadAssetsUserContext>();

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

    template<class Format>
    struct ScribeTraits<Atmos::Asset::Font, Format> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Asset::Font>;
    };
}