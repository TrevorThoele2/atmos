#include "Asset.h"

namespace Atmos::Asset
{
    Asset::Asset(Arca::RelicInit init, const Atmos::Name& name) :
        core(init.Create<Atmos::Asset::Core>(name))
    {}

    Asset::Asset(Arca::RelicInit init, Arca::Serialization) :
        core(init.Find<Atmos::Asset::Core>())
    {}

    Asset& Asset::operator=(Asset&& arg) noexcept
    {
        core = std::move(arg.core);
        return *this;
    }

    Atmos::Name Asset::Name() const
    {
        return core->name;
    }

    Arca::Index<Core> Asset::Core() const
    {
        return core;
    }
}