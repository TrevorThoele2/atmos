
#include "Asset.h"

namespace Atmos
{
    Asset::Asset() : id(nullID)
    {}

    bool Asset::operator==(const Asset &arg) const
    {
        return id == arg.id;
    }

    bool Asset::operator!=(const Asset &arg) const
    {
        return !(*this == arg);
    }

    Asset::ID Asset::GetID() const
    {
        return id;
    }

    String Asset::GetString() const
    {
        return GetStringImpl();
    }
}