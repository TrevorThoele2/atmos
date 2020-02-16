#include "ScriptAsset.h"

#include "ShouldCreateAsset.h"

namespace Atmos::Asset
{
    ScriptAsset::ScriptAsset(Init init) : FileAsset(init)
    {}

    ScriptAsset::ScriptAsset(ScriptAsset&& arg) noexcept : FileAsset(std::move(arg))
    {}

    ScriptAsset& ScriptAsset::operator=(ScriptAsset&& arg) noexcept
    {
        FileAsset::operator=(std::move(arg));
        return *this;
    }
}

namespace Arca
{
    bool Traits<::Atmos::Asset::ScriptAsset>::ShouldCreate(
        Reliquary& reliquary, const ::Atmos::Name& name, ::Atmos::Asset::ScriptAsset::DataPtr&& data)
    {
        return Atmos::Asset::ShouldCreateAsset<::Atmos::Asset::ScriptAsset>(reliquary, name);
    }
}

namespace Inscription
{
    void Scribe<Atmos::Asset::ScriptAsset, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<Atmos::Asset::FileAsset<Atmos::Asset::ScriptAssetData, Atmos::Asset::ScriptAsset>>(
            object, archive);
    }
}