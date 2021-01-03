#include "ActionAsset.h"

#include "ShouldCreateAsset.h"

namespace Atmos::Asset
{
    Action::Action(
        Arca::RelicInit init,
        const Atmos::Name& name,
        const MappedKeys& mappedKeys)
        :
        Asset(init, name),
        mappedKeys(mappedKeys)
    {}

    Action::Action(Arca::RelicInit init, Arca::Serialization serialization) :
        Asset(init, serialization)
    {}

    Action::Action(Action&& arg) noexcept :
        Asset(std::move(arg)),
        mappedKeys(std::move(arg.mappedKeys))
    {}

    Action& Action::operator=(Action&& arg) noexcept
    {
        Asset::operator=(std::move(arg));
        mappedKeys = std::move(arg.mappedKeys);
        return *this;
    }
}

namespace Arca
{
    bool Traits<Atmos::Asset::Action>::ShouldCreate(
        Reliquary& reliquary,
        const Atmos::Name& name,
        Atmos::Asset::Action::MappedKeys mappedKeys)
    {
        return Atmos::Asset::ShouldCreate<Atmos::Asset::Action>(reliquary, name);
    }
}