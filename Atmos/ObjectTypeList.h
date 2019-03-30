#pragma once

#include <Chroma/VariadicTemplate.h>

namespace Atmos
{
    template<class... Types>
    using ObjectTypeList = ::Chroma::VariadicTemplate<Types...>;
}