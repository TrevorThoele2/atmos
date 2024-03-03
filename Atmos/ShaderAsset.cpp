#include "ShaderAsset.h"

#include "ShouldCreateAsset.h"

namespace Atmos::Asset
{
    ShaderAsset::ShaderAsset(Init init) :
        FileAsset(init)
    {}

    ShaderAsset::ShaderAsset(Init init, const Atmos::Name& name, DataPtr&& data) :
        FileAsset(init, name, std::move(data))
    {}

    ShaderAsset::ShaderAsset(ShaderAsset && arg) noexcept : FileAsset(std::move(arg))
    {}

    ShaderAsset& ShaderAsset::operator=(ShaderAsset&& arg) noexcept
    {
        FileAsset::operator=(std::move(arg));
        return *this;
    }
}

namespace Arca
{
    bool Traits<::Atmos::Asset::ShaderAsset>::ShouldCreate(
        Reliquary& reliquary, const ::Atmos::Name& name, ::Atmos::Asset::ShaderAsset::DataPtr&& data)
    {
        return Atmos::Asset::ShouldCreateAsset<::Atmos::Asset::ShaderAsset>(reliquary, name);
    }
}

namespace Inscription
{
    void Scribe<Atmos::Asset::ShaderAsset, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<Atmos::Asset::FileAsset<Atmos::Asset::ShaderAssetData, Atmos::Asset::ShaderAsset>>(
            object, archive);
    }
}