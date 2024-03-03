#include "LineMaterialAsset.h"

#include "ShouldCreateAsset.h"

namespace Atmos::Asset
{
    LineMaterial::LineMaterial(
        Arca::RelicInit init,
        const Atmos::Name& name,
        std::vector<Pass> passes)
        :
        Material(init, name, passes)
    {}

    LineMaterial::LineMaterial(Arca::RelicInit init, Arca::Serialization serialization) :
        Material(init, serialization)
    {}

    LineMaterial& LineMaterial::operator=(LineMaterial&& arg) noexcept
    {
        Material::operator=(std::move(arg));
        return *this;
    }
}

namespace Arca
{
    bool Traits<Atmos::Asset::LineMaterial>::ShouldCreate(
        Reliquary& reliquary,
        const Atmos::Name& name,
        const std::vector<Atmos::Asset::Material::Pass>&)
    {
        return Atmos::Asset::ShouldCreate<Atmos::Asset::LineMaterial>(reliquary, name);
    }
}