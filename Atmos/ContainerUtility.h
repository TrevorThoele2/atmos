#pragma once

#include <algorithm>

namespace Atmos
{
    template<class Container, class T>
    bool ContainerHas(Container& container, T& object)
    {
        return std::find(container.begin(), container.end(), object) != container.end();
    }
}