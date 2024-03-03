#include "MaterialAsset.h"

#include "ShouldCreateAsset.h"

namespace Atmos::Asset
{
    Material::Material(
        Arca::RelicInit init,
        const Atmos::Name& name,
        std::vector<Pass> passes)
        :
        Asset(init, name),
        passes(std::move(passes))
    {}

    Material::Material(Arca::RelicInit init, Arca::Serialization serialization) :
        Asset(init, serialization)
    {}

    Material::Material(Material&& arg) noexcept :
        Asset(std::move(arg)),
        passes(std::move(arg.passes))
    {}

    Material& Material::operator=(Material&& arg) noexcept
    {
        Asset::operator=(std::move(arg));
        passes = std::move(arg.passes);
        return *this;
    }

    auto Material::Passes() const -> std::vector<Pass>
    {
        return passes;
    }
}

namespace Arca
{
    bool Traits<Atmos::Asset::Material>::ShouldCreate(
        Reliquary& reliquary,
        const Atmos::Name& name,
        const std::vector<Atmos::Asset::Material::Pass>&)
    {
        return Atmos::Asset::ShouldCreate<Atmos::Asset::Material>(reliquary, name);
    }
}