#pragma once

#include <Arca/Relic.h>

#include "RandomNumberGenerator.h"

namespace Atmos::Random
{
    class Information
    {
    public:
        NumberGenerator random;
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Random::Information>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static TypeName TypeName() { return "Atmos::Random::Information"; }
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Random::Information, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Random::Information>;
    };
}