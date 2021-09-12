#pragma once

#include "ArcaRelicIncludes.h"

#include "Volume.h"

namespace Atmos::Initialization
{
    class Information final
    {
    public:
        struct Graphics
        {
            bool fullscreen;
        } graphics{};
    public:
        struct Sound
        {
            Audio::Volume masterVolume;
        } sound{};
    public:
        struct Controls
        {
            struct Actions
            {
            } actions;
        } controls{};
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Initialization::Information>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static TypeName TypeName() { return "Atmos::Initialization::Information"; }
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Initialization::Information, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Initialization::Information>;
    };
}