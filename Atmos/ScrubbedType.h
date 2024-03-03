#pragma once

#include <type_traits>

namespace Atmos
{
    template<class T>
    struct Scrubbed
    {
        typedef typename std::decay<T>::type Type;
    };
}