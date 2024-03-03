#pragma once

#include <type_traits>

namespace Atmos
{
    template<class T>
    class AngleUnits;

    template<class T>
    struct IsAngleUnits : std::is_base_of<AngleUnits<T>, T>
    {};
}