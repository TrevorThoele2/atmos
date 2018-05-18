
#include "RegistryObject.h"

#include <Inscription/Scribe.h>

namespace Atmos
{
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(RegistryObject)
    {
        scribe(niceName);
        scribe(description);
    }

    RegistryObject::RegistryObject(RegistryObject &&arg) : id(std::move(arg.id)), name(std::move(arg.name)), niceName(std::move(arg.niceName)), description(std::move(arg.description))
    {}

    RegistryObject& RegistryObject::operator=(RegistryObject &&arg)
    {
        id = std::move(arg.id);
        name = std::move(arg.name);
        niceName = std::move(arg.niceName);
        description = std::move(arg.description);
        return *this;
    }

    bool RegistryObject::operator==(const RegistryObject &arg) const
    {
        return id == arg.id;
    }

    bool RegistryObject::operator!=(const RegistryObject &arg) const
    {
        return !(*this == arg);
    }

    const Name& RegistryObject::GetName() const
    {
        return name;
    }

    RegistryObject::ID RegistryObject::GetID() const
    {
        return id;
    }

    const Name& RegistryObject::GetNiceName() const
    {
        return niceName;
    }

    const RegistryObject::Description& RegistryObject::GetDescription() const
    {
        return description;
    }
}