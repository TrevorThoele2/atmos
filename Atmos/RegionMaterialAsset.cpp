#include "RegionMaterialAsset.h"

#include "ShouldCreateAsset.h"

namespace Atmos::Asset
{
    RegionMaterial::RegionMaterial(
        Arca::RelicInit init,
        const Atmos::Name& name,
        std::vector<Pass> passes)
        :
        Material(init, name, passes)
    {}

    RegionMaterial::RegionMaterial(Arca::RelicInit init, Arca::Serialization serialization) :
        Material(init, serialization)
    {}

    RegionMaterial& RegionMaterial::operator=(RegionMaterial&& arg) noexcept
    {
        Material::operator=(std::move(arg));
        return *this;
    }
}

namespace Arca
{
    bool Traits<Atmos::Asset::RegionMaterial>::ShouldCreate(
        Reliquary& reliquary,
        const Atmos::Name& name,
        const std::vector<Atmos::Asset::Material::Pass>&)
    {
        return Atmos::Asset::ShouldCreate<Atmos::Asset::RegionMaterial>(reliquary, name);
    }
}