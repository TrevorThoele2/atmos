#pragma once

#include <functional>

#include "Serialization.h"

namespace Atmos
{
    namespace Modulator
    {
        typedef size_t LinkID;
        typedef std::function<LinkID(::Inscription::Scribe&)> LinkIDResolver;
    }
}