
#pragma once

namespace Atmos
{
    struct NullDeleter
    {
        template<typename T>
        void operator()(T*) {};
    };
}