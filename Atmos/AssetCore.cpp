#include "AssetCore.h"

namespace Atmos::Asset
{
    Core::Core(const Name& name) : name(name)
    {}

    bool Core::operator==(const Core& arg) const
    {
        return name == arg.name;
    }

    bool Core::operator!=(const Core& arg) const
    {
        return !(*this == arg);
    }
}