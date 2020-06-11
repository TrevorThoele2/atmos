#include "MaterialAsset.h"

#include "ShouldCreateAsset.h"

namespace Atmos::Asset
{
    Material::Material(Init init) :
        Asset(init)
    {}

    Material::Material(
        Init init,
        const Atmos::Name& name,
        MaterialType type,
        Arca::Index<Shader> vertexShader,
        Arca::Index<Shader> fragmentShader)
        :
        Asset(init, name),
        type(type), vertexShader(std::move(vertexShader)), fragmentShader(std::move(fragmentShader))
    {}

    Material::Material(Material&& arg) noexcept :
        Asset(std::move(arg)),
        type(arg.type), vertexShader(std::move(arg.vertexShader)), fragmentShader(std::move(arg.fragmentShader))
    {}

    Material& Material::operator=(Material && arg) noexcept
    {
        Asset::operator=(std::move(arg));
        type = arg.type;
        vertexShader = std::move(arg.vertexShader);
        fragmentShader = std::move(arg.fragmentShader);
        return *this;
    }

    MaterialType Material::Type() const
    {
        return type;
    }

    Arca::Index<Shader> Material::VertexShader() const
    {
        return vertexShader;
    }

    Arca::Index<Shader> Material::FragmentShader() const
    {
        return fragmentShader;
    }
}

namespace Arca
{
    bool Traits<::Atmos::Asset::Material>::ShouldCreate(
        Reliquary& reliquary,
        const ::Atmos::Name& name,
        Atmos::Asset::MaterialType,
        Index<Atmos::Asset::Shader>,
        Index<Atmos::Asset::Shader>)
    {
        return Atmos::Asset::ShouldCreate<::Atmos::Asset::Material>(reliquary, name);
    }
}

namespace Inscription
{
    void Scribe<Atmos::Asset::Material, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<Atmos::Asset::Asset<Atmos::Asset::Material>>(object, archive);
        archive(object.vertexShader);
        archive(object.fragmentShader);
    }
}