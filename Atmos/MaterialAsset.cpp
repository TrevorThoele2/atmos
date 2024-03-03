#include "MaterialAsset.h"

#include "ShouldCreateAsset.h"

namespace Atmos::Asset
{
    MaterialAsset::MaterialAsset(Init init) :
        Asset(init)
    {}

    MaterialAsset::MaterialAsset(
        Init init,
        const Atmos::Name& name,
        Arca::Index<ShaderAsset> vertexShader,
        Arca::Index<ShaderAsset> fragmentShader)
        :
        Asset(init, name),
        vertexShader(std::move(vertexShader)), fragmentShader(std::move(fragmentShader))
    {}

    MaterialAsset::MaterialAsset(MaterialAsset&& arg) noexcept :
        Asset(std::move(arg)),
        vertexShader(std::move(arg.vertexShader)), fragmentShader(std::move(arg.fragmentShader))
    {}

    MaterialAsset& MaterialAsset::operator=(MaterialAsset && arg) noexcept
    {
        Asset::operator=(std::move(arg));
        vertexShader = std::move(arg.vertexShader);
        fragmentShader = std::move(arg.fragmentShader);
        return *this;
    }

    Arca::Index<ShaderAsset> MaterialAsset::VertexShader() const
    {
        return vertexShader;
    }

    Arca::Index<ShaderAsset> MaterialAsset::FragmentShader() const
    {
        return fragmentShader;
    }
}

namespace Arca
{
    bool Traits<::Atmos::Asset::MaterialAsset>::ShouldCreate(
        Reliquary& reliquary,
        const ::Atmos::Name& name,
        Index<Atmos::Asset::ShaderAsset>,
        Index<Atmos::Asset::ShaderAsset>)
    {
        return Atmos::Asset::ShouldCreateAsset<::Atmos::Asset::MaterialAsset>(reliquary, name);
    }
}

namespace Inscription
{
    void Scribe<Atmos::Asset::MaterialAsset, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<Atmos::Asset::Asset<Atmos::Asset::MaterialAsset>>(object, archive);
        archive(object.vertexShader);
        archive(object.fragmentShader);
    }
}