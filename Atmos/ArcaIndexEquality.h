#pragma once

#include <Arca/Index.h>

namespace Arca
{
    class OpenRelic;
}

namespace Atmos
{
    template<class T>
    bool IndexEqualsValue(const Arca::Index<T>& left, const Arca::Index<T>& right)
    {
        return !left || !right ? !left && !right : *left == *right;
    }

    bool IndexEqualsValue(const Arca::Index<Arca::OpenRelic>& left, const Arca::Index<Arca::OpenRelic>& right);
}