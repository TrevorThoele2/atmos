#pragma once

#include <Arca/Curator.h>

#include "String.h"

#include <Chroma/Function.h>

namespace Atmos
{
    class DebugScreen : public Arca::Curator
    {};
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::DebugScreen, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::DebugScreen>;
    };
}