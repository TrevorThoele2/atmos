#include "AudioAsset.h"

#include "ShouldCreateAsset.h"

namespace Atmos::Asset
{
    Audio::Audio(Arca::RelicInit init, const ::Atmos::Name& name, ResourcePtr&& resource) :
        AssetWithResource(init, name, std::move(resource)),
        init(init)
    {}

    Audio::Audio(Arca::RelicInit init, Arca::Serialization serialization) :
        AssetWithResource(init, serialization),
        init(init)
    {}

    Audio::Audio(Audio&& arg) noexcept :
        AssetWithResource(std::move(arg)),
        init(arg.init)
    {}

    Audio& Audio::operator=(Audio&& arg) noexcept
    {
        AssetWithResource::operator=(std::move(arg));
        return *this;
    }

    bool Audio::operator==(const Audio& arg) const
    {
        return AssetWithResource::operator==(arg);
    }

    bool Audio::operator!=(const Audio& arg) const
    {
        return !(*this == arg);
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