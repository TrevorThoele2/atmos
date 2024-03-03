#include "ScriptAsset.h"

#include "ShouldCreateAsset.h"

namespace Atmos::Asset
{
    Script::Script(Init init, const ::Atmos::Name& name, ResourcePtr&& resource) :
        AssetWithResource(init, name, std::move(resource))
    {}

    Script::Script(Init init, Arca::Serialization serialization) :
        AssetWithResource(init, serialization)
    {}

    Script::Script(Script&& arg) noexcept : AssetWithResource(std::move(arg))
    {}

    Script& Script::operator=(Script&& arg) noexcept
    {
        AssetWithResource::operator=(std::move(arg));
        return *this;
    }
}

namespace Arca
{
    bool Traits<::Atmos::Asset::Script>::ShouldCreate(
        Reliquary& reliquary, const ::Atmos::Name& name, const ::Atmos::Asset::Script::ResourcePtr& data)
    {
        return Atmos::Asset::ShouldCreate<::Atmos::Asset::Script>(reliquary, name);
    }
}

namespace Inscription
{
    void Scribe<Atmos::Asset::Script, BinaryArchive>::ScrivenImplementation(
        ObjectT& object, ArchiveT& archive)
    {
        BaseScriven<Atmos::Asset::AssetWithResource<Atmos::Asset::Resource::Script, Atmos::Asset::Script>>(
            object, archive);
    }
}