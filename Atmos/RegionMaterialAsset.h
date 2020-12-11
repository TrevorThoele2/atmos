#pragma once

#include "MaterialAsset.h"

namespace Atmos::Asset
{
    class RegionMaterial final : public Material
    {
    public:
        RegionMaterial(
            Arca::RelicInit init,
            const Atmos::Name& name,
            std::vector<Pass> passes);
        RegionMaterial(Arca::RelicInit init, Arca::Serialization serialization);
        RegionMaterial(RegionMaterial&& arg) noexcept = default;

        RegionMaterial& operator=(RegionMaterial&& arg) noexcept;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::RegionMaterial>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Asset::RegionMaterial";
        static bool ShouldCreate(
            Reliquary& reliquary,
            const Atmos::Name& name,
            const std::vector<Atmos::Asset::RegionMaterial::Pass>& passes);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Asset::RegionMaterial> final
    {
    public:
        using ObjectT = Atmos::Asset::RegionMaterial;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            BaseScriven<Atmos::Asset::Material>(object, archive);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Asset::RegionMaterial, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Asset::RegionMaterial>;
    };
}