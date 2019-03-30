#pragma once

#include <type_traits>

namespace Atmos
{
    class Object;
    class ObjectSystem;

    template<class T>
    struct IsAtmosObject : std::is_base_of<Object, T>
    {};

    template<class T>
    struct IsAtmosObjectSystem : std::is_base_of<ObjectSystem, T>
    {};
}