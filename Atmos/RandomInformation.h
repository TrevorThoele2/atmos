#pragma once

#include "ArcaRelicIncludes.h"

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
        static constexpr ObjectType objectType = ObjectType::Relic;
        static const inline TypeName typeName = "Atmos::Random::Information";
        static constexpr Locality locality = Locality::Global;
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