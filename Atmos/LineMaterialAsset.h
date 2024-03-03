#pragma once

#include "MaterialAsset.h"

namespace Atmos::Asset
{
    class LineMaterial final : public Material
    {
    public:
        LineMaterial(
            Arca::RelicInit init,
            const Atmos::Name& name,
            std::vector<Pass> passes);
        LineMaterial(Arca::RelicInit init, Arca::Serialization serialization);
        LineMaterial(LineMaterial&& arg) noexcept = default;

        LineMaterial& operator=(LineMaterial&& arg) noexcept;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::LineMaterial>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Atmos::Asset::LineMaterial";
        static bool ShouldCreate(
            Reliquary& reliquary,
            const Atmos::Name& name,
            const std::vector<Atmos::Asset::LineMaterial::Pass>& passes);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Asset::LineMaterial> final
    {
    public:
        using ObjectT = Atmos::Asset::LineMaterial;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            BaseScriven<Atmos::Asset::Material>(object, archive);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Asset::LineMaterial, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Asset::LineMaterial>;
    };
}