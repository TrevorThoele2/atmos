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

    void Script::Setup(ResourcePtr&& set)
    {
        SetResource(std::move(set));
    }
}

namespace Arca
{
    bool Traits<Atmos::Asset::Script>::ShouldCreate(
        Reliquary& reliquary, const Atmos::Name& name, const ::Atmos::Asset::Script::ResourcePtr& data)
    {
        return Atmos::Asset::ShouldCreate<::Atmos::Asset::Script>(reliquary, name);
    }
}