#include "MaterialAsset.h"

#include "ShouldCreateAsset.h"

namespace Atmos::Asset
{
    Material::Pass::Pass(Arca::Index<Shader> vertexShader, Arca::Index<Shader> fragmentShader) :
        vertexShader(vertexShader), fragmentShader(fragmentShader)
    {}

    Arca::Index<Shader> Material::Pass::VertexShader() const
    {
        return vertexShader;
    }

    Arca::Index<Shader> Material::Pass::FragmentShader() const
    {
        return fragmentShader;
    }

    Material::Material(
        Init init,
        const Atmos::Name& name,
        MaterialType type,
        std::vector<Pass> passes)
        :
        Asset(init, name),
        type(type), passes(std::move(passes))
    {}

    Material::Material(Init init, Arca::Serialization serialization) :
        Asset(init, serialization)
    {}

    Material::Material(Material&& arg) noexcept :
        Asset(std::move(arg)),
        type(arg.type), passes(std::move(arg.passes))
    {}

    Material& Material::operator=(Material && arg) noexcept
    {
        Asset::operator=(std::move(arg));
        type = arg.type;
        passes = std::move(arg.passes);
        return *this;
    }

    MaterialType Material::Type() const
    {
        return type;
    }

    auto Material::Passes() const -> std::vector<Pass>
    {
        return passes;
    }
}

namespace Arca
{
    bool Traits<::Atmos::Asset::Material>::ShouldCreate(
        Reliquary& reliquary,
        const ::Atmos::Name& name,
        Atmos::Asset::MaterialType,
        const std::vector<Atmos::Asset::Material::Pass>&)
    {
        return Atmos::Asset::ShouldCreate<::Atmos::Asset::Material>(reliquary, name);
    }
}