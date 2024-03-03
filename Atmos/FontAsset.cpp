#include "FontAsset.h"

#include "ShouldCreateAsset.h"

namespace Atmos::Asset
{
    Font::Font(
        Arca::RelicInit init,
        const Atmos::Name& name,
        ResourcePtr&& resource)
        :
        AssetWithResource(init, name, std::move(resource)),
        init(init)
    {}

    Font::Font(Arca::RelicInit init, Arca::Serialization serialization) :
        AssetWithResource(init, serialization),
        init(init)
    {}

    Font::Font(Font&& arg) noexcept :
        AssetWithResource(std::move(arg)),
        init(arg.init)
    {}

    Font& Font::operator=(Font&& arg) noexcept
    {
        AssetWithResource::operator=(std::move(arg));
        return *this;
    }

    bool Font::operator==(const Font& arg) const
    {
        return AssetWithResource::operator==(arg);
    }

    bool Font::operator!=(const Font& arg) const
    {
        return !(*this == arg);
    }

    void Font::Setup(ResourcePtr&& set)
    {
        SetResource(std::move(set));
    }

    Spatial::Size2D Font::Size(const String& string, bool bold, bool italics) const
    {
        return ContainsResource() ? Resource()->Size(string, bold, italics) : Spatial::Size2D{};
    }
}

namespace Arca
{
    bool Traits<Atmos::Asset::Font>::ShouldCreate(
        Reliquary& reliquary,
        const Atmos::Name& name,
        const Atmos::Asset::Font::ResourcePtr&)
    {
        return Atmos::Asset::ShouldCreate<Atmos::Asset::Font>(reliquary, name);
    }
}