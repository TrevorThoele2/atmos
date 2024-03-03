#include "AudioAsset.h"

#include "ShouldCreateAsset.h"

namespace Atmos::Asset
{
    Audio::Audio(Init init, const ::Atmos::Name& name, ResourcePtr&& resource) :
        AssetWithResource(init, name, std::move(resource))
    {}

    Audio::Audio(Init init, Arca::Serialization serialization) :
        AssetWithResource(init, serialization)
    {}

    Audio::Audio(Audio&& arg) noexcept : AssetWithResource(std::move(arg))
    {}

    Audio& Audio::operator=(Audio&& arg) noexcept
    {
        AssetWithResource::operator=(std::move(arg));
        return *this;
    }

    void Audio::Setup(ResourcePtr&& set)
    {
        SetResource(std::move(set));
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