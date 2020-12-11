#include "ImageMaterialAsset.h"

#include "ShouldCreateAsset.h"

namespace Atmos::Asset
{
    ImageMaterial::ImageMaterial(
        Arca::RelicInit init,
        const Atmos::Name& name,
        std::vector<Pass> passes)
        :
        Material(init, name, passes)
    {}

    ImageMaterial::ImageMaterial(Arca::RelicInit init, Arca::Serialization serialization) :
        Material(init, serialization)
    {}

    ImageMaterial& ImageMaterial::operator=(ImageMaterial&& arg) noexcept
    {
        Material::operator=(std::move(arg));
        return *this;
    }
}

namespace Arca
{
    bool Traits<Atmos::Asset::ImageMaterial>::ShouldCreate(
        Reliquary& reliquary,
        const Atmos::Name& name,
        const std::vector<Atmos::Asset::Material::Pass>&)
    {
        return Atmos::Asset::ShouldCreate<Atmos::Asset::ImageMaterial>(reliquary, name);
    }
}