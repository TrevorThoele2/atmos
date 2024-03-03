
#pragma once

#include <Chroma/Event.h>

namespace Atmos
{
    template<class... Args>
    using Event = Chroma::Event<Args...>;
}