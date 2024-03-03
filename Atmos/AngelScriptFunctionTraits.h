#pragma once

#include <Chroma/FunctionTraits.h>

namespace Atmos::Scripting::Angel
{
    template<auto toArca>
    using FunctionTraits = Chroma::FunctionTraits<std::remove_const_t<decltype(toArca)>>;
}