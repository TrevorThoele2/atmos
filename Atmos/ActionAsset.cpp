#include "ActionAsset.h"

#include "ShouldCreateAsset.h"

namespace Atmos::Asset
{
    Action::Action(
        Arca::RelicInit init,
        const Atmos::Name& name,
        Input::Key boundKey,
        const Modifiers& boundModifiers)
        :
        Asset(init, name),
        boundKey(boundKey),
        boundModifiers(boundModifiers)
    {}

    Action::Action(Arca::RelicInit init, Arca::Serialization serialization) :
        Asset(init, serialization)
    {}

    Action::Action(Action&& arg) noexcept :
        Asset(std::move(arg)),
        boundKey(std::move(arg.boundKey)),
        boundModifiers(std::move(arg.boundModifiers))
    {}

    Action& Action::operator=(Action&& arg) noexcept
    {
        Asset::operator=(std::move(arg));
        boundKey = std::move(arg.boundKey);
        boundModifiers = std::move(arg.boundModifiers);
        return *this;
    }
}

namespace Arca
{
    bool Traits<Atmos::Asset::Action>::ShouldCreate(
        Reliquary& reliquary,
        const Atmos::Name& name,
        Atmos::Input::Key boundKey,
        const Atmos::Asset::Action::Modifiers& boundModifiers)
    {
        return Atmos::Asset::ShouldCreate<Atmos::Asset::Action>(reliquary, name);
    }
}