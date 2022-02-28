#include "Material.h"

namespace Atmos::Render
{
    bool Material::operator==(const Material& arg) const
    {
        return IndexEqualsValue(script, arg.script);
    }

    bool Material::operator!=(const Material& arg) const
    {
        return !(*this == arg);
    }
}