#pragma once

#include "MaterialAsset.h"

namespace Atmos::Asset
{
    class ImageMaterial final : public Material
    {
    public:
        ImageMaterial(
            Arca::RelicInit init,
            const Atmos::Name& name,
            std::vector<Pass> passes);
        ImageMaterial(Arca::RelicInit init, Arca::Serialization serialization);
        ImageMaterial(ImageMaterial&& arg) noexcept = default;

        ImageMaterial& operator=(ImageMaterial&& arg) noexcept;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::ImageMaterial>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Asset::ImageMaterial";
        static bool ShouldCreate(
            Reliquary& reliquary,
            const Atmos::Name& name,
            const std::vector<Atmos::Asset::ImageMaterial::Pass>& passes);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Asset::ImageMaterial> final
    {
    public:
        using ObjectT = Atmos::Asset::ImageMaterial;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            BaseScriven<Atmos::Asset::Material>(object, archive);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Asset::ImageMaterial, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Asset::ImageMaterial>;
    };
}