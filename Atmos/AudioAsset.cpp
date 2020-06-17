#include "AudioAsset.h"

#include "ShouldCreateAsset.h"

namespace Atmos::Asset
{
    Audio::Audio(Init init) : AssetWithResource(init)
    {}

    Audio::Audio(Init init, const ::Atmos::Name& name, ResourcePtr&& resource) :
        AssetWithResource(init, name, std::move(resource))
    {}

    Audio::Audio(Audio&& arg) noexcept : AssetWithResource(std::move(arg))
    {}

    Audio& Audio::operator=(Audio&& arg) noexcept
    {
        AssetWithResource::operator=(std::move(arg));
        return *this;
    }
}

namespace Arca
{
    bool Traits<::Atmos::Asset::Audio>::ShouldCreate(
        Reliquary& reliquary, const ::Atmos::Name& name, const ::Atmos::Asset::Audio::ResourcePtr& data)
    {
        return Atmos::Asset::ShouldCreate<::Atmos::Asset::Audio>(reliquary, name);
    }
}

namespace Inscription
{
    void Scribe<::Atmos::Asset::Audio, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<Atmos::Asset::AssetWithResource<Atmos::Asset::Resource::Audio, Atmos::Asset::Audio>>(
            object, archive);
    }
}