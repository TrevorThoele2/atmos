#include "ScriptAsset.h"

#include "ShouldCreateAsset.h"

namespace Atmos::Asset
{
    Script::Script(Arca::RelicInit init, const ::Atmos::Name& name, ResourcePtr&& resource) :
        AssetWithResource(init, name, std::move(resource)),
        init(init)
    {}

    Script::Script(Arca::RelicInit init, Arca::Serialization serialization) :
        AssetWithResource(init, serialization),
        init(init)
    {}

    Script::Script(Script&& arg) noexcept :
        AssetWithResource(std::move(arg)),
        init(arg.init)
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