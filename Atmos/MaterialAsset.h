#pragma once

#include "Asset.h"

#include "MaterialAssetPass.h"

#include <Inscription/VectorScribe.h>

namespace Atmos::Asset
{
    class Material final : public Asset
    {
    public:
        using Pass = MaterialPass;
    public:
        Material(
            Arca::RelicInit init,
            const Atmos::Name& name,
            std::vector<Pass> passes);
        Material(Arca::RelicInit init, Arca::Serialization serialization);
        Material(Material&& arg) noexcept;

        Material& operator=(Material&& arg) noexcept;

        bool operator==(const Material& arg) const;
        bool operator!=(const Material& arg) const;

        [[nodiscard]] std::vector<Pass> Passes() const;
    private:
        std::vector<Pass> passes;
    private:
        INSCRIPTION_ACCESS;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Asset::Material>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static TypeName TypeName() { return "Atmos::Asset::Material"; }
        static bool ShouldCreate(
            Reliquary& reliquary,
            const Atmos::Name& name,
            const std::vector<Atmos::Asset::Material::Pass>& passes);
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Asset::Material> final
    {
    public:
        using ObjectT = Atmos::Asset::Material;
    public:
        template<class Archive>
        void Scriven(ObjectT& object, Archive& archive)
        {
            BaseScriven<Atmos::Asset::Asset>(object, archive);
            archive("passes", object.passes);
        }
    };

    template<class Archive>
    struct ScribeTraits<Atmos::Asset::Material, Archive> final
    {
        using Category = ArcaCompositeScribeCategory<Atmos::Asset::Material>;
    };
}