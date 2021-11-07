#pragma once

#include <Arca/Curator.h>

#include "Log.h"

namespace Atmos::Logging
{
    class Curator final : public Arca::Curator
    {
    public:
        explicit Curator(Init init);
    public:
        void Handle(const Log& log);
    };
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Logging::Curator>
    {
        static constexpr ObjectType objectType = ObjectType::Curator;
        static const inline TypeName typeName = "Atmos::Logging::Curator";
        using HandledCommands = Arca::HandledCommands<Atmos::Logging::Log>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Logging::Curator, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Logging::Curator>;
    };
}