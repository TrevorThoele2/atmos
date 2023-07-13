#include "MaterialAsset.h"

#include "ShouldCreateAsset.h"

namespace Atmos::Asset
{
    Material::Material(
        Arca::RelicInit init,
        const Atmos::Name& name,
        const Arca::Index<Script>& asset,
        const String& executeName,
        const Scripting::Parameters& parameters)
        :
        Asset(init, name),
        scriptData(asset, executeName, parameters)
    {}

    Material::Material(Arca::RelicInit init, Arca::Serialization serialization) :
        Asset(init, serialization)
    {}

    Material::Material(Material&& arg) noexcept :
        Asset(std::move(arg)),
        scriptData(std::move(arg.scriptData))
    {}

    Material& Material::operator=(Material&& arg) noexcept
    {
        Asset::operator=(std::move(arg));
        scriptData = std::move(arg.scriptData);
        return *this;
    }

    bool Material::operator==(const Material& arg) const
    {
        return Asset::operator==(arg) && scriptData == arg.scriptData;
    }

    bool Material::operator!=(const Material& arg) const
    {
        return !(*this == arg);
    }
}

namespace Arca
{
    bool Traits<Atmos::Asset::Material>::ShouldCreate(
        Reliquary& reliquary,
        const Atmos::Name& name,
        const Index<Atmos::Asset::Script>&,
        const Atmos::String&,
        const Atmos::Scripting::Parameters&)
    {
        return Atmos::Asset::ShouldCreate<Atmos::Asset::Material>(reliquary, name);
    }
}